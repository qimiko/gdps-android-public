#!/bin/sh

apk_file=$1
if [ $# -eq 0 ]; then
  echo "Arg 1 must be the path to the apk file"
  exit 1
fi

apktool d "$apk_file" -f -o gdps

mkdir -p out

./install-patches.sh gdps