//
// Created by stellaura on 27/09/24.
//
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>

int main(int argc, char *argv[]) {
  const int num_processes = 500;  // 并发进程数量
  pid_t pids[num_processes];

  for (int i = 0; i < num_processes; ++i) {
    pid_t pid = fork();
    if (pid == 0) {  // 子进程
      // 构造输出文件名
      std::string filename = "output_" + std::to_string(i) + ".txt";

      // 打开文件进行写入，重定向标准输出和标准错误
      int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
      }

      // 重定向标准输出和标准错误到文件
      dup2(fd, STDOUT_FILENO);
      dup2(fd, STDERR_FILENO);
      close(fd);  // 关闭文件描述符

      // 构造命令行参数
      char *args[] = { (char *)"bin/M1/M1-pstree", (char *)"-p", (char *)"-s", (char *)"pid", nullptr };
      execv(args[0], args);
      perror("execv failed");  // 如果 execv 返回，则出错
      exit(EXIT_FAILURE);
    } else if (pid > 0) {  // 父进程
      pids[i] = pid;
    } else {  // fork 出错
      perror("fork failed");
      exit(EXIT_FAILURE);
    }
  }

  // 等待所有子进程结束
  for (int i = 0; i < num_processes; ++i) {
    waitpid(pids[i], nullptr, 0);
  }

  std::cout << "All child processes have completed." << std::endl;
  return 0;
}
