/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-05 21:26:40
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:52:51
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "InetAddress.h"
#include <string.h>
InetAddress::InetAddress() : addr_len(sizeof(addr))
{
    bzero(&addr, sizeof(addr));
}
InetAddress::InetAddress(const char *ip, uint16_t port) : addr_len(sizeof(addr))
{
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr_len = sizeof(addr);
}

InetAddress::~InetAddress()
{
}

void InetAddress::setInetAddr(sockaddr_in _addr, socklen_t _addr_len)
{
    addr = _addr;
    addr_len = _addr_len;
}

sockaddr_in InetAddress::getAddr()
{
    return addr;
}
socklen_t InetAddress::getAddr_len()
{
    return addr_len;
}