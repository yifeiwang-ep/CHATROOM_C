/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-05 09:10
 * Filename :        common.h
 * Description :
 * *****************************************************************************/
#ifndef COMMON_H_
#define COMMON_H_

#include<stdio.h>
#include<sys/stat.h>
#include<dirent.h>
#include<assert.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<pthread.h>
#include<netinet/in.h>
#include<time.h>
#include <sys/time.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"struct.h"

int file_write(int fd, const void * data, int len); //写入文件

int file_read(int fd, int len, void * data); //读出文件

int file_len(int fd);// 获得文件长度

void my_err(const char * err_string, int line);

int recv_data(int conn_fd, send_t *precv);

char l_getc(); //从键盘获取一个字符，避免回车

void cl_stdin(); //清空输入缓冲

void getpwd(int maxlen, char *pwd);

int input_check(int input_len, char *input_buf); //输入合法性检查

void show_recv(int conn_fd, char * recv_buf, int buflen);  //显示接收消息

user_date_t DateNow();  //获取当前日期

user_time_t TimeNow();  //获取当前时间

char* Str2Upper(char * entUpperName );  //字符串转化

int ser_atoi(char *buf); //将字符转化为int

int send_data(int conn_fd, send_t * psend); //发送数据

void send_message(int conn_fd, char * send_name); //send message,need servive answer

void show_message(int conn_fd);       //recvie and show message

void recv_message(int conn_fd, message_t * data);

#endif

