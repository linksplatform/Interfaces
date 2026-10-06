## Summary

`cpp-test.yml` (at `06a7067`) configures and builds the tests (`cmake --build .`) but never runs them. A failing assertion therefore still produces a green "Test cpp" run. The callers make it worse: linksplatform/Interfaces, for example, triggers it with `pull_request: types: [edited]`, so commits pushed to a PR do not start it at all. Only edits to the PR title or body do.

## Evidence

- linksplatform/Interfaces run [36213155523](https://github.com/linksplatform/Interfaces/actions/runs/36213155523) logs `[100%] Built target Platform.Interfaces.Tests`, then the job ends. No `ctest` command runs and the test binary is never executed.
- linksplatform/Interfaces#151 includes an [experiment](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/experiments/cpp-false-positive-test-run.sh) that adds `ASSERT_TRUE(false)`. Repeating the reusable workflow's steps still exits 0. Running `ctest` exits non-zero. The log is in `dev/log/issues/150/pulls/151/cpp-false-positive-experiment.log`.

## Minimal reproduction

1. In any repo that calls `linksplatform/Workflows/.github/workflows/cpp-test.yml@main`, add a failing gtest assertion.
2. Push it to a PR. The workflow does not start, because only `edited` is subscribed.
3. Edit the PR title. The workflow runs and passes.

## Workaround

linksplatform/Interfaces#151 replaced the call with a local workflow. The local workflow runs [`cpp/build-and-test.sh`](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/cpp/build-and-test.sh), which does the following:

- `conan install`
- `cmake --preset`
- `cmake --build`
- `ctest --output-on-failure --no-tests=error`

It also uses the default `pull_request` activity types.

## Suggested fix

- Add `ctest --test-dir <build> --output-on-failure --no-tests=error` after the build. `--no-tests=error` turns "no tests registered" into a failure as well. Projects must call `enable_testing()` and `gtest_discover_tests()`.
- Remove `types: [edited]` from the callers' templates.
