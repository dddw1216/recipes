load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_proto_library(
    name = "dynamic_pb_proto",
    deps = [
        ":toolslib_dynamic_pb_proto",
    ],
)

proto_library(
    name = "toolslib_dynamic_pb_proto",
    srcs = glob([
        "*.proto",
    ]),
)