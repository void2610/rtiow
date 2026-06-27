# 仮想関数の定義を持つシーン要素を STATIC ライブラリにする
add_library(rtcore STATIC
  src/sphere.cpp
  src/moving_sphere.cpp
  src/hittable_list.cpp
  src/camera.cpp
)
# 自身のビルドと利用側の両方にインクルードパスを伝える
target_include_directories(rtcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# ヘッダで rtmath の型を公開しているので PUBLIC で伝播させる
target_link_libraries(rtcore PUBLIC rtmath)
