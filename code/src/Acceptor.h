/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 12:41:22
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:50:15
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Acceptor
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *acceptChannel;
    std::function<void(Socket *)> newConnectionCallback;

public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket *)>);
};
