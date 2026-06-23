# Change Log

## 0.4.0
### Installer scripts

- Added explicit distro installers: `install-arch.sh`, `install-fedora.sh`, `install-ubuntu.sh`, `install-debian.sh`, `install-opensuse.sh`, and `install-alpine.sh`.
- Added `install-linux.sh` auto-detect wrapper.
- Documented all distro installers in `README.md` and added a short `READ.md` entry point.

- Added Settings → About with About Qt and About RemiDock actions.
- Added RemiDock version and clickable GitHub link to the About RemiDock dialog.
- Updated application metadata and license notices for Qt rights.
- Added cross-distro dependency/build scripts and a GitHub Actions workflow that builds Linux artifacts on every branch and publishes master artifacts to a GitHub release.

## 0.3.3

- Now RemiDock reads .desktop file Actions=..., where Chrome/Chromium store “New Incognito Window”.

## 0.3.2

- Fixed AUR to run RemiDock immediately.
- Fixed color theme to work with all icon themes.

## 0.3.1

- Updated aur.sh
- Added /build to .gitignore

## 0.2.1

- Fixed aur.sh

## 0.2.0

- Box around icons removed.
- Dock frame made optional.
- AUR package added (for Archlinux).

## 0.1.1

- Added exit button.
- Fixed moving applications in settings window.
- Fixed opening applications from dock makes applications headless.

## 0.1.0

Initial release
