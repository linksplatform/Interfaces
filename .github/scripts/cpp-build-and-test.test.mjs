#!/usr/bin/env node

// Runs cpp/build-and-test.sh against a stub conan that prints what Conan
// 2.33's `conan profile detect` prints on stderr, and stops at `conan install`.

import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import { chmodSync, mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import test from "node:test";
import { fileURLToPath } from "node:url";

const script = fileURLToPath(new URL("../../cpp/build-and-test.sh", import.meta.url));

// Copied from the Test cpp jobs of run 37531301224.
const advice = [
  "WARN: This profile is a guess of your environment, please check it.",
  "WARN: Defaulted to cppstd='gnu17' for apple-clang.",
  "WARN: The output of this command is not guaranteed to be stable and can change in future Conan versions.",
  "WARN: Use your own profile files for stability.",
];
const unrelated = "WARN: Unable to detect the compiler version";

const runWithStubConan = () => {
  const bin = mkdtempSync(join(tmpdir(), "conan-stub-"));
  try {
    writeFileSync(
      join(bin, "conan"),
      [
        "#!/usr/bin/env bash",
        'case "$1 $2" in',
        '  "profile path") exit 1 ;;',
        `  "profile detect") echo "[settings]"; printf '%s\\n' ${[...advice, unrelated].map((line) => JSON.stringify(line)).join(" ")} >&2 ;;`,
        "  *) exit 3 ;;",
        "esac",
        "",
      ].join("\n"),
    );
    chmodSync(join(bin, "conan"), 0o755);
    return spawnSync("bash", [script], {
      encoding: "utf8",
      env: { ...process.env, PATH: `${bin}:${process.env.PATH}`, VERBOSE: "", RUNNER_DEBUG: "" },
    });
  } finally {
    rmSync(bin, { recursive: true, force: true });
  }
};

test("drops only the advisory warnings of conan profile detect", () => {
  const result = runWithStubConan();
  assert.equal(result.status, 3, "the script must stop at the stubbed conan install");
  assert.match(result.stdout, /\[settings\]/);
  for (const line of advice) {
    assert.ok(!result.stderr.includes(line), `still printed: ${line}`);
  }
  assert.ok(result.stderr.includes(unrelated), "an unrelated warning was dropped");
});
