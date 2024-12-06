/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-03 12:02:23
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 12:05:29
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "src/util.h"

#define BUFFER_SIZE 1024

int main()
{
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    // 定义服务器地址结构
    struct sockaddr_in serve_addr;
    bzero(&serve_addr, sizeof(serve_addr));              // 将服务器地址结构清零
    serve_addr.sin_family = AF_INET;                     // 设置地址族为IPv4
    serve_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置IP地址为127.0.0.1
    serve_addr.sin_port = htons(8888);                   // 设置端口号为8888，使用网络字节序

    // 连接到服务器
    errif(connect(sockfd, (sockaddr *)&serve_addr, sizeof(serve_addr)) == -1, "socket connect error");

    while (1)
    {
        char buf[BUFFER_SIZE];
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if (write_bytes == -1)
        {
            printf("socket alredy disconnected,can't write any more!\n");
            break;
        }
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("message from client fd %d: %s\n", sockfd, buf);
        }
        else if (read_bytes == 0)
        {
            printf("server fd %d disconneted\n", sockfd);
            close(sockfd);
            break;
        }
        else if (read_bytes == -1)
        {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);

    return 0;
}