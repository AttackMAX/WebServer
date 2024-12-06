/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 00:35:36
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 12:08:32
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {}

void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

void Channel::handleEvent()
{
    callback();
}

int Channel::getFd()
{
    return fd;
}

uint32_t Channel::getEvents()
{
    return events;
}
uint32_t Channel::getRevents()
{
    return revents;
}

bool Channel::getInEpoll()
{
    return inEpoll;
}

void Channel::setInEpoll()
{
    inEpoll = true;
}

void Channel::setRevents(uint32_t _ev)
{
    revents = _ev;
}

void Channel::setCallback(std::function<void()> _cb)
{
    callback = _cb;
}