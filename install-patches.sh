#!/bin/sh

apk_folder=$1
if [ $# -eq 0 ]; then
  echo "Arg 1 must be the path to the apk folder"
  exit 1
fi

echo "[patch] Patching folder $1"

patch_folder="./patch/"

echo "[patch] Running deletes"

del_list="${patch_folder}/DELETE_LIST"
while IFS= read -r line
do
  rm -rf "$apk_folder/$line"
done < "$del_list"

echo "[patch] Copying patch files"

cp -a "$patch_folder/files/." "$apk_folder"
