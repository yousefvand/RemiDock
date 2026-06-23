#!/usr/bin/env bash
set -Eeuo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

if [[ -r /etc/os-release ]]; then
    # shellcheck disable=SC1091
    . /etc/os-release
else
    echo "Could not read /etc/os-release. Use a distro-specific script in scripts/." >&2
    exit 1
fi

id="${ID:-}"
id_like="${ID_LIKE:-}"

case " $id $id_like " in
    *' arch '*|*' manjaro '*)
        exec "$SCRIPT_DIR/install-arch.sh" "$@"
        ;;
    *' fedora '*|*' rhel '*|*' centos '*)
        exec "$SCRIPT_DIR/install-fedora.sh" "$@"
        ;;
    *' ubuntu '*)
        exec "$SCRIPT_DIR/install-ubuntu.sh" "$@"
        ;;
    *' debian '*)
        exec "$SCRIPT_DIR/install-debian.sh" "$@"
        ;;
    *' opensuse '*|*' suse '*)
        exec "$SCRIPT_DIR/install-opensuse.sh" "$@"
        ;;
    *' alpine '*)
        exec "$SCRIPT_DIR/install-alpine.sh" "$@"
        ;;
    *)
        echo "Unsupported distro ID='$id' ID_LIKE='$id_like'. Use one of the distro-specific scripts in scripts/." >&2
        exit 1
        ;;
esac
