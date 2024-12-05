/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-04 01:05:03
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 01:10:20
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
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "Channel.h"

// #define MAX_EVENTS 1024
#define READ_BUFFER 1024

// void setnonblocking(int fd)
// {
//     fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
// }

void handleReadEvent(int);

int main()
{
    Socket *serve_sock = new Socket();
    InetAddress *serve_addr = new InetAddress("127.0.0.1", 8888);
    serve_sock->bind(serve_addr);

    serve_sock->listen();

    Epoll *ep = new Epoll();
    serve_sock->setnonblocking();
    Channel *serveChannel = new Channel(ep, serve_sock->getfd());
    serveChannel->enableReading();

    while (1)
    {
        std::vector<Channel *> activeChannels = ep->poll();
        int epoll_siz = activeChannels.size();
        for (int i = 0; i < epoll_siz; i++)
        {
            if (activeChannels[i]->getFd() == serve_sock->getfd())
            {
                InetAddress *client_addr = new InetAddress();
                Socket *client_sock = new Socket(serve_sock->accept(client_addr));
                printf("new client fd %d! IP: %s Port: %d\n", client_sock->getfd(),
                       inet_ntoa(client_addr->addr.sin_addr),
                       ntohs(client_addr->addr.sin_port));
                client_sock->setnonblocking();
                Channel *clientChannel = new Channel(ep, client_sock->getfd());
                clientChannel->enableReading();
            }
            else if (activeChannels[i]->getRevents() & EPOLLIN)
            { // 可读事件
                handleReadEvent(activeChannels[i]->getFd());
            }
            else
            {
                printf("something else happened\n");
            }
        }
    }
    delete serve_sock;
    delete serve_addr;
    return 0;
}

void handleReadEvent(int socked)
{
    char buf[READ_BUFFER];
    while (1) // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(socked, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("message from client fd %d: %s\n", socked, buf);
            write(socked, buf, sizeof(buf));
        }
        else if (read_bytes == 0) // EOF，客户端断开连接
        {
            printf("client fd %d disconneted\n", socked);
            close(socked);
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