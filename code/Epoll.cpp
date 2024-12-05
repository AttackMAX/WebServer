/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:51:26
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-05 22:04:46
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "Epoll.h"
#include "util.h"
#include <string.h>
#include <unistd.h>
#include <vector>

#define MAX_EVENTS 1024

Epoll::Epoll()
{
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    evs = new epoll_event[MAX_EVENTS];
    bzero(evs, sizeof(*evs) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if (epfd != -1)
    {
        close(epfd);
        epfd = -1;
    }
    delete[] evs;
}

void Epoll::addFd(int fd, uint32_t op)
{
    struct epoll_event evin;
    bzero(&evin, sizeof(evin));
    evin.events = op;
    evin.data.fd = fd;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evin), "epoll add event error");
}

std::vector<epoll_event> Epoll::poll(int timeout)
{
    std::vector<epoll_event> activeEve;
    int nfds = epoll_wait(epfd, evs, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; i++)
    {
        activeEve.push_back(evs[i]);
    }
    return activeEve;
}