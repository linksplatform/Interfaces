#!/usr/bin/env node

/**
 * Pack the header-only C++ library as a native NuGet package.
 *
 * The shared linksplatform/Workflows deploy-cpp job ran
 * `sudo apt-get install nuget`, but Ubuntu 24.04 no longer ships the Mono
 * NuGet CLI, so every release failed before packing (issue #150). This script
 * stages the layout of the last published package (0.3.41) and packs it with
 * the .NET SDK through a throwaway project that points at the nuspec:
 *
 *   images/icon.png
 *   build/native/<id>.targets
 *   lib/native/include/*.h
 *
 * Usage: node pack-cpp-nuget.mjs <output-directory> [--verbose]
 * `--verbose`, VERBOSE=1 or RUNNER_DEBUG=1 prints the staged files and the
 * full dotnet output. Writes package_id, version and package_path to
 * GITHUB_OUTPUT and the release notes to <output-directory>/release-notes.md.
 */

import { execFileSync } from "node:child_process";
import {
  appendFileSync,
  copyFileSync,
  mkdirSync,
  mkdtempSync,
  readdirSync,
  readFileSync,
  rmSync,
  writeFileSync,
} from "node:fs";
import { tmpdir } from "node:os";
import path from "node:path";

const repositoryRoot = path.resolve(import.meta.dirname, "../..");
export const nuspecPath = path.join(
  repositoryRoot,
  "cpp/Platform.Interfaces/Platform.Interfaces.TemplateLibrary.nuspec",
);
const headersDirectory = path.dirname(nuspecPath);
const assetsDirectory = path.join(repositoryRoot, "cpp/nuget");

const readElement = (nuspec, name) => {
  const match = new RegExp(`<${name}>([^<]+)</${name}>`).exec(nuspec);
  if (!match) {
    throw new Error(`The nuspec has no <${name}> element`);
  }
  return match[1].trim();
};

export const readPackageInfo = (nuspec) => ({
  id: readElement(nuspec, "id"),
  version: readElement(nuspec, "version"),
  releaseNotes: readElement(nuspec, "releaseNotes"),
});

export const expectedEntries = (id, headers) => [
  "images/icon.png",
  `build/native/${id}.targets`,
  ...headers.map((header) => `lib/native/include/${header}`),
];

export const withStagedFiles = (nuspec) => {
  if (/<files>/.test(nuspec)) {
    throw new Error("The nuspec already lists <files>; the pack layout is generated");
  }
  return nuspec.replace(
    "</metadata>",
    '</metadata>\n  <files>\n    <file src="content/**" target="" />\n  </files>',
  );
};

export const findMissingEntries = (expected, actual) => {
  const present = new Set(actual);
  return expected.filter((entry) => !present.has(entry));
};

const listHeaders = () =>
  readdirSync(headersDirectory)
    .filter((name) => name.endsWith(".h"))
    .sort();

const listPackageEntries = (packagePath) =>
  execFileSync("unzip", ["-Z1", packagePath], { encoding: "utf8" })
    .split("\n")
    .filter(Boolean);

const setOutput = (name, value) => {
  console.log(`${name}=${value}`);
  if (process.env.GITHUB_OUTPUT) {
    appendFileSync(process.env.GITHUB_OUTPUT, `${name}=${value}\n`);
  }
};

export const run = (args = process.argv.slice(2), environment = process.env) => {
  const verbose =
    args.includes("--verbose") ||
    environment.VERBOSE === "1" ||
    environment.RUNNER_DEBUG === "1";
  const outputDirectory = args.find((arg) => !arg.startsWith("--"));
  if (!outputDirectory) {
    console.error("Usage: node pack-cpp-nuget.mjs <output-directory> [--verbose]");
    return 2;
  }

  const nuspec = readFileSync(nuspecPath, "utf8");
  const { id, version, releaseNotes } = readPackageInfo(nuspec);
  const headers = listHeaders();
  if (headers.length === 0) {
    console.error(`::error title=C++ NuGet pack failed::No headers in ${headersDirectory}`);
    return 1;
  }

  const staging = mkdtempSync(path.join(tmpdir(), "cpp-nuget-"));
  try {
    const content = path.join(staging, "content");
    for (const directory of ["images", "build/native", "lib/native/include"]) {
      mkdirSync(path.join(content, directory), { recursive: true });
    }
    copyFileSync(path.join(assetsDirectory, "icon.png"), path.join(content, "images/icon.png"));
    copyFileSync(
      path.join(assetsDirectory, "TemplateLibrary.targets"),
      path.join(content, `build/native/${id}.targets`),
    );
    for (const header of headers) {
      copyFileSync(
        path.join(headersDirectory, header),
        path.join(content, "lib/native/include", header),
      );
    }
    writeFileSync(path.join(staging, `${id}.nuspec`), withStagedFiles(nuspec));
    writeFileSync(
      path.join(staging, "pack.csproj"),
      [
        '<Project Sdk="Microsoft.NET.Sdk">',
        "  <PropertyGroup>",
        "    <TargetFramework>netstandard2.0</TargetFramework>",
        "    <IncludeBuildOutput>false</IncludeBuildOutput>",
        "    <NoBuild>true</NoBuild>",
        `    <NuspecFile>${id}.nuspec</NuspecFile>`,
        "    <NuspecBasePath>$(MSBuildProjectDirectory)</NuspecBasePath>",
        "  </PropertyGroup>",
        "</Project>",
        "",
      ].join("\n"),
    );

    const absoluteOutput = path.resolve(outputDirectory);
    mkdirSync(absoluteOutput, { recursive: true });
    const packOutput = execFileSync(
      "dotnet",
      ["pack", path.join(staging, "pack.csproj"), "--output", absoluteOutput, "--nologo"],
      { encoding: "utf8", stdio: ["ignore", "pipe", "inherit"] },
    );
    if (verbose) {
      console.log(packOutput);
    }

    const packagePath = path.join(absoluteOutput, `${id}.${version}.nupkg`);
    const entries = listPackageEntries(packagePath);
    if (verbose) {
      console.log(entries.join("\n"));
    }
    const missing = findMissingEntries(expectedEntries(id, headers), entries);
    const stray = entries.filter((entry) => /(^|\/)(obj|bin)\/|\.csproj$/.test(entry));
    for (const entry of missing) {
      console.error(`::error title=C++ NuGet pack failed::${entry} is missing from ${packagePath}`);
    }
    for (const entry of stray) {
      console.error(`::error title=C++ NuGet pack failed::${entry} must not be packed`);
    }
    if (missing.length > 0 || stray.length > 0) {
      return 1;
    }

    writeFileSync(path.join(absoluteOutput, "release-notes.md"), `${releaseNotes}\n`);
    console.log(`Packed ${id} ${version} with ${headers.length} headers.`);
    setOutput("package_id", id);
    setOutput("version", version);
    setOutput("package_path", packagePath);
    return 0;
  } finally {
    rmSync(staging, { recursive: true, force: true });
  }
};

if (process.argv[1] && import.meta.url === new URL(`file://${process.argv[1]}`).href) {
  process.exitCode = run();
}
