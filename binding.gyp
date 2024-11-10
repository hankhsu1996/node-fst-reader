{
  "targets": [
    {
      "target_name": "fst_reader",
      "sources": [
        "src/fst_reader.cpp",
        "external/libfst/fstapi.c",
        "external/libfst/fastlz.c",
        "external/libfst/lz4.c"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "external/libfst"
      ],
      "cflags": ["-fexceptions"],
      "cflags_cc": ["-fexceptions"],
      "link_settings": {
        "libraries": [
          "-lz"
        ]
      },
      "defines": [
        "NAPI_CPP_EXCEPTIONS",
        "FST_CONFIG_INCLUDE=\"fstapi.h\""
      ]
    }
  ]
}
