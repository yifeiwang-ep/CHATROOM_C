/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-05 09:46
 * Filename :        account_per.c
 * Description :
 * *****************************************************************************/
#include "client.h"

//检查用户文件是否存在
int userinfo_p_checkfile(const char *username){

    chdir("/home/wyf/");

    if(access(username, 0) == 0){
        return 1;
    }
    else{
        return -1;
    }
}

int creat_file(const char * string){
    int fd;

    if((fd = open(string, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP )) < 0){
        my_err("file_creat", __LINE__);
    }

    return 0;
}


//创建用户文件
int creat_userdir(const char *username){
    int ret = 0;
    int fd;
    DIR * dir;
    char pathname[255];

    strcpy(pathname, "/home/wyf/");
    strcat(pathname, username);
    strcat(pathname, "/");

    chdir("/home/wyf/");

    if((fd = mkdir(username, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) < 0){

        my_err("creat_info",__LINE__);
    }

    dir = opendir(username);
    if(dir == NULL){
        my_err("dir", __LINE__);
    }

    chdir(pathname);

    if((fd = open(username, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP )) < 0){
        my_err("creat_info",__LINE__);
    }
    close(fd);
    if((fd = open("message", O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP )) < 0){
        my_err("creat_message",__LINE__);
    }

    close(fd);
    closedir(dir);
    ret = 1;

    return ret;
}

//写入用户信息
int userinfo_p_insert(const userinfo_t * data){
    int ret = 0;
    int fd;
    int len;

    my_chdir();
    fd = open(data->username, O_RDWR | O_TRUNC );
    if(fd < 0){
        my_err("open_p", __LINE__);
    }
    len = sizeof(userinfo_t);
    ret = file_write(fd, data, len);
    close(fd);

    return ret;
}

//根据用户id获取用户信息并通过data传出

int userinfo_p_seletid(int userid, userinfo_t * data){

    FILE *fp;
    int found = 0;
    userinfo_t buf;
    fp = fopen("USER","rb");
    if(fp == NULL){
        return 0;
    }
    while(!feof(fp)){
        if(fread(&buf,sizeof(userinfo_t),1,fp)){
                data = &buf;
                found = 1;
                break;
        }
    }
    fclose(fp);

   return found;
}


//根据用户名获取用户信息并通过data传出

int userinfo_p_seletname(const char* username, userinfo_t * data){

    FILE *fp;
    int found = 0;
    userinfo_t buf;
    fp = fopen("USER","rb");
    if(fp == NULL){
        return 0;
    }
    while(!feof(fp)){
        if(fread(&buf,sizeof(userinfo_service_t),1,fp)){
            if(strcmp(buf.username,username) == 0){
                data = &buf;
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}



int userinfo_p_delet(const char * username){
    FILE * fp;
    int ret = 0;

    fp = fopen(username,"rb+");
    if(fp == NULL){
        my_err("delet_p", __LINE__);
    }
    fclose(fp);
    if(unlink(username) < 0){
        my_err("unlink_p",__LINE__);
    }
    ret = 1;
    return ret;
}


