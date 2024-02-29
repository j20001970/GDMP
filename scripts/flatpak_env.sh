#!/usr/bin/env bash

# Script to enable Flatpak environment, adjust RUNTIME and ARGS to fit your needs.

RUNTIME="org.freedesktop.Sdk"
ARGS=("-p" "--share=network")
ARGS+=("--filesystem=xdg-cache/bazel")
ARGS+=("--filesystem=xdg-cache/bazelisk")
ARGS+=("--filesystem=$(pwd)")

flatpak run "${ARGS[@]}" "$@" $RUNTIME
