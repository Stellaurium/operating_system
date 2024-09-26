project(logisim)


# 获取当前文件夹的名称 然后放到build/bin目录下面的对应文件夹里面
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(EXECUTABLE_OUTPUT_PATH ${ROOT_EXECUTABLE_OUTPUT_PATH}/${CURRENT_FOLDER_NAME})

# 如果有 设置 sub project 的私有的include的文件夹
set(SUBPROJECT_INCLUDE_DIRS include)
include_directories(${SUBPROJECT_INCLUDE_DIRS})


#file(GLOB_RECURSE c_source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE cpp_source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(source_files ${cpp_source_files})


add_executable(logisim ${source_files})

target_link_libraries(logisim fmt::fmt)
#target_link_libraries(logisim Catch2::Catch2WithMain)

# 定义一个自定义目标来运行logisim
add_custom_target(run_logisim
        COMMAND ${EXECUTABLE_OUTPUT_PATH}/logisim | ${PYTHON_EXECUTABLE} src/seg-display.py
#        DEPENDS 的文件是 add_executable编译出来的可执行文件，需要保证编译出来了之后 再调用这个命令
        DEPENDS logisim
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Run logisim and display via seg-display.py"
        )
