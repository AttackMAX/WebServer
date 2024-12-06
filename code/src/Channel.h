/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 00:35:33
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 12:08:37
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel
{
private:
    EventLoop *loop;  // 指向一个Epoll类
    int fd;           // 负责的文件描述符
    uint32_t events;  // 希望监听这个文件描述符的那些时间
    uint32_t revents; // 返回该Channel时文件描述符正在发生的事件
    bool inEpoll;     // 表示当前Channel是否已经在epoll红黑树中，为了方便区分使用EPOLL_CTL_ADD还是EPOLL_CTL_MOD
    std::function<void()> callback;

public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();
    void handleEvent();
    void enableReading(); // 监听该Channel上的读事件
    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();
    void setRevents(uint32_t);
    void setCallback(std::function<void()>);
};