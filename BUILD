package(
  default_visibility = ["//visibility:public"],
)

filegroup(
  name = "path_bins",
  srcs = [":typo-guard"],
)

cc_binary(
  name = "typo-guard",
  srcs = ["main.c"],
  deps = [
    ":app",
    "//data/base16",
    "//third-party:murmurhash3",
  ],
)

cc_library(
  name = "app",
  srcs = ["app.c"],
  hdrs = ["app.h"],
  deps = [
    "//data/base16",
    "//third-party:murmurhash3",
  ],
  visibility = ["//visibility:private"],
)
