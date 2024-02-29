project(test)

# 获取当前文件夹的名称 然后放到build/bin目录下面的对应文件夹里面
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_EXECUTABLE_OUTPUT_PATH}/${CURRENT_FOLDER_NAME})

# 如果有 设置 sub project 的私有的include的文件夹
set(SUBPROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SUBPROJECT_INCLUDE_DIRS})


file(GLOB_RECURSE source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(test ${source_files})

# 添加依赖  
target_link_libraries(test fmt::fmt)
target_link_libraries(test Catch2::Catch2WithMain)
#target_link_libraries(test Catch2::Catch2)
# # target_link_libraries(test Boost::boost) # 不明原因 Boost::boost 不能使用
target_link_libraries(test ${Boost_LIBRARIES})
#target_link_libraries(test spdlog::spdlog)
#target_link_libraries(test plf_colony::plf_colony)