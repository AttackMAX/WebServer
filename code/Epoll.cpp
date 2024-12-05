/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:51:26
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 01:09:18
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "Epoll.h"
#include "util.h"
#include "Channel.h"
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

void Epoll::updateChannel(Channel *channel)
{
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if (!channel->getInEpoll())
    {
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->setInEpoll();
    }
    else
    {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

std::vector<Channel * > Epoll::poll(int timeout)
{
    std::vector<Channel * > activeCha;
    int siz = epoll_wait(epfd, evs, MAX_EVENTS, timeout);
    errif(siz == -1, "epoll wait error");
    for (int i = 0; i < siz; i++)
    {
        Channel *ch = (Channel *)evs[i].data.ptr;
        ch->setRevents(evs[i].events);
        activeCha.push_back(ch);
    }
    return activeCha;
}

// std::vector<epoll_event> Epoll::poll(int timeout)
// {
//     std::vector<epoll_event> activeEve;
//     int nfds = epoll_wait(epfd, evs, MAX_EVENTS, timeout);
//     errif(nfds == -1, "epoll wait error");
//     for (int i = 0; i < nfds; i++)
//     {
//         activeEve.push_back(evs[i]);
//     }
//     return activeEve;
// }