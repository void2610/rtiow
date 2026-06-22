include(FetchContent)
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)

add_library(rtimage STATIC src/writer.cpp src/stb_impl.cpp)
target_include_directories(rtimage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(rtimage PRIVATE ${stb_SOURCE_DIR})
target_link_libraries(rtimage PUBLIC rtmath)
