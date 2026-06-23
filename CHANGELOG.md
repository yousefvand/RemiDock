# Change Log


## 0.4.4

- Fixed CI

## 0.4.3

- Made scripts executable

## 0.4.2

- Fixed CI

## 0.4.1

- Fixed CI

## 0.4.0

- Support major Linux distros

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

### Fixed

- Fixed Ubuntu 24.04 / Qt 6.4 build failure by using `QQmlApplicationEngine::loadFromModule()` only on Qt 6.5+ and falling back to the embedded QML resource URL on Qt 6.4.
- Lowered the declared minimum Qt requirement from 6.5 to 6.4 for stable distro packages.
- Added `libxkbcommon` development packages to distro installers to satisfy Qt GUI/XKB checks.

### Runtime loader fix
- Removed `QQmlApplicationEngine::loadFromModule("RemiDock", "Main")` because this project stores `Main.qml` under `qml/Main.qml`, which can produce `Module "RemiDock" contains no type named "Main"` on newer Qt builds.
- Added explicit `RESOURCE_PREFIX /qt/qml` and now always loads `qrc:/qt/qml/RemiDock/qml/Main.qml`.

### Fixes

- Reduced the About RemiDock dialog height from the oversized 330 px layout to 280 px.
- Restored About Qt to Qt's standard in-process dialog instead of launching a helper process, preventing full-screen behavior and stale dialog state.
