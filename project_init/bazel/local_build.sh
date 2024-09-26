#!/bin/bash

# Clean useless file
CleanUselessFile() {
  # Remove core file
  if ls core.* 1>/dev/null 2>&1; then
    echo "rm core.*"
    rm core.*
  fi
  # Remove vscode false/ dir
  if [ -d "./false" ]; then
    echo "rm -rf ./false"
    rm -r "./false"
  fi
}
CleanUselessFile

# Define repositories to override
OVERRIDE_REPOS=(
  # "brpc=/data/project/brpc"
)

# Build override options
OVERRIDE_OPTIONS=()
for repo in "${OVERRIDE_REPOS[@]}"; do
  OVERRIDE_OPTIONS+=("--override_repository=$repo")
done

build_type=$1
if [ "$build_type" == "d" -o "$build_type" == "debug" ]; then
  echo "Run bazel build command with debug..."
  bazel build ... --sandbox_debug --experimental_repository_cache=false \
    --compilation_mode=dbg --copt="-O0" --copt="-ggdb3" --strip="never" --copt="-DHOOK_CXA_THROW" \
    "${OVERRIDE_OPTIONS[@]}"
else
  echo "Run bazel build command..."
  bazel build ... --sandbox_debug --experimental_repository_cache=false \
    "${OVERRIDE_OPTIONS[@]}"
fi
