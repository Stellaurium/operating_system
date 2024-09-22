# 南京大学 蒋炎岩 操作系统

[bilibili](https://www.bilibili.com/video/BV1Xm411f7CM/)

[课程主页](https://jyywiki.cn/OS/2024/)

已经是三周目了。现在正好是2024的春天，与课程进度同步。希望能够好好学完，至少学完并发。

本项目使用 Ubuntu 22.04LTS 开发。

## 对于代码下载

### 过时内容

点击 课程ppt界面(vscode类似的前端课程界面)
会获得 类似于 `ftp://jyywiki.cn/os/introduction/mini-rv32ima/`
之类的fpt地址

~~正确~~目前可行的的下载方式是

1. ftp jyywiki.cn
2. 用户名输入 anonymous; 密码(为空)输入 \<enter\>
3. 然后就进入了根目录
4. cd 切换目录
5. lcd 切换下载位置
6. mget * 只能下载文件，不能下载文件夹
7. 所以需要lcd不停切换，不停创建新的文件夹，然后再下载文件夹
8. 至少上面的流程可用

~~直接wget要么无限循环 要么卡住~~

### 当前方式
在2024年9月22号，在用ubuntu更新了证书之后，再添加上`--no-check-certificate`就可以正常使用了。

```
sudo apt install ca-certificates
sudo update-ca-certificates
```

通过这个方式，可以下载整个jyy wiki里的内容，相当多


## 项目格式

`~/Programs/course/operating_system_jyy/os-demos`: 直接从网站上wget了全部的内容，没有任何修改

`~/Programs/course/operating_system_jyy`: 自己拿回来重新写的项目 使用 cmake + c++

## 目录结构

projects 目录里存放各种小项目，就是课上下载下来的每一个项目

每个小项目里面有src和include，可能会有CMakeLists.txt，如果没有就是不需要编译

这个小项目里面的include是项目独占的，而根目录下的include是所有小项目共享的

根目录下的CMakeLists.txt负责初始化一些变量，然后逐一调用每个小项目里面的CMakeLists.txt，分离每个项目的编译方式。

整个项目的根目录下的 conanfile.txt 负责管理包的导入等

## 构建方式

首先在根目录 `mkdir build` 存放所有的编译过程中的临时文件，`build/bin`里面存放所有的二进制文件，每个小项目会在`build/bin`里面创建一个新的文件夹，名称为其自己的文件夹名称。`cd build`然后进入。

`conan install .. --build=missing` 执行conan的安装。安装完成后，会生成`./Debug/generators/conan_toolchain.cmake`用于后续cmake的执行。

` cmake .. -DCMAKE_TOOLCHAIN_FILE=./Debug/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"` 执行cmake，获得makefile。

`cmake --build .` 编译

然后可以使用 `make run_<mimi_project_name>`执行相应的mini project。(尽量支持)



## 项目依赖

1. Catch2
2. fmt
3. colony (不一定有用)
3. Boost (不一定有用)
3. spdlog (不一定有用)





