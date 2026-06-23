#!/usr/bin/env bash
set -Eeuo pipefail
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/_install-common.sh"

require_package_manager zypper

if [[ "$SKIP_DEPS" != "1" ]]; then
    log "Installing openSUSE dependencies"
    zypper_flags=()
    [[ "$ASSUME_YES" == "1" ]] && zypper_flags=(--non-interactive --gpg-auto-import-keys)

    # zypper global options must be placed before the command.
    # Correct: zypper --non-interactive install package
    # Wrong:   zypper install --non-interactive package
    as_root zypper "${zypper_flags[@]}" refresh
    as_root zypper "${zypper_flags[@]}" install \
        patterns-devel-base-devel_basis \
        cmake ninja gcc-c++ git pkgconf-pkg-config \
        extra-cmake-modules \
        qt6-base-devel qt6-declarative-devel qt6-svg-devel \
        qt6-tools qt6-imageformats \
        libxkbcommon-devel libpulse-devel layer-shell-qt6-devel
else
    log "Skipping dependency install because SKIP_DEPS=1"
fi

configure_build_install
