/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 11:47:42
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 12:19:23
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : loop(_loop)
{
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonblocking();

    Channel *servChannel = new Channel(loop, serv_sock->getfd());
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    servChannel->setCallback(cb);
    servChannel->enableReading();
}

Server::~Server()
{
}

void Server::handleReadEvent(int sockfd)
{
    char buf[READ_BUFFER];
    while (true)
    { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            // buf[bytes_read] = '\0';
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if (bytes_read == -1 && errno == EINTR)
        { // 客户端正常中断、继续读取
            printf("continue reading");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { // 非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
        else if (bytes_read == 0)
        { // EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd); // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock)
{
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getfd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    Channel *clntChannel = new Channel(loop, clnt_sock->getfd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getfd());
    clntChannel->setCallback(cb);
    clntChannel->enableReading();
}