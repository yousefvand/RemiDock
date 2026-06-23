#!/usr/bin/env bash
set -Eeuo pipefail

# aur.sh - Build, package-install, autostart, run, and publish RemiDock to the AUR.
#
# Default values are set for:
#   pkgname:  remidock
#   version:  0.4.0
#   source:   https://github.com/yousefvand/RemiDock.git
#   binary:   RemiDock
#
# Usage:
#   ./aur.sh
#
# This script does not use sudo cmake --install. It installs the generated
# package via makepkg/pacman so pacman owns the installed files.
#
# Useful environment overrides:
#   VERSION=0.3.1 ./aur.sh
#   TAG=v0.3.1 ./aur.sh
#   PKGREL=2 ./aur.sh
#   AUR_WORKDIR="$PWD/.aur" ./aur.sh
#   SKIP_PACKAGE_INSTALL=1 ./aur.sh
#   SKIP_RUN=1 ./aur.sh
#   SKIP_AUR_PUBLISH=1 ./aur.sh
#   NONINTERACTIVE=1 ./aur.sh

PKGNAME="${PKGNAME:-remidock}"
VERSION="${VERSION:-0.4.10}"
GITHUB_REPO="${GITHUB_REPO:-https://github.com/yousefvand/RemiDock.git}"
BINARY_NAME="${BINARY_NAME:-RemiDock}"
APP_ID="${APP_ID:-org.remisa.RemiDock}"
LICENSE_NAME="${LICENSE_NAME:-MIT}"
# By default, keep AUR packaging work out of the user home directory.
# Set AUR_WORKDIR=/some/path explicitly if you want to keep the generated AUR repo.
USER_SUPPLIED_AUR_WORKDIR="${AUR_WORKDIR:-}"
if [[ -n "$USER_SUPPLIED_AUR_WORKDIR" ]]; then
    AUR_WORKDIR="$USER_SUPPLIED_AUR_WORKDIR"
    CLEAN_AUR_WORKDIR=0
else
    AUR_WORKDIR="$(mktemp -d --tmpdir remidock-aur.XXXXXXXXXX)"
    CLEAN_AUR_WORKDIR=1
fi

BUILD_DIR="${BUILD_DIR:-build}"
NONINTERACTIVE="${NONINTERACTIVE:-0}"
SKIP_PACKAGE_INSTALL="${SKIP_PACKAGE_INSTALL:-0}"
SKIP_RUN="${SKIP_RUN:-0}"
SKIP_AUR_PUBLISH="${SKIP_AUR_PUBLISH:-0}"
CREATE_TAG_IF_MISSING="${CREATE_TAG_IF_MISSING:-0}"
PKGREL="${PKGREL:-1}"

PROJECT_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
AUR_REPO_DIR="$AUR_WORKDIR/$PKGNAME"
DESKTOP_SOURCE="$PROJECT_ROOT/data/$APP_ID.desktop"
ICON_SOURCE="$PROJECT_ROOT/icon.png"

cleanup() {
    if [[ "${CLEAN_AUR_WORKDIR:-0}" == "1" && -n "${AUR_WORKDIR:-}" && -d "$AUR_WORKDIR" ]]; then
        rm -rf -- "$AUR_WORKDIR"
    fi
}
trap cleanup EXIT

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

confirm() {
    local prompt="$1"
    if [[ "$NONINTERACTIVE" == "1" ]]; then
        return 0
    fi

    read -r -p "$prompt [y/N] " reply
    [[ "$reply" == "y" || "$reply" == "Y" || "$reply" == "yes" || "$reply" == "YES" ]]
}

require_command() {
    command -v "$1" >/dev/null 2>&1 || die "Required command not found: $1"
}

require_arch_linux() {
    [[ -f /etc/arch-release ]] || die "This script is intended to run on Arch Linux."
}

install_dependencies() {
    log "Installing required build/runtime dependencies"
    sudo pacman -S --needed --noconfirm \
        base-devel cmake ninja gcc git openssh namcap \
        extra-cmake-modules \
        qt6-base qt6-declarative qt6-svg qt6-tools qt6-imageformats \
        layer-shell-qt libpulse
}

build_local() {
    log "Building RemiDock locally"
    cmake -S "$PROJECT_ROOT" -B "$PROJECT_ROOT/$BUILD_DIR" -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build "$PROJECT_ROOT/$BUILD_DIR"
}

