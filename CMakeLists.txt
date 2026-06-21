# プロジェクトの基本設定
cmake_minimum_required(VERSION 3.20)
project(raytracer CXX)

# LSPに対応させる
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# ルートにリンクを貼る
file(CREATE_LINK ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# サブディレクトリの登録 (依存される側(lib)が先)
add_subdirectory(libs/math)
add_subdirectory(libs/core)
add_subdirectory(app)
