/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 11:47:41
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 12:57:16
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once

class EventLoop;
class Socket;
class Acceptor;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;

public:
    Server(EventLoop *);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};
