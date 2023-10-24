# Lifecycle & automation
All the CI automation and lifecycle management is done through GitHub Actions. 
There are different rules for different workflows.

## Development and branching
Even though we are not strictly using [git-flow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow#:~:text=Gitflow%20is%20an%20alternative%20Git,lived%20branches%20and%20larger%20commits.), we are following a similar branching strategy.
- All features are developed in feature branches. `git checkout -b feature/my_feature`
- All features are merged into the `develop` branch. `git checkout develop && git merge feature/my_feature`
- All bugfixes are developed in bugfix branches. `git checkout -b bugfix/my_bugfix`

### Forking and Pull requests
- Promote forking of the repository for better collaboration. This is a good practice for open source projects. This will allow you also to have control over rights, permissions and actions environments.
- This also allows you to have a separate repository for a contributors own development and testing. Contributers can then make a PR to the main repository when you are ready.
- Contributing from forks can only be made through Pull requests, this makes the overall codebase management more transparent.
- Furthermore there are specific automations like tests that are triggered on pull requests. This is a good way to make sure that the code is tested before merging, and educates contributors in writing tests.

## What is automated?
- Tests on every pull request to develop and main branches for all platforms (Linux, Windows, MacOS). Worfklow: [`pip.yaml`](../.github/workflows/pip.yml).
- Automatic build and distribution to pypi on every release. Workflow: [`pypi.yaml`](../.github/workflows/wheels.yml).
- Automatic documentation generation. Workflow: [`docs.yaml`](../.github/workflows/docs.yaml).

## Releasing
- Update the version number in `pyproject.toml`. Do the same for the citation file. `CITAION.cff``.
- Use the release tag in GitHub to create a new release. This will trigger the automatic build and distribution to pypi. This manual step will trigger the workflow: [`pypi.yaml`](../.github/workflows/wheels.yaml).
