# Repository Mirroring Setup Guide

This document explains how to set up automatic mirroring to GitLab and Bitbucket for the Interfaces repository.

## Overview

The mirroring system automatically:
- Mirrors all branch and tag pushes to GitLab and Bitbucket
- Safely handles branch deletions (only if fully merged)
- Provides backup and multi-platform accessibility
- Maintains complete repository history

## Prerequisites

### 1. GitLab Setup
1. Create a project on GitLab at `gitlab.com/linksplatform/interfaces`
2. Generate a Personal Access Token:
   - Go to GitLab → User Settings → Access Tokens
   - Create token with `api`, `read_repository`, and `write_repository` scopes
   - Save the token securely

### 2. Bitbucket Setup
1. Create a repository on Bitbucket at `bitbucket.org/linksplatform/interfaces`
2. Generate an App Password:
   - Go to Bitbucket → Personal settings → App passwords
   - Create password with `Repositories: Write` permission
   - Save the password securely

## Required GitHub Secrets

Add the following secrets to your GitHub repository settings:

### GitLab Secrets
- `GITLAB_TOKEN`: Your GitLab Personal Access Token
- `GITLAB_PROJECT_PATH`: `linksplatform/interfaces` (or your preferred path)

### Bitbucket Secrets
- `BITBUCKET_USERNAME`: Your Bitbucket username
- `BITBUCKET_APP_PASSWORD`: Your Bitbucket App Password
- `BITBUCKET_PROJECT_PATH`: `linksplatform/interfaces` (or your preferred path)

## How to Add Secrets

1. Go to your GitHub repository
2. Navigate to Settings → Secrets and variables → Actions
3. Click "New repository secret"
4. Add each secret with the exact names listed above

## Workflow Features

### Automatic Mirroring
- **Push Events**: All branch and tag pushes are automatically mirrored
- **Full History**: Complete repository history is maintained
- **Error Handling**: Graceful fallbacks if mirror operations fail

### Safe Branch Deletion
- **Merge Check**: Only deletes branches from mirrors if they were fully merged
- **Safety First**: Skips deletion if merge status cannot be determined
- **Main Branch Protection**: Checks against main/master branch for merge status

### Tag Synchronization
- **All Tags**: Mirrors all repository tags to both platforms
- **Release Sync**: Keeps release tags in sync across all platforms

## Monitoring

### Workflow Status
- Check GitHub Actions tab for mirror workflow status
- Each platform (GitLab/Bitbucket) has separate jobs for better isolation
- Failed mirrors don't block other operations

### Troubleshooting

#### Common Issues
1. **Authentication Failures**
   - Verify tokens/passwords are correct and not expired
   - Check that secrets are properly named in GitHub

2. **Repository Not Found**
   - Ensure target repositories exist on GitLab/Bitbucket
   - Verify project paths match the configured secrets

3. **Permission Errors**
   - Confirm GitLab token has required scopes
   - Verify Bitbucket app password has write permissions

#### Logs
Check the GitHub Actions logs for detailed error messages:
1. Go to Actions tab in your repository
2. Click on the failed workflow run
3. Expand the failed job to see error details

## Manual Sync (Emergency)

If automatic mirroring fails, you can manually sync:

```bash
# Clone the repository
git clone https://github.com/linksplatform/Interfaces.git
cd Interfaces

# Add remotes
git remote add gitlab https://oauth2:TOKEN@gitlab.com/linksplatform/interfaces.git
git remote add bitbucket https://USERNAME:PASSWORD@bitbucket.org/linksplatform/interfaces.git

# Push everything
git push --mirror gitlab
git push --mirror bitbucket
```

## Security Considerations

- Tokens and passwords are stored as encrypted GitHub secrets
- Workflow runs only have access to secrets during execution
- All authentication uses secure HTTPS connections
- Branch deletion safety checks prevent accidental data loss

## Testing

After setup, test the mirroring by:
1. Creating a test branch and pushing it
2. Verifying it appears on both GitLab and Bitbucket
3. Merging the branch and deleting it
4. Confirming it's removed from mirrors

## Support

For issues with this mirroring setup:
1. Check the workflow logs in GitHub Actions
2. Verify all secrets are correctly configured
3. Ensure target repositories exist and are accessible
4. Review this documentation for troubleshooting steps