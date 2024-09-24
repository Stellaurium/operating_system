set(project_name hanoi)
project(${project_name})

# 获取当前文件夹的名称 然后放到build/bin目录下面的对应文件夹里面
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_EXECUTABLE_OUTPUT_PATH}/${CURRENT_FOLDER_NAME})

# 如果有 设置 sub project 的私有的include的文件夹
set(SUBPROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SUBPROJECT_INCLUDE_DIRS})


add_executable(${project_name} src/hanoi_main.cpp)
target_link_libraries(${project_name} fmt::fmt)
target_link_libraries(${project_name} Catch2::Catch2WithMain)
