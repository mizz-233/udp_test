/*===============================================
*   文件名称：udpc.c
*   创 建 者：     
*   创建日期：2023年04月12日
*   描    述：
================================================*/
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    //1、创建套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(-1);
    }
    printf("socket success!\n");

    //2、向服务器发起连接
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.s_addr = inet_addr("192.168.12.167");

    int s_len = sizeof(saddr);
    //发送数据
    char buf[64] = {0};
    while(1)
    {
        fgets(buf, 64, stdin);
        int ret = sendto(sockfd, buf, strlen(buf),0,(struct sockaddr*)&saddr,s_len);
        if(ret < 0)
        {
            perror("sendto");
            exit(-1);
        }
        memset(buf,0,64);
        ret=recvfrom(sockfd,buf,64,0,NULL,NULL);
        if(ret<0)
        {
            perror("recvfrom");
            exit(-1);
        }
        printf("%s\n",buf);
    }
    close(sockfd);
    return 0;
}
