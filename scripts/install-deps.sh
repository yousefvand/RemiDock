#!/usr/bin/env bash
set -Eeuo pipefail

# Install RemiDock build/runtime dependencies on common Linux distributions.
# Supported package managers: pacman, apt, dnf, zypper, apk.
# Set ASSUME_YES=0 to disable non-interactive package-manager flags.

ASSUME_YES="${ASSUME_YES:-1}"
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

sudo_cmd=()
if [[ "$WITH_SUDO" == "1" ]] || { [[ "$WITH_SUDO" == "auto" ]] && [[ "${EUID:-$(id -u)}" -ne 0 ]]; }; then
    have sudo || die "sudo is required. Re-run as root or install sudo."
    sudo_cmd=(sudo)
fi

run_pm() {
    "${sudo_cmd[@]}" "$@"
}

install_pacman() {
    local yes=()
    [[ "$ASSUME_YES" == "1" ]] && yes=(--noconfirm)
    log "Installing dependencies with pacman"
    run_pm pacman -Syu --needed "${yes[@]}" \
        base-devel cmake ninja gcc git extra-cmake-modules \
        qt6-base qt6-declarative qt6-svg qt6-tools qt6-imageformats \
        layer-shell-qt libpulse
}

install_apt() {
    local yes=()
    [[ "$ASSUME_YES" == "1" ]] && yes=(-y)
    log "Installing dependencies with apt"
    run_pm apt-get update
    run_pm apt-get install "${yes[@]}" \
        build-essential cmake ninja-build g++ git pkg-config \
        extra-cmake-modules \
        qt6-base-dev qt6-base-dev-tools qt6-declarative-dev qt6-declarative-dev-tools \
        qt6-svg-dev qt6-tools-dev qt6-tools-dev-tools qt6-image-formats-plugins \
        libpulse-dev liblayershellqtinterface-dev
}

install_dnf() {
    local yes=()
    [[ "$ASSUME_YES" == "1" ]] && yes=(-y)
    log "Installing dependencies with dnf"
    run_pm dnf install "${yes[@]}" \
        @development-tools cmake ninja-build gcc-c++ git pkgconf-pkg-config \
        extra-cmake-modules \
        qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qtsvg-devel qt6-qttools-devel qt6-qtimageformats \
        pulseaudio-libs-devel layer-shell-qt-devel
}

install_zypper() {
    local yes=()
    [[ "$ASSUME_YES" == "1" ]] && yes=(--non-interactive)
    log "Installing dependencies with zypper"
    run_pm zypper refresh
    run_pm zypper install "${yes[@]}" \
        patterns-devel-base-devel_basis cmake ninja gcc-c++ git pkgconf-pkg-config \
        extra-cmake-modules \
        qt6-base-devel qt6-declarative-devel qt6-svg-devel qt6-tools qt6-imageformats \
        libpulse-devel layer-shell-qt-devel
}

install_apk() {
    log "Installing dependencies with apk"
    run_pm apk add --update \
        build-base cmake ninja g++ git pkgconf extra-cmake-modules \
        qt6-qtbase-dev qt6-qtdeclarative-dev qt6-qtsvg-dev qt6-qttools-dev qt6-qtimageformats \
        pulseaudio-dev layer-shell-qt-dev
}

if have pacman; then
    install_pacman
elif have apt-get; then
    install_apt
elif have dnf; then
    install_dnf
elif have zypper; then
    install_zypper
elif have apk; then
    install_apk
else
    die "Unsupported distribution/package manager. Install Qt 6, QML/Quick Controls 2, Qt SVG, LayerShellQt, ECM, CMake, Ninja, a C++20 compiler, and PulseAudio development files manually."
fi

log "Dependencies installed. Build with: scripts/build-linux.sh"
