package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "dynamic_pb_test",
  srcs = [
    "dynamic_pb_test.cc",
  ],
  deps = [
    "@com_google_protobuf//:protobuf",
    "//dynamic_pb/proto:dynamic_pb_proto",
  ],
  linkopts = [
    "-pthread",
    "-ldl",
  ],
)