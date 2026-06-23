#!/usr/bin/env bash
set -Eeuo pipefail
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/_install-common.sh"

require_package_manager dnf

if [[ "$SKIP_DEPS" != "1" ]]; then
    log "Installing Fedora dependencies"
    yes_flags=()
    [[ "$ASSUME_YES" == "1" ]] && yes_flags=(-y)
    as_root dnf install "${yes_flags[@]}" \
        @development-tools \
        cmake ninja-build gcc-c++ git pkgconf-pkg-config \
        extra-cmake-modules \
        qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qtsvg-devel \
        qt6-qttools-devel qt6-qtimageformats \
        libxkbcommon-devel pulseaudio-libs-devel layer-shell-qt-devel
else
    log "Skipping dependency install because SKIP_DEPS=1"
fi

configure_build_install
