#!/usr/bin/env bash
# Shared installer helpers for RemiDock distro-specific install scripts.
# This file is sourced by install-*.sh scripts; it is not meant to be run directly.

set -Eeuo pipefail

PROJECT_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"
INSTALL_PREFIX="${INSTALL_PREFIX:-/usr}"
BUILD_TYPE="${BUILD_TYPE:-Release}"
GENERATOR="${GENERATOR:-Ninja}"
ASSUME_YES="${ASSUME_YES:-1}"
SKIP_DEPS="${SKIP_DEPS:-0}"
SKIP_BUILD="${SKIP_BUILD:-0}"
SKIP_INSTALL="${SKIP_INSTALL:-0}"
WITH_SUDO="${WITH_SUDO:-auto}"

log() {
    printf '\n\033[1;34m==>\033[0m %s\n' "$*"
}

warn() {
    printf '\n\033[1;33mWARNING:\033[0m %s\n' "$*" >&2
}

die() {
    printf '\n\033[1;31mERROR:\033[0m %s\n' "$*" >&2
    exit 1
}

have() {
    command -v "$1" >/dev/null 2>&1
}

need_cmd() {
    have "$1" || die "Required command not found: $1"
}

require_package_manager() {
    local pm="$1"
    have "$pm" || die "This installer needs '$pm'. Use the installer script for your distribution."
}

sudo_cmd=()
if [[ "$WITH_SUDO" == "1" ]] || { [[ "$WITH_SUDO" == "auto" ]] && [[ "${EUID:-$(id -u)}" -ne 0 ]]; }; then
    have sudo || die "sudo is required. Install sudo, run as root, or set WITH_SUDO=0 when already root."
    sudo_cmd=(sudo)
fi

as_root() {
    "${sudo_cmd[@]}" "$@"
}

maybe_yes_apt() {
    [[ "$ASSUME_YES" == "1" ]] && printf '%s\n' "-y"
}

maybe_yes_dnf() {
    [[ "$ASSUME_YES" == "1" ]] && printf '%s\n' "-y"
}

maybe_yes_zypper() {
    [[ "$ASSUME_YES" == "1" ]] && printf '%s\n' "--non-interactive"
}

maybe_yes_pacman() {
    [[ "$ASSUME_YES" == "1" ]] && printf '%s\n' "--noconfirm"
}

configure_build_install() {
    if [[ "$SKIP_BUILD" == "1" ]]; then
        log "Skipping build because SKIP_BUILD=1"
        return
    fi

    need_cmd cmake
    if [[ "$GENERATOR" == "Ninja" ]]; then
        need_cmd ninja
    fi

    log "Configuring RemiDock"
    cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" -G "$GENERATOR" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"

    log "Building RemiDock"
    cmake --build "$BUILD_DIR" --parallel

    if [[ "$SKIP_INSTALL" == "1" ]]; then
        log "Skipping system install because SKIP_INSTALL=1"
        log "Run locally with: $BUILD_DIR/bin/RemiDock"
        return
    fi

    log "Installing RemiDock to $INSTALL_PREFIX"
    as_root cmake --install "$BUILD_DIR"

    log "Installation complete"
    printf 'Run RemiDock from your app launcher, or run: %s\n' "RemiDock"
}
