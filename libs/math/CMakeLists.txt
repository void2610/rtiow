# 仮想関数の定義を持つ .cpp をコンパイルするため STATIC ライブラリにする
add_library(rtmath STATIC
  src/sphere.cpp
  src/hittable_list.cpp
)
# 自身のビルドと利用側の両方にインクルードパスを伝える
target_include_directories(rtmath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# 使う側にもC++20を伝播させる
target_compile_features(rtmath PUBLIC cxx_std_20)
