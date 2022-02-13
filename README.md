# 1.9 GDPS Android

## Requirements

* APKTool in path
* android-ndk-r17 or below (must support gnustl)
* Geometry Dash **1.92** APK. 1.93 or above will _not_ work!
* Git

## Building

1. Grab the submodules if they haven't been grabbed already.

2. Setup a copy of the APK by providing it as the first argument in [setup-apk.sh](setup-apk.sh).  
   Example usage: `./setup-apk.sh GeometryDash192.apk`  

3. Setup CMake to point to the Android toolchain and the [CMakeLists.txt](CMakeLists.txt) in this directory.

4. Modify [build-apk.sh](build-apk.sh) to sign the APK.  
   [Uber APK Signer](https://github.com/patrickfav/uber-apk-signer) is the signing tool used for release builds.

5. Build the APK.  
	 This will initially build a library. Using the `install` target will run the rest of the scripts, putting an APK into the `out` directory.  
	 See the project's [VSCode tasks](.vscode/tasks.json) for some example build commands.

## Development

This project follows a split publicity model. The public repository squashes all history up to the initial public release (U8.1.2). This is due to the repository historically containing proprietary files and code blacklisted from public release.

The public repository will be updated upon each new release of the 1.9 GDPS. Only the `main` branch from the public repository will be synced. Commits made to the public repository will be synced to the private repository when they are made. Merging will be dealt with personally.

## Licensing

This repository is licensed under the [Mozilla Public License v2.0](LICENSE).