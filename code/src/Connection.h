/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 13:16:05
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:52:02
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Connection
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    std::function<void(Socket *)> deleteConnectionCallback;

public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket *)>);
};
