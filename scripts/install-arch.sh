#!/usr/bin/env bash
set -Eeuo pipefail
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/_install-common.sh"

require_package_manager pacman

if [[ "$SKIP_DEPS" != "1" ]]; then
    log "Installing Arch Linux / Manjaro / EndeavourOS dependencies"
    yes_flags=()
    [[ "$ASSUME_YES" == "1" ]] && yes_flags=(--noconfirm)
    as_root pacman -Syu --needed "${yes_flags[@]}" \
        base-devel cmake ninja gcc git extra-cmake-modules \
        qt6-base qt6-declarative qt6-svg qt6-tools qt6-imageformats \
        layer-shell-qt libpulse
else
    log "Skipping dependency install because SKIP_DEPS=1"
fi

configure_build_install
