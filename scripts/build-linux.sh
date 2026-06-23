#!/usr/bin/env bash
set -Eeuo pipefail

# Configure, build, install into a staging directory, and create a portable
# distro artifact tarball containing the install tree.

PROJECT_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"
STAGE_DIR="${STAGE_DIR:-$PROJECT_ROOT/stage}"
ARTIFACT_DIR="${ARTIFACT_DIR:-$PROJECT_ROOT/artifacts}"
BUILD_TYPE="${BUILD_TYPE:-Release}"
INSTALL_PREFIX="${INSTALL_PREFIX:-/usr}"
GENERATOR="${GENERATOR:-Ninja}"

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

detect_distro() {
    if [[ -n "${DISTRO_NAME:-}" ]]; then
        printf '%s' "$DISTRO_NAME"
        return
    fi

    if [[ -r /etc/os-release ]]; then
        # shellcheck disable=SC1091
        . /etc/os-release
        printf '%s' "${ID:-linux}-${VERSION_ID:-rolling}"
    else
        printf 'linux'
    fi
}

detect_arch() {
    uname -m | tr '[:upper:]' '[:lower:]'
}

project_version() {
    sed -nE 's/^project\(RemiDock VERSION ([^ ]+) .*/\1/p' "$PROJECT_ROOT/CMakeLists.txt" | head -n 1
}

have cmake || die "cmake not found. Run scripts/install-deps.sh first."
have ninja || die "ninja not found. Run scripts/install-deps.sh first."

VERSION="${VERSION:-$(project_version)}"
[[ -n "$VERSION" ]] || die "Could not determine project version from CMakeLists.txt."

log "Configuring RemiDock $VERSION"
cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" -G "$GENERATOR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"

log "Building"
cmake --build "$BUILD_DIR" --parallel

log "Installing into staging directory"
rm -rf -- "$STAGE_DIR"
DESTDIR="$STAGE_DIR" cmake --install "$BUILD_DIR"

if [[ -f "$PROJECT_ROOT/icon.png" ]]; then
    install -Dm644 "$PROJECT_ROOT/icon.png" "$STAGE_DIR$INSTALL_PREFIX/share/pixmaps/remidock.png"
    if [[ -f "$STAGE_DIR$INSTALL_PREFIX/share/applications/org.remisa.RemiDock.desktop" ]]; then
        sed -i 's/^Icon=.*/Icon=remidock/' "$STAGE_DIR$INSTALL_PREFIX/share/applications/org.remisa.RemiDock.desktop"
    fi
fi

install -Dm644 "$PROJECT_ROOT/LICENSE" "$STAGE_DIR$INSTALL_PREFIX/share/licenses/remidock/LICENSE"
install -Dm644 "$PROJECT_ROOT/README.md" "$STAGE_DIR$INSTALL_PREFIX/share/doc/remidock/README.md"

DISTRO="$(detect_distro | tr '/ :' '---')"
ARCH="$(detect_arch)"
mkdir -p "$ARTIFACT_DIR"
ARTIFACT="$ARTIFACT_DIR/remidock-$VERSION-$DISTRO-$ARCH.tar.gz"

log "Creating artifact: $ARTIFACT"
tar -C "$STAGE_DIR" -czf "$ARTIFACT" .

log "Artifact ready: $ARTIFACT"
