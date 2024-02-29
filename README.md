# 南京大学 蒋炎岩 操作系统

[bilibili](https://www.bilibili.com/video/BV1Xm411f7CM/)

[课程主页](https://jyywiki.cn/OS/2024/)

已经是三周目了。现在正好是2024的春天，与课程进度同步。希望能够好好学完，至少学完并发。

## 对于代码下载

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

## 项目格式

`~/Programs/course/operating_system/operating_system_jyy_base`: 原始的ftp根目录结构 原始文件 没有任何修改

`~/Programs/course/operating_system/operating_system_jyy`: 自己拿回来重新写的项目 使用 cmake + c++

因为项目里面 什么代码都有,c/python等，还是从c++里面拿了出来

## 目录结构

目前的想法是： src里面存放各种各样的小项目(例如 电路模拟器，递归等项目) 每个项目一个文件夹

然后文件夹里面一个cmake，用于该文件自己的编译

整个项目的根目录下有一个cmake 管理包的导入等

最后的结果就是 所有的小项目被编译到根目录中build的对应的文件夹里面

同时 cmake 还可以有类似 make里面 make clear之类的命令

(现在还不会上面的cmake怎么写 然后顺带学了cmake)

## 项目依赖

1. Catch2
2. fmt
3. colony (不一定有用)
3. Boost (不一定有用)
3. spdlog (不一定有用)





