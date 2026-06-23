#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"

"$PROJECT_ROOT/scripts/build-linux.sh"
exec "$BUILD_DIR/bin/RemiDock"
