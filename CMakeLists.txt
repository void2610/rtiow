# プロジェクトの基本設定
cmake_minimum_required(VERSION 3.20)
project(raytracer CXX)

# LSPに対応させる
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# stbをルートで取得し、複数ライブラリで共有する
include(FetchContent)
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# ヘッダオンリーのstbをインクルードパスとして配るINTERFACEライブラリ
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

# サブディレクトリの登録 (依存される側(lib)が先)
add_subdirectory(libs/math)
add_subdirectory(libs/core)
add_subdirectory(libs/image)

option(BUILD_PYTHON_BINDINGS "Build nanobind Python bindings" ON)

if(BUILD_PYTHON_BINDINGS)
  # uvのpythonを優先
  set(Python_FIND_VIRTUALENV FIRST)
  set(Python_FIND_FRAMEWORK LAST)

  # nanobindが要求する最低限
  find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)
  # pipで入れたnanobindのCMake configを探させる
  execute_process(
    COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE nanobind_ROOT
  )
  find_package(nanobind CONFIG REQUIRED)
endif()

add_subdirectory(bindings)
add_subdirectory(app)
