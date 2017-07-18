/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-03 20:31
 * Filename :        service.c
 * Description :
 * *****************************************************************************/
#include "service.h"
#define EPOLL_MAX 1024

int *service(send_t *precv){

    switch(precv->command_type){

        case _USERADD:

            serve_user_register(precv);

            break;
        case _USERCHA:

            serve_user_update(precv);
            break;
        case _USERDEL:

            serve_user_delet(precv);
            break;
        case _USERONL:

            serve_user_login(precv);

            break;
        case _USEROFFL:

            serve_user_logoff(precv);
            break;
        case _FRIENDLIST:

            server_friend_show(precv);

            break;
        case _FRIENDDEL:

            server_friend_delet(precv);

            break;
        case _USERSALL:
            server_friend_add(precv);

            break;
        case _UPLOAD:

            file_upload(precv);

            break;
        case _DOWNLOAD:

            file_download(precv);

            break;
        case _FRIENDSEND:
            server_friend_send(precv);
            break;

        case _FRIENDFIND:

            server_friend_find(precv);
            break;
        case _FRIENDASK:

            server_friend_combain(precv);
            break;

        case _CHATONE:
            server_chat_one(precv, _CHATONE);
            break;
            /*   case _GROUPLIST:
                 server_group_show(conn_fd);
                 break;
                 case _GROUPSEND:
                 server_group_add(conn_fd);
                 break;
                 case _CHATONE:
                 server_chat_one(conn_fd);   //
                 break;
                 case _CHAT:
                 server_message_leave(conn_fd);
                 break;
                 case 27:
                 server_message_temp(conn_fd);
                 break;
                 case 28:
                 server_chat_group(conn_fd); //
                 break;*/
        default:
            break;
    }
    pthread_exit(0);
    return NULL;
}

int main(){

    int num = 0;
    struct sockaddr_in cli_addr, serv_addr;
    struct epoll_event ev, events[EPOLL_MAX];
    char flag_recv = _ONLINE;
    int sock_fd, conn_fd;
    int epfd, nfds;
    int i;
    int optval;

    send_t recv_b;
    send_t *precv = &recv_b;
    pthread_t id;

    socklen_t cli_len;


    epfd = epoll_create(EPOLL_MAX);

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


    //套接字加入epoll
    ev.data.fd = sock_fd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &ev);


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

    system("clear");
    puts("service is working...");

    while(1){

        //设置永久阻塞
        nfds = (epoll_wait(epfd, events, 1, -1));

        if(nfds < 0){
            break;
        }

        for(i = 0; i < nfds; i++){
            //套接字描述符改变_连接请求
            if(events[i].data.fd == sock_fd){

                //通过accept接受客户端的连接请求，并返回连接套接字用于手法数据
                if((conn_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &cli_len)) == -1){
                    exit(0);
                    //my_err("accept", __LINE__);
                }

                printf("client from [IP:%s] is working.%d\n\n",inet_ntoa(cli_addr.sin_addr),num);
                num++;

                //初始化客户端fd
                ev.data.fd = conn_fd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);

            }

            //有读事件发生
            else if(events[i].events & EPOLLIN){
                int fd;
                if(recv_data(events[i].data.fd, precv) <= 0 || precv->command_type == _USEROFFL){

                    printf("\n\nclient: %d disconnect\n", events[i].data.fd);
                    //strcpy(precv->friendinfo.username, username);
                    precv->conn_fd = events[i].data.fd;
                    fd = events[i].data.fd;
                    serve_user_logoff(precv);

                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
                    close(fd);

                    continue;
                }

                precv->conn_fd = events[i].data.fd;
                fd = events[i].data.fd;
                //线程参数结构体赋值

                //下线则解除
                void *flag ;
                pthread_create(&id, NULL,(void*)service,precv);//新开线程去处理事件*/
                pthread_join(id,&flag);
            }
        }
    }

    close (epfd);
    shutdown(sock_fd, SHUT_RD);
    return 0;
}

