package(default_visibility = ["//visibility:public"])

cc_library(
  name = "dynamic_pb",
  hdrs = [
    "pb_message_factory.h",
  ],
  srcs = glob([
    "*.h",
    "*.cc",
  ]),
  deps = [
    "@com_google_protobuf//:protobuf",
  ],
  linkopts = [
    "-pthread",
    "-ldl",
  ],
)