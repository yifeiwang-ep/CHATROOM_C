/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-17 15:53
 * Filename :        shan.c
 * Description :
 * *****************************************************************************/

#define ACCOUNTSIZE 100  //最大连接数量
#define FRIENDSIZE 50    //最大好友数
#define BUFSIZE 225    //最大缓冲区
#define GROUPSIZE 50 //最大群规模
#define GROUPCOUNT 20 //最大群数量


typedef struct{
    char username[32];
    char usersex[10];
    char status;
}friend_info_t;

typedef struct{
    char groupname[32];
    char owner[32];
    int groupid;
    int size;
    friend_info_t member[GROUPSIZE];
}group_info_t;

typedef struct {
    int year;
    int month;
    int day;
}user_date_t;

typedef struct{
    int hour;
    int minute;
    int second;
}user_time_t;

typedef struct{
    user_date_t date;
    user_time_t time;
    char send_name[32];
    char from_name[32];
    char message_buf[BUFSIZE-36];

}message_t;

typedef struct{
    char filename[32];
    char filepath[BUFSIZE];
    int count;
    int lenth;
}fileinfo_message_t;

typedef struct{

    int conn_fd;
    int flag_recv;
}thread_t;

typedef struct {

    user_date_t date;
    user_time_t time;
}user_logtime_t;


typedef struct{

	char markname[32];
	int filelen;
	int sendlen;
	int givelen;
	int leftlen;

}fileinfo_t;


typedef struct{
    char filepath[32];
    char markname[32];
    int filelen;
    int sendlen;
    int givelen;
    int sendtimes;
    int leftlen;
    char buf[BUFSIZE];
}file_t;



typedef struct{
    int userid;
    char username[32];
    char userpasswd[32];
    char usersex[10];
    char leavemes_flag;
    char status;
    int friendslist[FRIENDSIZE];
    int grouplist[GROUPCOUNT];
    int conn_fd;
    user_logtime_t ontime;
    user_logtime_t offtime;
}userinfo_service_t;


typedef struct{
    userinfo_service_t userinfo;
    friend_info_t friendinfo;
    group_info_t group_info;
    user_date_t data;
    user_time_t time;
    message_t message;
    fileinfo_message_t messageinfo;
    fileinfo_t file;
    int conn_fd;
    char check_answer;
    int input_check;
    char userstatus;
    int command_type;
    char buf[BUFSIZE];
}send_t;



#include<stdio.h>
#include<unistd.h>
#include<errno.h>

int main(int argc, char *argv[])
{
    send_t hh;

    int i;
    i = sizeof(hh);
    printf("%d f\n", i);

    return 0;
}
