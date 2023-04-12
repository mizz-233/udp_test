/*===============================================
*   文件名称：udps.c
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
#include <fcntl.h>
#include <wait.h>
#include <time.h>
int main(int argc, char *argv[])
{ 
    int sp=socket(AF_INET,SOCK_DGRAM,0);
    if(sp<0)
    {
        perror("socket");
        exit(-1);
    }
    printf("socket success\n");
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(8888);
    saddr.sin_addr.s_addr=inet_addr("192.168.12.167");
    int s_len=sizeof(saddr);
    int ret=bind(sp,(struct sockaddr*)&saddr,s_len);
    if(ret<0)
    {
        perror("bind");
        exit(-1);
    }
    printf("bind success\n");
    struct sockaddr_in caddr;
    memset(&caddr,0,sizeof(caddr));
    int c_len=sizeof(caddr);
    time_t tm;
    while(1)
    {
        char buf[64]={0};
        while(1)
        { 
            memset(buf, 0, 64);
            ret = recvfrom(sp,buf,64,0,(struct sockaddr*)&caddr,&c_len);
            if(ret < 0)
            {
                perror("recvfrom");
                exit(-1);
            }
            if(strcmp(buf,"time\n")==0)
            {
                memset(buf,0,64);
                time(&tm);
                strcpy(buf,ctime(&tm));
                ret=sendto(sp,buf,strlen(buf),0,(struct sockaddr*)&caddr,c_len);
            }
            printf("recv %dbytes: %s\nip-->%sport-->%d\n", ret, buf,inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));
        }
    }
    
    close(sp);
    return 0;
} 

