/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-04 01:05:03
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-05 20:27:31
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include "util.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    // 定义服务器地址结构
    struct sockaddr_in serve_addr;
    bzero(&serve_addr, sizeof(serve_addr));              // 将服务器地址结构清零
    serve_addr.sin_family = AF_INET;                     // 设置地址族为IPv4
    serve_addr.sin_port = htons(8888);                   // 设置端口号为8888，使用网络字节序
    serve_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置IP地址为127.0.0.1

    // 绑定套接字到指定的IP地址和端口号
    errif(bind(sockfd, (sockaddr *)&serve_addr, sizeof(serve_addr)) == -1, "socket bind error");

    // 监听套接字，等待客户端连接
    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

    int epollfd = epoll_create1(0);
    errif(epollfd == -1, "epoll create error");

    struct epoll_event ev, evs[MAX_EVENTS];
    bzero(evs, sizeof(evs));
    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    setnonblocking(sockfd);
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (1)
    {
        int nfds = epoll_wait(epollfd, evs, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error");
        for (int i = 0; i < nfds; i++)
        {
            if (evs[i].data.fd == sockfd)
            {
                // 定义客户端地址结构
                struct sockaddr_in client_addr;
                bzero(&client_addr, sizeof(client_addr));        // 将客户端地址结构清零
                socklen_t client_addr_len = sizeof(client_addr); // 初始化客户端地址长度
                // 接受客户端连接
                int client_sockfd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_len);
                errif(client_sockfd == -1, "socket accept error");
                // 打印客户端的文件描述符、IP地址和端口号
                printf("new client fd %d! IP %s Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                bzero(&ev, sizeof(ev));
                ev.data.fd = client_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(client_sockfd);
                epoll_ctl(epollfd, EPOLL_CTL_ADD, client_sockfd, &ev);
            }
            else if (evs[i].events & EPOLLIN)
            { // 可读事件
                char buf[READ_BUFFER];
                while (1) // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
                {
                    bzero(buf, sizeof(buf));
                    ssize_t read_bytes = read(evs[i].data.fd, buf, sizeof(buf));
                    if (read_bytes > 0)
                    {
                        printf("message from client fd %d: %s\n", evs[i].data.fd, buf);
                        write(evs[i].data.fd, buf, sizeof(buf));
                    }
                    else if (read_bytes == 0) // EOF，客户端断开连接
                    {
                        printf("client fd %d disconneted\n", evs[i].data.fd);
                        close(evs[i].data.fd);
                        break;
                    }
                    else if (read_bytes == -1 && errno == EINTR) // 客户端正常中断，继续读取
                    {
                        printf("continue reading");
                        continue;
                    }
                    else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
                    {
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    }
                }
            }
            else
            {
                printf("something else happened\n");
            }
        }
    }
    close(sockfd);
    return 0;
}