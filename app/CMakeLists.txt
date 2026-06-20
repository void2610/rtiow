# 実行ファイルを作る
add_executable(raytracer main.cpp)
# ライブラリをリンク
target_link_libraries(raytracer PRIVATE rtmath)

