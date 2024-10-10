package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "thread_pool_test",
  srcs = [
    "thread_pool_test.cc",
  ],
  deps = [
    "//thread_pool:thread_pool",
    "//dynamic_pb:dynamic_pb",
    "//dynamic_pb/proto:dynamic_pb_proto",
  ],
  linkopts = [
    "-pthread",
    "-ldl",
  ],
)