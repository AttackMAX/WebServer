/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:51:24
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 01:54:26
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
    int epfd;                // epoll 文件描述符
    struct epoll_event *evs; // 存储 epoll 事件的数组

public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op); // 将文件描述符添加到 epoll 监控列表，并指定监控的事件类型
    // std::vector<epoll_event> poll(int timeout = -1);
    void updateChannel(Channel *);                 // 更新已有 Channel 对应的文件描述符的事件
    std::vector<Channel *> poll(int timeout = -1); // 等待事件发生，并返回活跃的 Channel 列表
};
