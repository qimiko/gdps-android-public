#!/bin/sh
# This script is intended to "sign" the apk file at gdps/dist/gdps19.apk and put it into the out/ folder.

echo "[error] Signing tool not defined, your apk will fail to install!"
cp gdps/dist/gdps19.apk out/gdps19.apk
