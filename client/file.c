#include"client.h"

int file_ui_init(int conn_fd, char* username)
{
    char choice;
    send_t send_buf;
    send_t *psend = &send_buf;

    do{
        print();
        puts(" ");
        puts("\033[33mwhat do you want?\033[0m");
        puts("downloads");
        puts("upload");
        puts("return");
        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        choice = l_getc();

        switch(choice){

            case 'u':
            case 'U':
                upload_file(conn_fd, username);
                break;
            case 'd':
            case 'D':
                cl_stdin();
                puts("input the file markname");
                printf("markname:");
                input_check(32, psend->file.markname);
                puts(" ");

                psend->command_type = _DOWNLOAD;
                psend->conn_fd = conn_fd;
                strcpy(psend->userinfo.username, username);
                send_data(psend->conn_fd, psend);
                break;
        }
    }while(choice != 'r' && choice != 'R');
    return 0;
}

int upload_file(int conn_fd, char* username){

    int fd;
    char *temp_buf;
    file_t file_buf;
    file_t *pfile = &file_buf;
    send_t send_buf;
    send_t *psend = &send_buf;

    temp_buf = (char*) malloc (1024*1024);

    print();
    puts("input file name with absolute path");
    printf("filepath:");
    cl_stdin();
    input_check(32, pfile->filepath);

    fd = open(pfile->filepath, O_RDWR );
    if(fd < 0){
        puts("wrong path or file not exist");
        return 0;
    }

    //get the file information , size or ...
    pfile->filelen = file_len(fd);
    pfile->leftlen = file_len(fd);
    pfile->givelen = 0;

    file_read(fd, pfile->filelen, temp_buf);

    puts("");
    puts("<system>：file information below");
    printf("filepath:\033[33m%s\033[0m\n", pfile->filepath);
    printf("size:\033[33m%d\033[0mB\n\n", pfile->filelen);

    if(pfile->filelen < 0){
        my_err("<\033[31msystem\033[0m>:wrong path or file not exist", __LINE__);
    }
    else{
        puts("set your file a markname for confidential");
        printf("markname:");
        input_check(32, pfile->markname);
        //copy file into send_buf;

        psend->userstatus = LOADSTART;

        print();
        while(1){

            strcpy(psend->file.markname, pfile->markname);
            psend->file.filelen = pfile->filelen;
            psend->command_type = _UPLOAD;

            psend->conn_fd = conn_fd;
            strcpy(psend->userinfo.username, username);

            //start upload
            if(psend->userstatus == LOADSTART){

                pfile->sendlen = send_data(conn_fd,psend);
                //change flag
                psend->userstatus = VALID_USERINFO;

                if(pfile->sendlen < 0){
                    my_err("upload faild", __LINE__);
                }
                sleep(1);
                continue;
            }

            //no means
            if((pfile->leftlen <= 0) || (pfile->givelen >= pfile->filelen)){
                break;
            }

            //when not the last time
            if(pfile->leftlen >= BUFSIZE){
                psend->file.givelen = pfile->givelen;
                memcpy(psend->buf, temp_buf + pfile->givelen, BUFSIZE);
                psend->file.sendlen = BUFSIZE;
                pfile->sendlen = send_data(conn_fd, psend);

                if(pfile->sendlen <= 0){
                    my_err("upload faild", __LINE__);
                    //can make mark;
                }
            }
            else{
                memcpy(psend->buf, temp_buf+pfile->givelen, pfile->leftlen);
                psend->userstatus = LOADOVER;
                psend->file.givelen = pfile->givelen + pfile->leftlen;
                psend->file.sendlen = pfile->leftlen;
                pfile->sendlen = send_data(conn_fd, psend);

                if(pfile->sendlen <= 0){
                    my_err("upload faild", __LINE__);
                }
                pfile->givelen = pfile->givelen + pfile->leftlen;

                break;
            }

            if(pfile->givelen < pfile->filelen ){
                //设置光标位置
                //清除从光标到行尾的内容
                printf("sended \033[33m%d\033[0mB , filelen: \033[33m%d\033[0mB\n", pfile->givelen, pfile->filelen);
                printf("\033[1A");
                printf("\033[K");
            }

            usleep(100000);
            pfile->givelen += BUFSIZE;
            pfile->leftlen = pfile->filelen - pfile->givelen;

        }//while

        close(fd);
        free(temp_buf);
        if(pfile->givelen == pfile->filelen){

            puts("100%");
            puts("upload over");
            return 0;
        }
        else{
            my_err("upload faild", __LINE__);
        }
    }//else
    return 0;
}

int download_file(send_t *precv){

    int ret;
    int fd;
    file_t file_buf;
    file_t *pfile = &file_buf;

    //maskname 判断
    if(precv->userstatus == _LOADERR){
        puts("wrong markname");
        return 0;
    }

    fd = open(precv->file.markname, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP);
    if(fd < 0){
        my_err("file_open", __LINE__);
    }
    // 开始标志
    if(precv->userstatus == LOADSTART){
        puts("the file information below:");
        printf("name:\033[33m%s\033[0m\n", precv->file.markname);
        printf("size:\033[33m%dB\033[0m\n", precv->file.filelen);
        puts("start recving the file...");
        puts(" ");
    }else{

        ret = file_write(fd, precv->buf, precv->file.sendlen );
        pfile->givelen = file_len(fd);
        printf("recvlen:\033[33m%d\033[0mB  filelen:\033[33m%d\033[0m\n", pfile->givelen, precv->file.filelen);
        printf("\033[1A");
        printf("\033[K");

        if(precv->userstatus == LOADOVER){

            //
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
