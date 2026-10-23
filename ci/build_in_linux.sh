#!/bin/bash
set -euo pipefail

root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$root"

require_executable() {
    if [[ ! -s "$1" || ! -x "$1" ]]; then
        echo "Missing or non-executable build artifact: $1" >&2
        exit 1
    fi
}

# Build the application with the configured generator (Makefiles or Ninja).
mkdir -p build
(
    cd build
    cmake ..
    cmake --build . -- -j"$(nproc)"
)
require_executable bin/Caster_Service
require_executable bin/tool/strsvr_mult

# Build and package the runtime dependencies only after the application succeeds.
make -C env/redis-7.2.1 MALLOC=libc -j"$(nproc)"
mkdir -p bin/env/redis bin/env/scripts
cp env/redis-7.2.1/src/redis-server bin/env/redis/redis-server
require_executable bin/env/redis/redis-server
cp .cmake/redis_inux.conf.in bin/env/redis/redis.config
cp -r env/scripts/ bin/env
