/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:16:32
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:53:25
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once

class InetAddress;
class Socket
{
private:
    int fd;

public:
    Socket();
    Socket(int _fd);
    ~Socket();

    void bind(InetAddress *);
    void listen();
    void setnonblocking();

    int accept(InetAddress *);

    int getFd();
};
