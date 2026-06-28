add_library(rtimage STATIC src/writer.cpp src/stb_impl.cpp)
target_include_directories(rtimage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# stbの実装(stb_impl.cpp)で使うのでstbをリンク
target_link_libraries(rtimage PRIVATE stb)
target_link_libraries(rtimage PUBLIC rtmath)
