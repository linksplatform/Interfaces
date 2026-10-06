## Summary

`hint: Using 'master' as the name for the initial branch` is still printed by `actions/checkout` in `docs.yml`, `security.yml`, `workflows.yml` on `main` (22e53c8). The `GIT_CONFIG_COUNT` / `GIT_CONFIG_KEY_0: init.defaultBranch` / `GIT_CONFIG_VALUE_0: main` env block from #35 is only in `release.yml` and `links.yml`.

Occurrences in the latest `main` run of each workflow: workflows.yml [35580680054](https://github.com/link-foundation/csharp-ai-driven-development-pipeline-template/actions/runs/35580680054) (2), docs.yml [35580680196](https://github.com/link-foundation/csharp-ai-driven-development-pipeline-template/actions/runs/35580680196) (1), security.yml [34455449315](https://github.com/link-foundation/csharp-ai-driven-development-pipeline-template/actions/runs/34455449315) (2).

## Reproduction

```bash
repo=link-foundation/csharp-ai-driven-development-pipeline-template
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

Then add a workflow policy test that requires it in every workflow with an `actions/checkout` step, so a new workflow can't miss it again. linksplatform/Interfaces does this in `.github/scripts/workflow-policy.test.mjs` ([PR #151](https://github.com/linksplatform/Interfaces/pull/151)). The C++ template has the same gap in `workflows.yml` (reported there).

Evidence: [templates-git-master-hint.txt](https://github.com/linksplatform/Interfaces/blob/issue-150-dfc4758d0022/dev/log/issues/150/pulls/151/upstream/templates-git-master-hint.txt).
