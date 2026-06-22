# 実行ファイルを作る
add_executable(raytracer main.cpp)
# ライブラリをリンク
target_link_libraries(raytracer PRIVATE rtcore)
target_link_libraries(raytracer PRIVATE rtimage)

