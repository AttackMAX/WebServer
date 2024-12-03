
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 定义服务器地址结构
    struct sockaddr_in serve_addr;
    bzero(&serve_addr, sizeof(serve_addr));              // 将服务器地址结构清零
    serve_addr.sin_family = AF_INET;                     // 设置地址族为IPv4
    serve_addr.sin_port = htons(8888);                   // 设置端口号为8888，使用网络字节序
    serve_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置IP地址为127.0.0.1

    // 连接到服务器
    connect(sockfd, (sockaddr *)&serve_addr, sizeof(serve_addr));

    return 0;
}