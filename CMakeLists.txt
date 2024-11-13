#需求的最低版本cmake程序版本
cmake_minimum_required(VERSION 3.12)

#本工程的名字
project(OpenGL)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#找到include文件夹
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include)
#找到lib文件夹
link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib/glfw
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib/assimp
)


#把文件添加到主文件中

add_subdirectory(render)

#把需要拷贝的资源路径都放到ASSETS里

file(GLOB ASSETS "./render/shader/assets" "./assets""./assets/texture""./thirdParty/lib/assimp/assimp-vc143-mtd.dll")

#把ASSETS指代的目录集合的内容，都拷贝到可执行文件目录下
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

#搜索所有的Cpp，加入SRCS变量中
#aux_source_directory(. SRCS)

#本工程主程序文件及输出程序名称
#add_executable(GL_Study ${SRCS})
add_executable(opengl "main.cpp" )

add_executable(test "test.cpp")
#把库文件链接到项目main文件中
target_link_libraries(opengl glfw3.lib assimp-vc143-mtd.lib render_lib)
target_link_libraries(test glfw3.lib assimp-vc143-mtd.lib render_lib)