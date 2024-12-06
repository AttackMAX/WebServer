/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 11:47:41
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:53:12
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once

#include <map>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection *> connections;

public:
    Server(EventLoop *);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
};
