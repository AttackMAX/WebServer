/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-04 01:05:03
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:54:02
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "src/EventLoop.h"
#include "src/Server.h"

int main()
{
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();
    return 0;
}