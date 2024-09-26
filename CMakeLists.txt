cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(operating_system)

set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
# 临时变量 为了子cmake继承使用 在子cmake内部再配置保存位置
set(ROOT_EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
# python 解释器位置
set(PYTHON_EXECUTABLE ${PROJECT_SOURCE_DIR}/venv/bin/python)


# 寻找库 在构建的时候再链接 (因为有的子项目可能不用)
find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)
find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)


include_directories(${PROJECT_INCLUDE_DIRS})


# =====================================================================================
# 需要排除的目录名称
set(EXCLUDED_DIRECTORIES "compiler-opt" "strace" "minimal" "mosaic")
# =====================================================================================


# 获取所有子项目的目录
file(GLOB SECTIONS "${PROJECT_SOURCE_DIR}/projects/*")

# 遍历每个项目并添加
foreach (SECTION ${SECTIONS})
    if (IS_DIRECTORY ${SECTION})
        # 获取每个项目中的二级目录
        file(GLOB SUB_PROJECTS "${SECTION}/*")
        foreach (SUB_PROJECT ${SUB_PROJECTS})
            if (IS_DIRECTORY ${SUB_PROJECT})
                # 获取最后一个/后的名称 方便使用 EXCLUDED_DIRECTORIES 进行排除
                get_filename_component(LAST_PART ${SUB_PROJECT} NAME)
                if (NOT ${LAST_PART} IN_LIST EXCLUDED_DIRECTORIES)
                    add_subdirectory(${SUB_PROJECT})
                endif ()
            endif ()
        endforeach ()
    endif ()
endforeach ()