remove_unmanaged_file_if_present() {
    local path="$1"

    if [[ ! -e "$path" ]]; then
        return
    fi

    if pacman -Qo "$path" >/dev/null 2>&1; then
        warn "Keeping pacman-owned file: $path"
        return
    fi

    warn "Removing old unmanaged RemiDock file that would conflict with pacman: $path"
    sudo rm -rf -- "$path"
}

remove_old_manual_install_conflicts() {
    log "Checking for old unmanaged RemiDock files from previous manual installs"

    remove_unmanaged_file_if_present "/usr/bin/$BINARY_NAME"
    remove_unmanaged_file_if_present "/usr/share/applications/$APP_ID.desktop"
    remove_unmanaged_file_if_present "/usr/share/pixmaps/remidock.png"
}

install_aur_package_locally() {
    if [[ "$SKIP_PACKAGE_INSTALL" == "1" ]]; then
        warn "Skipping package install because SKIP_PACKAGE_INSTALL=1"
        return
    fi

    remove_old_manual_install_conflicts

    log "Installing the built RemiDock package through pacman"
    (
        cd "$AUR_REPO_DIR"
        makepkg --install --syncdeps --noconfirm
    )
}

enable_autostart() {
    if [[ "$SKIP_PACKAGE_INSTALL" == "1" ]]; then
        warn "Skipping autostart because SKIP_PACKAGE_INSTALL=1"
        return
    fi

    log "Adding RemiDock to user autostart"
    mkdir -p "$HOME/.config/autostart"

    if [[ -f "/usr/share/applications/$APP_ID.desktop" ]]; then
        cp "/usr/share/applications/$APP_ID.desktop" "$HOME/.config/autostart/$APP_ID.desktop"
    elif [[ -f "$DESKTOP_SOURCE" ]]; then
        cp "$DESKTOP_SOURCE" "$HOME/.config/autostart/$APP_ID.desktop"
        sed -i 's/^Icon=.*/Icon=remidock/' "$HOME/.config/autostart/$APP_ID.desktop"
    else
        cat > "$HOME/.config/autostart/$APP_ID.desktop" <<EOF
[Desktop Entry]
Type=Application
Name=RemiDock
Comment=Custom Qt/QML dock for KDE Plasma
Exec=$BINARY_NAME
Icon=remidock
Terminal=false
Categories=Utility;
StartupNotify=false
X-GNOME-Autostart-enabled=true
EOF
    fi

    grep -q '^X-GNOME-Autostart-enabled=' "$HOME/.config/autostart/$APP_ID.desktop" || \
        printf 'X-GNOME-Autostart-enabled=true\n' >> "$HOME/.config/autostart/$APP_ID.desktop"
    grep -q '^X-KDE-autostart-after=' "$HOME/.config/autostart/$APP_ID.desktop" || \
        printf 'X-KDE-autostart-after=panel\n' >> "$HOME/.config/autostart/$APP_ID.desktop"

    chmod 644 "$HOME/.config/autostart/$APP_ID.desktop"
}

run_remidock() {
    if [[ "$SKIP_RUN" == "1" ]]; then
        warn "Skipping run because SKIP_RUN=1"
        return
    fi

    log "Starting RemiDock"
    pkill -x "$BINARY_NAME" >/dev/null 2>&1 || true
    nohup "$BINARY_NAME" > "$HOME/.cache/remidock.log" 2>&1 &
    disown || true
}

remote_tag_exists() {
    local tag="$1"
    git ls-remote --exit-code --tags "$GITHUB_REPO" "refs/tags/$tag" >/dev/null 2>&1
}

detect_or_prepare_tag() {
    if [[ -n "${TAG:-}" ]]; then
        REMOTE_TAG="$TAG"
    elif remote_tag_exists "v$VERSION"; then
        REMOTE_TAG="v$VERSION"
    elif remote_tag_exists "$VERSION"; then
        REMOTE_TAG="$VERSION"
    else
        REMOTE_TAG="v$VERSION"
    fi

    if remote_tag_exists "$REMOTE_TAG"; then
        log "Using upstream tag: $REMOTE_TAG"
        return
    fi

    warn "Tag '$REMOTE_TAG' does not exist on $GITHUB_REPO."

    if [[ "$CREATE_TAG_IF_MISSING" == "1" ]] || confirm "Create and push tag '$REMOTE_TAG' from the current local repository?"; then
        [[ -d "$PROJECT_ROOT/.git" ]] || die "Current project is not a git repository. Create/push tag manually first."
        git -C "$PROJECT_ROOT" diff --quiet || die "Local git tree has uncommitted changes. Commit them before tagging."
        git -C "$PROJECT_ROOT" tag -a "$REMOTE_TAG" -m "RemiDock $VERSION"
        git -C "$PROJECT_ROOT" push origin "$REMOTE_TAG"
        remote_tag_exists "$REMOTE_TAG" || die "Tag was pushed but could not be verified on GitHub."
    else
        die "A versioned AUR package needs a public source tag. Push tag '$REMOTE_TAG' first, then rerun."
    fi
}

