/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:51:24
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:52:26
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
private:
    int epfd;
    struct epoll_event *events;

public:
    Epoll();
    ~Epoll();

    // void addFd(int fd, uint32_t op);
    void updateChannel(Channel *);
    // std::vector<epoll_event> poll(int timeout = -1);
    std::vector<Channel *> poll(int timeout = -1);
};
