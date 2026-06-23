#!/usr/bin/env bash
set -Eeuo pipefail
source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/_install-common.sh"

require_package_manager apk

if [[ "$SKIP_DEPS" != "1" ]]; then
    log "Installing Alpine Linux dependencies"
    as_root apk add --update \
        build-base cmake ninja g++ git pkgconf extra-cmake-modules \
        qt6-qtbase-dev qt6-qtdeclarative-dev qt6-qtsvg-dev \
        qt6-qttools-dev qt6-qtimageformats \
        pulseaudio-dev layer-shell-qt-dev
else
    log "Skipping dependency install because SKIP_DEPS=1"
fi

configure_build_install
