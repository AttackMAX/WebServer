/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:21:20
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-05 21:50:13
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>

Socket::Socket()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(int _fd) : fd(_fd)
{
    errif(fd == -1, "socket create error");
}

Socket::~Socket()
{
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *addr)
{
    errif(::bind(fd, (sockaddr *)&addr->addr, addr->addr_len), "socket bind error");
}

void Socket::listen()
{
    errif(::listen(fd, SOMAXCONN), "socket listen error");
}

void Socket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr)
{
    int clnt_fd = ::accept(fd, (sockaddr *)&addr->addr, &addr->addr_len);
    errif(clnt_fd == -1, "socket accept error");
    return clnt_fd;
}

int Socket::getfd()
{
    return fd;
}