set(project_name M1-pstree)
project(${project_name})

# 获取当前文件夹的名称 然后放到build/bin目录下面的对应文件夹里面
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_EXECUTABLE_OUTPUT_PATH}/${CURRENT_FOLDER_NAME})

# 如果有 设置 sub project 的私有的include的文件夹
set(SUBPROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SUBPROJECT_INCLUDE_DIRS})


file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM source_files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/M1-test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/M1-main.cpp)


add_executable(test_${project_name} ${source_files} ${CMAKE_CURRENT_SOURCE_DIR}/src/M1-test.cpp)
add_executable(${project_name} ${source_files} ${CMAKE_CURRENT_SOURCE_DIR}/src/M1-main.cpp)

# 添加依赖
target_link_libraries(test_${project_name} fmt::fmt)
target_link_libraries(test_${project_name} Catch2::Catch2WithMain)
target_link_libraries(${project_name} fmt::fmt)
