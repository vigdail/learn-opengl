cmake_minimum_required(VERSION 3.10)

project(Hello_OpenGL)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB_RECURSE SRC_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE INC_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.hpp)
    
message(${SRC_FILES})

add_executable(Hello_OpenGL src/main.cpp ${SRC_FILES} ${INC_FILES})

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
add_library("glad" "${LIB_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${LIB_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${LIB_DIR}/include")

add_compile_definitions(_DEBUG)

add_dependencies(Hello_OpenGL OpenGL::OpenGL)
include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(Hello_OpenGL OpenGL::OpenGL glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS} ${GLEW_LIBRARIES} glad)