// Copyright (C) 2020 by Lixian. All rights reserved.
// Date: 2020-07-03
#include <getopt.h>
#include <string>
#include "EventLoop.h"
#include "Server.h"
#include "base/Logging.h"


int main(int argc, char *argv[]) {
    int threadNum = 4;
    int port = 8080;
    std::string logPath = "./WebServer.log";

    /*
    * #include <unistd.h>
    * int getopt(int argc, char * const argv[], const char *optstring);
    * getopt返回值为选项
    * getopt设置的全局变量包括：
    *     1) extern char *optarg: 指向当前选项参数的指针
    *     2) extern int optind: 再次调用getopt时的下一个argv指针的索引
    *     3) extern int opterr: 控制是否向STDERR打印错误(为0表示关闭打印)
    *     4) extern int optopt: 最后一个未知选项
    */

    int opt;
    const char *optString = "t:l:p:";
    while ((opt = getopt(argc, argv, optString)) != -1) {
        switch (opt) { // opt为选项
            case 't': {
                threadNum = atoi(optarg); // optarg为选项参数
                break;
            }
            case 'l': {
                logPath = optarg;
                if (logPath.size() < 2 || optarg[0] != '/') {
                    perror("logPath should start with /");
                    abort();
                }
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            default:
                break;
        }
    }

    Logger::setLogFileName(logPath);
#ifndef _PTHREADS
    LOG << "MACRO _PTHREADS is not defined!";
#endif
    EventLoop mainLoop;
    Server httPServer(&mainLoop, threadNum, port);
    httPServer.start();
    mainLoop.loop();
    return 0;
}