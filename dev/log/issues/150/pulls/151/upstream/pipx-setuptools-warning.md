**Issue**

A fresh `pipx install` prints `WARNING: Skipping setuptools as it is not installed.` on Python 3.12+. Nothing is wrong, but the line goes to stderr, so CI logs show a warning on every run that starts with an empty `PIPX_HOME`. GitHub-hosted macOS runners are one example.

Expected: no warning when setuptools is already absent.

The cause is in `SharedLibs._create` (`src/pipx/shared_libs.py`, lines 147-152 on `main` at 4615fbc):

```python
# Remove setuptools before the .pth file exposes shared libraries to apps. Python <3.12 venvs bundle a
# copy that fails under 3.12+ because the standard library no longer includes distutils.
run_subprocess(
    [self.python_path, "-m", "pip", "--no-input", "uninstall", "-y", "setuptools"],
    capture_stderr=False,
)
```

Since Python 3.12, `python -m venv` no longer installs setuptools, so on 3.12+ this uninstall always targets a missing package. pip then prints the warning, and `capture_stderr=False` sends it to the user's stderr.

**Environment**

- OS: Ubuntu 24.04 (Linux 6.8). Also seen on GitHub's `macos-15` runner image.
- Shell: bash 5.2.21
- Python version and path: 3.14.7, `/tmp/pipx-repro-venv/bin/python`
- `pipx --version` output: `1.17.11`

**Output of the failing command**

```console
$ PIPX_HOME=$(mktemp -d) PIPX_BIN_DIR=$(mktemp -d) pipx install --verbose pycowsay==0.0.0.2
...
pipx >(run_subprocess:217): running /tmp/tmp.fFQhqAhaBm/shared/bin/python -m pip --no-input --disable-pip-version-check install --upgrade --force-reinstall pip >= 26.1
pipx >(run_subprocess:217): running /tmp/tmp.fFQhqAhaBm/shared/bin/python -m pip --no-input uninstall -y setuptools
WARNING: Skipping setuptools as it is not installed.
...
```

The GitHub Actions log of the `macos-15` runner shows the same thing: `creating shared libraries...` is followed by `WARNING: Skipping setuptools as it is not installed.` ([job log](https://github.com/linksplatform/Interfaces/actions/runs/37526407726/job/112484415682#step:3:1)).

**Workaround**

Filter out exactly that line:

```bash
pipx install conan==2.33.0 \
  2> >(grep -vxF 'WARNING: Skipping setuptools as it is not installed.' >&2)
```

**Suggested fix**

Do either of these:

- Run the uninstall only when setuptools is present, for example after checking `importlib.util.find_spec("setuptools")` with the shared venv's Python.
- Capture its stderr (`capture_stderr=True`) and log it at debug level. A real failure would still be reported by checking the return code.

Found while cleaning the CI warnings of linksplatform/Interfaces ([PR #151](https://github.com/linksplatform/Interfaces/pull/151)).