int file_upload(send_t * precv){

    int ret;
    int fd;
    file_t file_buf;
    file_t *pfile = &file_buf;

    fd = open(precv->file.markname, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP);
    if(fd < 0){
        my_err("file_open", __LINE__);
    }    // 开始标志

    if(precv->userstatus == LOADSTART){
        printf("user start recving file :%s\n", precv->file.markname);
        printf("size of the file: %dB\n", precv->file.filelen);
    }else{

        ret = file_write(fd, precv->buf, precv->file.sendlen);
        pfile->givelen = file_len(fd);

        //  printf("recvlen : %dB\n\n", precv->file.givelen);
        //
        if(precv->userstatus == LOADOVER){
            if(pfile->givelen != precv->file.givelen){
                my_err("recv_len",__LINE__);
            }


            if(pfile->givelen == precv->file.filelen){
                puts("download over");
            }
            else{
                my_err("download failed", __LINE__);
            }
        }
    }
    close(fd);
    return 0;
}

int file_download(send_t* precv){

    int fd;
    file_t file_buf;
    file_t *pfile = &file_buf;
    send_t send_buf;
    send_t *psend = &send_buf;
    char *temp_buf;
    chdir("/home/");

    temp_buf = (char*)malloc(1024*1024);
    char path[32] = {"/home/"};

    strcat(path, precv->file.markname);
    psend->command_type = _DOWNLOAD;
    psend->conn_fd = precv->conn_fd;

    fd = open(path, O_RDWR );
    if(fd < 0){
        psend->userstatus = LOADWRONG;
        send_data(psend->conn_fd, psend);
        my_err("open_wrong", __LINE__);
    }

    //get the file information , size or ...
    //copy file into send_buf;
    pfile->filelen = file_len(fd);
    pfile->leftlen = pfile->filelen;
    pfile->givelen = 0;

    file_read(fd, pfile->filelen, temp_buf);
    puts("");
    puts("<system>:file send to user");
    printf("markname:%s\n", precv->file.markname);
    printf("size:%dB\n", pfile->filelen);
    puts("");
    psend->userstatus = LOADSTART;

    while(1){
        chdir("/home/");

        strcpy(psend->file.markname, precv->file.markname);
        psend->file.filelen = pfile->filelen;
        psend->command_type = _DOWNLOAD;
        psend->conn_fd = precv->conn_fd;

        if(psend->userstatus == LOADSTART){
            pfile->sendlen = send_data(psend->conn_fd,psend);

            if(pfile->sendlen < 0){
                my_err("download_file", __LINE__);
            }
            sleep(1);
            continue;
        }

        if((pfile->leftlen <= 0) || (pfile->givelen >= pfile->filelen)){
            break;
        }

        if(pfile->leftlen >= BUFSIZE){
            psend->file.givelen = pfile->givelen;
            memcpy(psend->buf, temp_buf+pfile->givelen, BUFSIZE);
            psend->file.sendlen = BUFSIZE;
            pfile->sendlen = send_data(precv->conn_fd, psend);

            if(pfile->sendlen <= 0){
                my_err("download_file", __LINE__);
                //can make mark;
            }
        }
        else{
            memcpy(psend->buf, temp_buf+pfile->givelen, pfile->leftlen);
            psend->userstatus = LOADOVER;
            psend->file.givelen = pfile->givelen + pfile->leftlen;
            psend->file.sendlen = pfile->leftlen;
            pfile->sendlen = send_data(precv->conn_fd, psend);

            if(pfile->sendlen <= 0){
                my_err("download_failed", __LINE__);
            }

            pfile->givelen = pfile->givelen + pfile->leftlen;

            break;
        }

        usleep(100000);

        pfile->givelen += BUFSIZE;
        pfile->leftlen = pfile->filelen - pfile->givelen;
    }//while

    close(fd);
    free(temp_buf);

    if(pfile->givelen == pfile->filelen){
        printf("user recved %dB, file len %dB\n", pfile->filelen, pfile->filelen);
        puts("user download over");
        return 0;
    }
    else{
        my_err("upload failed", __LINE__);
    }
    return 0;
}
