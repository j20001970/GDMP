#!/usr/bin/env bash

# Script to enable Flatpak environment, adjust RUNTIME and ARGS to fit your needs.

RUNTIME="org.freedesktop.Sdk"
ARGS=("-p" "--share=network")
ARGS+=("--filesystem=xdg-cache/bazel:create")
ARGS+=("--filesystem=xdg-cache/bazelisk:create")

dir=$(realpath $(dirname $0))
while [ $dir != "/" ]
do
    if [ -d "$dir/.git" ]; then
        ARGS+=("--filesystem=$dir")
        break
    else
        dir=$(dirname $dir)
    fi
done

if [ -n "$ANDROID_HOME" ]; then
    ARGS+=("--filesystem=${ANDROID_HOME}")
    ARGS+=("--env=ANDROID_HOME=${ANDROID_HOME}")
fi

if [ -n "$ANDROID_NDK_HOME" ]; then
    ARGS+=("--filesystem=${ANDROID_NDK_HOME}")
    ARGS+=("--env=ANDROID_NDK_HOME=${ANDROID_NDK_HOME}")
fi

flatpak run "${ARGS[@]}" "$@" $RUNTIME
