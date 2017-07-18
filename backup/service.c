/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-03 20:31
 * Filename :        service.c
 * Description :
 * *****************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include"../common/common.h"
#include"service.h"

#define SERV_PORT 4507
#define LISTENQ 12
#define INVALID_USERINFO 'n'
#define VALID_USERINFO 'y'
#define USERNAME 0
#define PASSWORD 1


int main(int argc, char *argv[]){
    int sock_fd, conn_fd;
    int optval;
    int flag_recv = USERNAME;
    int ret;
    pid_t pid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;
    char recv_buf[BUFSIZE];

    //创建一个TCP套接字
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(sock_fd < 0){
        my_err("socket", __LINE__);
    }

    //设置该套接字使之可以重新绑定端口
    optval = 1;
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(int)) < 0){
        my_err("setsokopt", __LINE__);
    }

    //初始化服务器端地址
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //将套接字绑定到本地端口
    if(bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)) < 0){
        my_err("bind",__LINE__);
    }

    //将套接字转化为监听套接字
    if(listen(sock_fd, LISTENQ) < 0){
        my_err("listen",__LINE__);
    }

    cli_len = sizeof(struct sockaddr_in);

    while(1){

        //通过accept接受客户端的连接请求，并返回连接套接字用于手法数据
        conn_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &cli_len);
        if(conn_fd < 0){
            my_err("accept", __LINE__);
        }

        printf("IP:%s is accepted.\n",inet_ntoa(cli_addr.sin_addr));

        //创建一个子进程处理请求
        if((pid = fork()) == 0){

            while(1){
                my_recv(conn_fd, recv_buf, sizeof(recv_buf));

                flag_recv = ser_atoi(recv_buf);
                switch(flag_recv){
                    case 11:
                        serve_user_register(conn_fd);
                        break;
                    case 12:
                        serve_user_update(conn_fd);
                        break;
                    case 13:
                        serve_user_delet(conn_fd);
                        break;
                    case 14:
                        serve_user_login(conn_fd);
                        break;
                    case 15:
                        serve_user_logoff(conn_fd);
                        break;
                    case 21:;
                    case 22:;
                    case 23:;
                    case 24:;
                    case 25:;
                }
            }//while

            close(sock_fd);
            close(conn_fd);
            exit(0); //子进程

        }

        else{
            close(conn_fd);
        }
    }
    return 0;
}

