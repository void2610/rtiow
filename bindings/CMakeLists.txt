nanobind_add_module(_rtiow
  STABLE_ABI # Python 3.12+で安定ABIを使う
  NB_STATIC # nanobindランタイムを静的リンク
  bindings.cpp
)

target_link_libraries(_rtiow PRIVATE rtcore rtimage)

# ビルド後に自動コピー
set_target_properties(_rtiow PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/python/rtiow"
)
