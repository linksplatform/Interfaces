## Summary

`hint: Using 'master' as the name for the initial branch` is still printed by `actions/checkout` in `docs.yml`, `security.yml`, `workflows.yml` on `main` (1e2f475). The `GIT_CONFIG_COUNT` / `GIT_CONFIG_KEY_0: init.defaultBranch` / `GIT_CONFIG_VALUE_0: main` env block from #28 is only in `release.yml` and `links.yml`.

Occurrences in the latest `main` run of each workflow: workflows.yml [37294585398](https://github.com/link-foundation/python-ai-driven-development-pipeline-template/actions/runs/37294585398) (3), docs.yml [37294585301](https://github.com/link-foundation/python-ai-driven-development-pipeline-template/actions/runs/37294585301) (2), security.yml [37314932499](https://github.com/link-foundation/python-ai-driven-development-pipeline-template/actions/runs/37314932499) (4).

## Reproduction

```bash
repo=link-foundation/python-ai-driven-development-pipeline-template
for f in .github/workflows/*.yml; do grep -q GIT_CONFIG_COUNT "$f" || echo "no GIT_CONFIG: $f"; done
gh run view <run id> --repo $repo --log | grep "Using 'master'"
```

## Workaround

None is needed; it is only noise. It does make real warnings harder to spot in the logs.

## Suggested fix

Add the same top-level env block to the three workflows:

```yaml
env:
  GIT_CONFIG_COUNT: '1'
  GIT_CONFIG_KEY_0: init.defaultBranch
  GIT_CONFIG_VALUE_0: main
```

Then add a workflow policy test that requires it in every workflow with an `actions/checkout` step, so a new workflow can't miss it again. linksplatform/Interfaces does this in `.github/scripts/workflow-policy.test.mjs` ([PR #151](https://github.com/linksplatform/Interfaces/pull/151)). The C++ template has the same gap in `workflows.yml` (link-foundation/cpp-ai-driven-development-pipeline-template#5).

Evidence: [templates-git-master-hint.txt](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/dev/log/issues/150/pulls/151/upstream/templates-git-master-hint.txt).
