# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.209.6/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version (use Debian 11/9, Ubuntu 18.04/21.04 on local arm64/Apple Silicon): debian-11, debian-10, debian-9, ubuntu-21.04, ubuntu-20.04, ubuntu-18.04
ARG VARIANT="bullseye"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
	&& apt-get -y install --no-install-recommends unzip android-tools-adb apktool libncurses5

WORKDIR /toolchain/
ADD https://dl.google.com/android/repository/android-ndk-r17c-linux-x86_64.zip /toolchain/
RUN unzip /toolchain/android-ndk-r17c-linux-x86_64.zip
ENV ANDROID_NDK_HOME=/toolchain/android-ndk-r17c/
ENV PATH="${ANDROID_NDK_HOME}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/:${ANDROID_NDK_HOME}:${PATH}"
RUN rm /toolchain/android-ndk-r17c-linux-x86_64.zip

WORKDIR /workspaces/gdps-android/aurav2/external/breakpad/breakpad/
RUN git clone https://chromium.googlesource.com/linux-syscall-support src/third_party/lss