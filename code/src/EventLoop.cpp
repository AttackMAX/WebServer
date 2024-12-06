/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 11:47:29
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:52:36
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false)
{
    ep = new Epoll();
}

EventLoop::~EventLoop()
{
    delete ep;
}

void EventLoop::loop()
{
    while (!quit)
    {
        std::vector<Channel *> chs;
        chs = ep->poll();
        for (auto it = chs.begin(); it != chs.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch)
{
    ep->updateChannel(ch);
}