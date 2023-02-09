# libaurav2

Core mod code for the 1.9 GDPS Android source.

## Requirements

* android-ndk-r17 or below (must support gnustl)
* Geometry Dash **1.92** APK. 1.93 or above will _not_ work!
* Git

## Building

1. Grab the submodules if they haven't been grabbed already.

2. Setup CMake to point to the Android toolchain and the [CMakeLists.txt](CMakeLists.txt) in this directory.

## Development

This project follows a split publicity model. The public repository squashes all history up to the initial public release (U8.1.2). This is due to the repository historically containing proprietary files and code blacklisted from public release.

The public repository will be updated upon each new release of the 1.9 GDPS. Only the `main` branch from the public repository will be synced. Commits made to the public repository will be synced to the private repository when they are made. Merging will be dealt with personally.

## Documentation

Explanations for some parts of the code, including the crash reporter, can be found in the [docs folder](docs/).

## Licensing

This repository is licensed under the [Mozilla Public License v2.0](LICENSE).