github_tarball_url() {
    printf 'https://github.com/yousefvand/RemiDock/archive/refs/tags/%s.tar.gz' "$REMOTE_TAG"
}

download_and_checksum_source() {
    local url="$1"
    local tmpfile
    tmpfile="$(mktemp --suffix=.tar.gz)"

    log "Downloading source tarball to calculate sha256"
    curl -L --fail --retry 3 --output "$tmpfile" "$url"
    SOURCE_SHA256="$(sha256sum "$tmpfile" | awk '{print $1}')"
    rm -f "$tmpfile"

    [[ -n "$SOURCE_SHA256" ]] || die "Could not calculate source checksum."
}

write_pkgbuild() {
    local source_url="$1"
    local src_dir='RemiDock-${pkgver}'

    log "Writing PKGBUILD"
    cat > "$AUR_REPO_DIR/PKGBUILD" <<EOF
# Maintainer: Remisa Phillips <remisa.yousefvand@gmail.com>

pkgname=$PKGNAME
pkgver=$VERSION
pkgrel=$PKGREL
pkgdesc='Custom Qt/QML dock for KDE Plasma Wayland'
arch=('x86_64')
url='https://github.com/yousefvand/RemiDock'
license=('$LICENSE_NAME')
depends=(
  'qt6-base'
  'qt6-declarative'
  'qt6-svg'
  'qt6-imageformats'
  'layer-shell-qt'
  'libpulse'
)
makedepends=(
  'cmake'
  'ninja'
  'gcc'
  'extra-cmake-modules'
)
source=("\$pkgname-\$pkgver.tar.gz::$source_url")
sha256sums=('$SOURCE_SHA256')

build() {
  cmake -S "$src_dir" -B build -G Ninja \\
    -DCMAKE_BUILD_TYPE=Release \\
    -DCMAKE_INSTALL_PREFIX=/usr
  cmake --build build
}

package() {
  DESTDIR="\$pkgdir" cmake --install build

  install -Dm644 "$src_dir/icon.png" "\$pkgdir/usr/share/pixmaps/remidock.png"

  if [[ -f "\$pkgdir/usr/share/applications/$APP_ID.desktop" ]]; then
    sed -i 's/^Icon=.*/Icon=remidock/' "\$pkgdir/usr/share/applications/$APP_ID.desktop"
  fi

  # Install a global XDG autostart entry so RemiDock starts automatically
  # for users on their next Plasma/KDE login after installing from AUR.
  if [[ -f "$src_dir/data/$APP_ID.desktop" ]]; then
    install -Dm644 "$src_dir/data/$APP_ID.desktop" "\$pkgdir/etc/xdg/autostart/$APP_ID.desktop"
    sed -i 's/^Icon=.*/Icon=remidock/' "\$pkgdir/etc/xdg/autostart/$APP_ID.desktop"
    grep -q '^X-GNOME-Autostart-enabled=' "\$pkgdir/etc/xdg/autostart/$APP_ID.desktop" || \
      printf 'X-GNOME-Autostart-enabled=true\n' >> "\$pkgdir/etc/xdg/autostart/$APP_ID.desktop"
    grep -q '^X-KDE-autostart-after=' "\$pkgdir/etc/xdg/autostart/$APP_ID.desktop" || \
      printf 'X-KDE-autostart-after=panel\n' >> "\$pkgdir/etc/xdg/autostart/$APP_ID.desktop"
  fi
}
EOF
}

