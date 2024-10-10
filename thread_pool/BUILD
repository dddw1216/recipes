package(default_visibility = ["//visibility:public"])

cc_library(
  name = "thread_pool",
  hdrs = [
    "thread_pool.h",
  ],
  srcs = glob([
    "*.h",
    "*.cc",
  ]),
  deps = [
  ],
  linkopts = [
    "-pthread",
    "-ldl",
  ],
)