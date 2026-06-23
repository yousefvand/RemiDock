#!/bin/sh
set -eu

# Alpine's minimal container image does not include bash.  This script must
# start with /bin/sh so GitHub Actions can run it before dependencies exist.
# It installs bash, then the normal bash-based build scripts can run.

PROJECT_ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"
INSTALL_PREFIX="${INSTALL_PREFIX:-/usr}"
BUILD_TYPE="${BUILD_TYPE:-Release}"
GENERATOR="${GENERATOR:-Ninja}"
SKIP_DEPS="${SKIP_DEPS:-0}"
SKIP_BUILD="${SKIP_BUILD:-0}"
SKIP_INSTALL="${SKIP_INSTALL:-0}"
WITH_SUDO="${WITH_SUDO:-auto}"

log() {
    printf '\n\033[1;34m==>\033[0m %s\n' "$*"
}

die() {
    printf '\n\033[1;31mERROR:\033[0m %s\n' "$*" >&2
    exit 1
}

have() {
    command -v "$1" >/dev/null 2>&1
}

[ -n "$(command -v apk 2>/dev/null || true)" ] || die "This installer needs apk. Use the installer script for your distribution."

sudo_prefix=""
if [ "$WITH_SUDO" = "1" ] || { [ "$WITH_SUDO" = "auto" ] && [ "$(id -u)" -ne 0 ]; }; then
    have sudo || die "sudo is required. Install sudo, run as root, or set WITH_SUDO=0 when already root."
    sudo_prefix="sudo"
fi

as_root() {
    if [ -n "$sudo_prefix" ]; then
        sudo "$@"
    else
        "$@"
    fi
}

if [ "$SKIP_DEPS" != "1" ]; then
    log "Installing Alpine Linux dependencies"
    as_root apk add --update-cache \
        bash \
        build-base linux-headers \
        cmake extra-cmake-modules \
        ninja-build ninja-is-really-ninja \
        g++ git pkgconf \
        qt6-qtbase-dev qt6-qtdeclarative-dev qt6-qtsvg-dev \
        qt6-qttools-dev qt6-qtimageformats qt6-qtwayland-dev \
        libxkbcommon-dev pulseaudio-dev layer-shell-qt-dev
else
    log "Skipping dependency install because SKIP_DEPS=1"
fi

if [ "$SKIP_BUILD" = "1" ]; then
    log "Skipping build because SKIP_BUILD=1"
    exit 0
fi

have cmake || die "cmake not found after dependency installation."
have ninja || die "ninja not found after dependency installation."

log "Configuring RemiDock"
cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" -G "$GENERATOR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"

log "Building RemiDock"
cmake --build "$BUILD_DIR" --parallel

if [ "$SKIP_INSTALL" = "1" ]; then
    log "Skipping system install because SKIP_INSTALL=1"
    printf 'Run locally with: %s\n' "$BUILD_DIR/bin/RemiDock"
    exit 0
fi

log "Installing RemiDock to $INSTALL_PREFIX"
as_root cmake --install "$BUILD_DIR"

log "Installation complete"
printf 'Run RemiDock from your app launcher, or run: %s\n' "RemiDock"
