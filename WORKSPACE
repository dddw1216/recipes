load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

### bazel_deps
git_repository(
    name = "bazel_deps",
    remote = "https://github.com/dddw1216/bazel_deps.git",
    branch = "main",
)

### leveldb from bazel_deps
load("@bazel_deps//bazel_deps/leveldb:leveldb.bzl", "leveldb_workspace")
leveldb_workspace()

### google_absl
http_archive(
    name = "com_google_absl",
    urls = [
      "https://github.com/abseil/abseil-cpp/archive/98eb410c93ad059f9bba1bf43f5bb916fc92a5ea.zip",
    ],
    strip_prefix = "abseil-cpp-98eb410c93ad059f9bba1bf43f5bb916fc92a5ea",
    sha256 = "aabf6c57e3834f8dc3873a927f37eaf69975d4b28117fc7427dfb1c661542a87",
)

### google_protobuf
http_archive(
    name = "com_google_protobuf",
    sha256 = "3bd7828aa5af4b13b99c191e8b1e884ebfa9ad371b0ce264605d347f135d2568",
    strip_prefix = "protobuf-3.19.4",
    urls = [
        "https://github.com/protocolbuffers/protobuf/archive/v3.19.4.tar.gz",
    ],
)
load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()