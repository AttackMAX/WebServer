/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:51:26
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 02:06:46
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

// 更新 Channel 的事件
void Epoll::updateChannel(Channel *channel)
{
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;            // 将 Channel 对象的指针存储在 data.ptr 中
    ev.events = channel->getEvents(); // 获取需要监控的事件类型

    if (!channel->getInEpoll())
    {
        // 如果 Channel 尚未在 epoll 中注册，则添加它
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->setInEpoll(); // 标记 Channel 已在 epoll 中注册
    }
    else
    {
        // 如果 Channel 已在 epoll 中注册，则修改其监控的事件
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

// 等待事件发生并返回活跃的 Channel 列表
std::vector<Channel *> Epoll::poll(int timeout)
{
    std::vector<Channel *> activeCha;                     // 存储活跃的 Channel 对象
    int siz = epoll_wait(epfd, evs, MAX_EVENTS, timeout); // 等待事件
    errif(siz == -1, "epoll wait error");                 // 检查 epoll_wait 是否出错

    for (int i = 0; i < siz; i++)
    {
        Channel *ch = (Channel *)evs[i].data.ptr; // 获取触发事件的 Channel 对象
        ch->setRevents(evs[i].events);            // 设置触发的事件
        activeCha.push_back(ch);                  // 将活跃的 Channel 添加到列表中
    }
    return activeCha; // 返回活跃的 Channel 列表
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