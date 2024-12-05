/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:51:24
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-05 22:01:04
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll
{
private:
    int epfd;
    struct epoll_event *evs;

public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);
    std::vector<epoll_event> poll(int timeout = -1);
};
