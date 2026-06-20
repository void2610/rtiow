# ヘッダオンリーなのでコンパイル対象がない
add_library(rtmath INTERFACE)
# このライブラリにリンクした相手に自動でインクルードパスが伝わるようにする
target_include_directories(rtmath INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# 使う側にもC++20を伝播させる
target_compile_features(rtmath INTERFACE cxx_std_20)
