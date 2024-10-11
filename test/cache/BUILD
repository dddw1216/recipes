package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "demo_lru_cache_test",
  srcs = [
    "demo_lru_cache_test.cc",
  ],
  deps = [
    "//cache:cache",
    "//thread_pool:thread_pool",
    "//dynamic_pb:dynamic_pb",
    "//dynamic_pb/proto:dynamic_pb_proto",
  ],
  linkopts = [
    "-pthread",
    "-ldl",
  ],
)