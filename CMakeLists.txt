cmake_minimum_required(VERSION 3.26)
project(fin_program C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_subdirectory(src)
##add_subdirectory()
#
## 设置SDL库文件的路径
#set(SDL_LIBRARY_PATH "./SDL/lib")
#
## 设置SDL头文件的路径
#set(SDL_INCLUDE_PATH "./SDL/include")
#
## 添加SDL头文件路径
#include_directories(${SDL_INCLUDE_PATH})
#
## 添加SDL静态库
#set(SDL_LIBRARY
#        ${SDL_LIBRARY_PATH}/libSDL2.a
#        ${SDL_LIBRARY_PATH}/libSDL2main.a
#        ${SDL_LIBRARY_PATH}/libSDL2_image.a
#        ${SDL_LIBRARY_PATH}/libSDL2_mixer.a
#        ${SDL_LIBRARY_PATH}/libSDL2_ttf.a
#        # 添加其他可能需要的SDL库文件
#)
#
#link_libraries(SDL_LIBRARY)

link_directories("./SDL2.dll")
target_link_libraries(fin_program SDL2main)
target_link_libraries(fin_program SDL2)
target_link_libraries(fin_program SDL2_image)
target_link_libraries(fin_program SDL2_ttf)
target_link_libraries(fin_program SDL2_mixer)