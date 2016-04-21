# spawn-fcgi


## 梗概

本文内容对Spawn-fcgi源码进行解读，简要说明其原理，并具体说明其实现方式。

Spawn-fcgi 源码虽然只有600多行，但是初次阅读起来依然需要花很多时间。为了节省读者的学习成本，提高学习Spawn-fcgi 的效果，作者对Spawn-fcgi的源码做了裁剪，保留最核心的功能和原有的代码结构，且能编译后正常运行。最后代码只有200多行。

[源码地址在这里][1]

## 必备知识

要阅读Spawn-fcgi，读者至少需要掌握以下几个方面的知识或技能：

*   了解CGI和FastCgi的概念，了解其使用场景
*   基础的Linux C 环境编程，会使用常见的库函数比如getopt、exec 等
*   基础的Linux C 的多进程编程，熟悉fork、waitpid、setsid等函数
*   基础的Linux C 网络编程，熟悉建立tcp连接、select非阻塞方式通信，多路复用I/O等

## 裁剪后Spawn-fcgi的执行过程

*   创建服务器socket
*   fork进程，子进程初始化会有两个主要操作：

1.  把socket的文件描述符，复制到FCGI_LISTENSOCK_FILENO
2.  会执行execl 函数，运行cgi程序，并让cgi程序拥有子进程的上下文环境
3.  运行cgi程序后，使用FCGI_LISTENSOCK_FILENO这个描述符，来与webserver进行通信。

## 如何编译

gcc -o spawn-fcgi spawn-fcgi.c

一行命令即可

## 如何调用

./spawn-fcgi -f cgi -p 9001 -F 256

裁剪后也仅支持接收这三个参数

## 作者对于Spawn-fcgi的思考

*   fastcgi 协议规定，fcgi管理器中把网络描述符定为FCGI_LISTENSOCK_FILENO，为了一致CGI程序中复用FCGI_LISTENSOCK_FILENO的套接字，总感觉不是很完美。
*   Spawn-fcgi太简单，不需要单独做一个软件，完全可以集成到cgi程序中
*   Spawn-fcgi使用的是多进程，如果集成到cgi程序中，可以自由选择多进程模型、多线程模型
*   缺少进程守护监控，spawn-fcgi如果一个进程挂掉，不会被重启。
*   Spawn-fcgi 网络多路复用调用的是select，但现在最常用的是epoll

## Spawn-fcgi 补充知识

### 与原版相比，裁剪后Spawn-fcgi的失去了哪些功能

*   使用linux套接字文件建立tcp连接功能
*   对IPv6的支持
*   对root、group 用户的检查
*   对windows、Solaris等编译环境的支持
*   通过进程pid文件获取cgi进程
*   去除对autoconf等编译工具的依赖
*   ...

 [1]: https://github.com/zuocheng-liu/spawn-fcgi
