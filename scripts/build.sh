#!/usr/bin/env bash

set -euo pipefail

PROJECT_ROOT="$(realpath "$(dirname "${BASH_SOURCE[0]}")/..")"
BUILD_DIR="${PROJECT_ROOT}/build-local"
BUILD_TYPE="${1:-Debug}"

cmake \
    -S "${PROJECT_ROOT}" \
    -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

cmake --build "${BUILD_DIR}" --parallel

echo "guard9 ${BUILD_TYPE} build complete: ${BUILD_DIR}/guard9"
