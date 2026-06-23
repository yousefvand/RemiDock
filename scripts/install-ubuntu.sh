#!/usr/bin/env bash
set -Eeuo pipefail
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/_install-common.sh"

require_package_manager apt-get

if [[ "$SKIP_DEPS" != "1" ]]; then
    log "Installing Ubuntu dependencies"
    yes_flags=()
    [[ "$ASSUME_YES" == "1" ]] && yes_flags=(-y)
    as_root apt-get update
    as_root apt-get install "${yes_flags[@]}" \
        build-essential cmake ninja-build g++ git pkg-config \
        extra-cmake-modules \
        qt6-base-dev qt6-base-dev-tools \
        qt6-declarative-dev qt6-declarative-dev-tools \
        qt6-svg-dev qt6-tools-dev qt6-tools-dev-tools \
        qt6-image-formats-plugins \
        libpulse-dev liblayershellqtinterface-dev
else
    log "Skipping dependency install because SKIP_DEPS=1"
fi

configure_build_install
