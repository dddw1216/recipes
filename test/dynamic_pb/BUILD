package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "pb_message_factory_test",
  srcs = [
    "pb_message_factory_test.cc",
  ],
  deps = [
    "@com_google_protobuf//:protobuf",
    "//dynamic_pb:dynamic_pb",
    "//dynamic_pb/proto:dynamic_pb_proto",
  ],
  linkopts = [
    "-pthread",
    "-ldl",
  ],
)