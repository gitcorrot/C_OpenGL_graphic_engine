cmake_minimum_required(VERSION 3.2)
project(opengl_test VERSION 0.1.0)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package( Threads REQUIRED )

set( _glfw3_HEADER_SEARCH_DIRS "/usr/local/include")
find_package( GLFW3 NAMES glfw3 glfw REQUIRED )


# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/resources")

# Put all libraries into a variable
set(LIBS glfw3 rt m dl X11 Xau Xdmcp xcb ${CMAKE_THREAD_LIBS_INIT})

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})



set (source "${CMAKE_SOURCE_DIR}/resources")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/resources")
add_custom_command(
 TARGET ${PROJECT_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
 DEPENDS ${destination}
 COMMENT "symbolic link resources folder from ${source} => ${destination}"
)

