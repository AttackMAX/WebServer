/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-04 01:05:03
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-04 14:05:53
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

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

    // 定义客户端地址结构
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));        // 将客户端地址结构清零
    socklen_t client_addr_len = sizeof(client_addr); // 初始化客户端地址长度
    // 接受客户端连接
    int client_sockfd = accept(sockfd, (sockaddr *)&client_addr, &client_addr_len);
    errif(client_sockfd == -1, "socket accept error");
    // 打印客户端的文件描述符、IP地址和端口号
    printf("new client fd %d! IP %s Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    while (1)
    {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(client_sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("message from client fd %d: %s\n", client_sockfd, buf);
            write(client_sockfd, buf, sizeof(buf));
        }
        else if (read_bytes == 0)
        {
            printf("client fd %d disconneted\n", client_sockfd);
            close(client_sockfd);
            break;
        }
        else if (read_bytes == -1)
        {
            close(client_sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}