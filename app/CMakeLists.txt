# 実行ファイルを作る
add_executable(raytracer main.cpp)

# システムからThreadsを探す
find_package(Threads REQUIRED)

# ライブラリをリンク
target_link_libraries(raytracer PRIVATE rtcore)
target_link_libraries(raytracer PRIVATE rtimage)
target_link_libraries(raytracer PRIVATE Threads::Threads)

