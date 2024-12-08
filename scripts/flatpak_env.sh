#!/usr/bin/env bash

# Script to enable Flatpak environment, adjust RUNTIME and ARGS to fit your needs.

RUNTIME="org.freedesktop.Sdk"
ARGS=("-p" "--share=network")
ARGS+=("--filesystem=xdg-cache/bazel")
ARGS+=("--filesystem=xdg-cache/bazelisk")

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

flatpak run "${ARGS[@]}" "$@" $RUNTIME