prepare_aur_repo() {
    log "Preparing AUR repository in: $AUR_WORKDIR"
    mkdir -p "$AUR_WORKDIR"

    if [[ -d "$AUR_REPO_DIR/.git" ]]; then
        log "Synchronising existing local AUR repository with origin/master"
        git -C "$AUR_REPO_DIR" fetch origin master
        git -C "$AUR_REPO_DIR" checkout master
        git -C "$AUR_REPO_DIR" rebase origin/master
        return
    fi

    if git clone "ssh://aur@aur.archlinux.org/$PKGNAME.git" "$AUR_REPO_DIR"; then
        return
    fi

    warn "AUR clone failed. Trying to initialise from remote manually."
    mkdir -p "$AUR_REPO_DIR"
    git -C "$AUR_REPO_DIR" init
    git -C "$AUR_REPO_DIR" remote add origin "ssh://aur@aur.archlinux.org/$PKGNAME.git"

    if git -C "$AUR_REPO_DIR" fetch origin master; then
        git -C "$AUR_REPO_DIR" checkout -B master origin/master
    else
        warn "Could not fetch origin/master. Continuing as a new AUR repository."
        git -C "$AUR_REPO_DIR" checkout -B master
    fi
}

test_aur_package() {
    log "Generating .SRCINFO"
    (
        cd "$AUR_REPO_DIR"
        makepkg --printsrcinfo > .SRCINFO
    )

    log "Building AUR source package locally"
    (
        cd "$AUR_REPO_DIR"
        makepkg --clean --syncdeps --noconfirm
    )

    if command -v namcap >/dev/null 2>&1; then
        log "Running namcap checks"
        (
            cd "$AUR_REPO_DIR"
            namcap PKGBUILD || true
            local built_pkg
            built_pkg="$(ls -t ./*.pkg.tar.* 2>/dev/null | head -n 1 || true)"
            if [[ -n "$built_pkg" ]]; then
                namcap "$built_pkg" || true
            fi
        )
    fi
}

publish_aur_package() {
    if [[ "$SKIP_AUR_PUBLISH" == "1" ]]; then
        warn "Skipping AUR publish because SKIP_AUR_PUBLISH=1"
        return
    fi

    log "Checking AUR SSH access"
    ssh -T aur@aur.archlinux.org help >/dev/null || die "AUR SSH check failed."

    log "Committing and pushing to AUR"
    (
        cd "$AUR_REPO_DIR"
        git add PKGBUILD .SRCINFO

        if git diff --cached --quiet; then
            warn "No PKGBUILD/.SRCINFO changes to publish."
            exit 0
        fi

        git commit -m "Update $PKGNAME to $VERSION-$PKGREL"

        if [[ "$NONINTERACTIVE" == "1" ]] || confirm "Push $PKGNAME $VERSION-$PKGREL to the AUR now?"; then
            if ! git push origin master; then
                warn "AUR push was rejected. Fetching remote changes, rebasing, and trying once more."
                git fetch origin master
                git rebase origin/master
                git push origin master
            fi
        else
            if [[ "$CLEAN_AUR_WORKDIR" == "1" ]]; then
                warn "AUR push cancelled. Temporary files will be removed. Rerun with AUR_WORKDIR=/path/to/keep if you want to keep them."
            else
                warn "AUR push cancelled. Files are ready in: $AUR_REPO_DIR"
            fi
        fi
    )
}

main() {
    require_arch_linux

    require_command sudo
    require_command git
    require_command cmake
    require_command ninja
    require_command curl
    require_command ssh
    require_command makepkg
    require_command pacman

    [[ -f "$PROJECT_ROOT/CMakeLists.txt" ]] || die "Run this script from the RemiDock project root."
    [[ -f "$DESKTOP_SOURCE" ]] || warn "Desktop source file missing: $DESKTOP_SOURCE"
    [[ -f "$ICON_SOURCE" ]] || warn "Icon source file missing: $ICON_SOURCE"

    install_dependencies
    build_local
    detect_or_prepare_tag
    SOURCE_URL="$(github_tarball_url)"
    download_and_checksum_source "$SOURCE_URL"

    prepare_aur_repo
    write_pkgbuild "$SOURCE_URL"
    test_aur_package
    install_aur_package_locally
    enable_autostart
    run_remidock
    publish_aur_package

    log "Done."
    log "Local install: /usr/bin/$BINARY_NAME"
    log "Autostart file: $HOME/.config/autostart/$APP_ID.desktop"
    if [[ "$CLEAN_AUR_WORKDIR" == "1" ]]; then
        log "AUR repo was temporary and will be removed: $AUR_REPO_DIR"
    else
        log "AUR repo: $AUR_REPO_DIR"
    fi
}

main "$@"
