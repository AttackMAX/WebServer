/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-06 11:47:42
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:53:05
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr)
{
    acceptor = new Acceptor(loop);
    std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
    delete acceptor;
}

void Server::newConnection(Socket *sock)
{
    Connection *conn = new Connection(loop, sock);
    std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = conn;
}

void Server::deleteConnection(Socket *sock)
{
    Connection *conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}