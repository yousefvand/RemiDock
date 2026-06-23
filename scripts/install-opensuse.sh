#!/usr/bin/env bash
set -Eeuo pipefail
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/_install-common.sh"

require_package_manager zypper

if [[ "$SKIP_DEPS" != "1" ]]; then
    log "Installing openSUSE dependencies"
    yes_flags=()
    [[ "$ASSUME_YES" == "1" ]] && yes_flags=(--non-interactive)
    as_root zypper refresh
    as_root zypper install "${yes_flags[@]}" \
        patterns-devel-base-devel_basis \
        cmake ninja gcc-c++ git pkgconf-pkg-config \
        extra-cmake-modules \
        qt6-base-devel qt6-declarative-devel qt6-svg-devel \
        qt6-tools qt6-imageformats \
        libpulse-devel layer-shell-qt-devel
else
    log "Skipping dependency install because SKIP_DEPS=1"
fi

configure_build_install
