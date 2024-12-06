/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:22:22
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:52:58
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#pragma once
#include <arpa/inet.h>

class InetAddress
{
private:
    struct sockaddr_in addr;
    socklen_t addr_len;

public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();
    void setInetAddr(sockaddr_in _addr, socklen_t _addr_len);
    sockaddr_in getAddr();
    socklen_t getAddr_len();
};
