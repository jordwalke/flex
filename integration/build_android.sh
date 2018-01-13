#!/usr/bin/env bash
set -e
mkdir -p _dist
echo "Building armv7"
docker run -v $(pwd):/workspace --rm -it yunxing/reason-cross-compile-armv7-32 make android-armv7
cp _build/librelayout.a _dist/librelayout-armv7.a
echo "Artifact stored at _dist/relayout-armv7.a"

echo "Building x86"
docker run -v $(pwd):/workspace --rm -it yunxing/reason-cross-compile-x86-32 make android-x86
cp _build/librelayout.a _dist/librelayout-x86.a
echo "Artifact stored at _dist/relayout-x86.a"
