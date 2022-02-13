#!/bin/bash

pushd "$(dirname "$(readlink -f "$0")")" > /dev/null

echo "[build] Running patches"

./install-patches.sh gdps/

echo "[build] Build apk"

apktool b gdps

echo "[build] Sign apk"

./sign-apk.sh

popd > /dev/null
