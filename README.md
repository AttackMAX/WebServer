<!--
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-04 01:05:03
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 18:46:49
 * 
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved. 
-->
## 进度汇总

### 截止 2024/12/3

> 通过几个简单的socket，完成了服务器和客户端的连接请求。

### 截止 2024/12/4

> 设置了基本的错误处理，完成了服务器和客户端信息传输

### 截止 2024/12/5
> 通过epoll实现了高并发。

> 引入OOP思想，初步封装了Scocket,netAddress和Epoll，隐藏了底层语言实现的细节。

### 截止2024/12/6
> 初步将服务器改造成Reactor模式

> 分离接受连接这一模块，添加一个Acceptor类

> 将tcp连接封装成类Connetction 

于此一个完整的单线程服务器设计模式已经完成，服务器工作流程如下

服务器工作流程
初始化:

main() 创建EventLoop和Server实例。
Server 初始化Acceptor，绑定地址并开始监听。
事件循环:

EventLoop::loop() 进入循环，调用Epoll::poll()等待事件。
接受新连接:

当有新连接到达时，Acceptor 的回调 acceptConnection() 被触发。
创建新的Socket和Connection实例，并将其管理在Server中。
处理通信:

每个Connection拥有一个Channel，当有数据可读时，echo() 方法被调用。
echo() 从客户端读取数据并回写，实现回显功能。
事件分发:

Epoll 根据活动事件，调用对应Channel的handleEvent()，进而执行回调函数。
关闭连接:

当客户端断开或发生错误，Connection 调用deleteConnectionCallback，由Server移除并删除连接。
整个服务器基于事件驱动，使用非阻塞I/O和epoll高效管理大量并发连接，通过EventLoop统一调度事件，确保高性能和可扩展性。


[UML类图查询](pic/UML_day1-8.png)