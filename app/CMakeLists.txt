# GUI依存(GLFW + ImGui)を取得する
include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# ImGuiはCMakeを持たないのでソースを取得してターゲットを自前で組む
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.5
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw)
if(APPLE)
  target_link_libraries(imgui PUBLIC "-framework OpenGL")
endif()

# 実行ファイルを作る
add_executable(raytracer main.cpp)

# システムからThreadsを探す
find_package(Threads REQUIRED)

# ライブラリをリンク
target_link_libraries(raytracer PRIVATE rtcore)
target_link_libraries(raytracer PRIVATE rtimage)
target_link_libraries(raytracer PRIVATE Threads::Threads)
target_link_libraries(raytracer PRIVATE imgui)
