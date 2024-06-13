cmake_minimum_required(VERSION 3.0)

project(data-self-organisation-viewer)

set(CMAKE_BUILD_TYPE "Debug")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Vulkan)
find_package(X11 REQUIRED)

include_directories("include")
include_directories(${Vulkan_INCLUDE_DIR})

set(ALL_LIBS ${Vulkan_LIBRARY})

file(GLOB SOURCE CONFIGURE_DEPENDS 
"${PROJECT_SOURCE_DIR}/source/*.cpp"
"${PROJECT_SOURCE_DIR}/source/graphics/vulkan/primatives/*.cpp"
"${PROJECT_SOURCE_DIR}/source/graphics/vulkan/*.cpp"
"${PROJECT_SOURCE_DIR}/source/graphics/vulkan/shader/*.cpp"
"${PROJECT_SOURCE_DIR}/source/graphics/*.cpp"
"${PROJECT_SOURCE_DIR}/source/core/*.cpp"
"${PROJECT_SOURCE_DIR}/source/tracer/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${PROJECT_NAME} ${JPEG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
target_link_libraries(${PROJECT_NAME} X11)
