/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-05 15:25
 * Filename :        client.h
 * Description :
 * *****************************************************************************/
#ifndef CLIENT_H
#define CLIENT_H

#include "../common/common.h"

typedef enum{
    USER_FEMALE = 1,
    USER_MAN = 2
}userinfo_sex_t;

typedef enum{
    LEAVE_MESSAGE_IN = 1,
    LEAVE_MESSAGE_NO = 2
}userinfo_leavemes_t;


typedef struct{
    char type;
    char username[32];
    char userpasswd[32];
    char path[32];
    userinfo_sex_t sex;
    userinfo_leavemes_t flag_leave;
}userinfo_t;

void * client_t(void * fd);     //thread function

int user_init_(int conn_fd);        //menu

int Scanfkeyboard();    //直接读取键盘输入

int userinfo_s_add(userinfo_t *data );  //注册用

int userinfo_s_delet(const char *username);

int userinfo_s_select(const char *usestring, userinfo_t *data);

int userinfo_s_updata(userinfo_t *data, const char *username);

int userinfo_s_login(const char *username);  //登录文件的检查的创建

int message_tips(send_t *precv);

int *thread_fun(int conn_fd);

int signin_ui(int conn_fd);

int pthread_recv(int conn_fd);

void input_userinfo(int conn_fd, const char *string);

void userinfo_ui_init(int conn_fd, char *username); //登录后界面

void account_ui_init(int conn_fd, char* username);

void account_ui_update(int conn_fd,char *username);

void account_ui_delete(int conn_fd,char *username);

void account_ui_logoff(int conn_fd, char *username);

int userinfo_p_checkfile(const char * username); //检查文件是否存在

int userinfo_p_update(const userinfo_t *data); //修改用户信息

int userinfo_p_insert(const userinfo_t *data);  //写入用户信息

int userinfo_p_delet(const char *username); //注销用户

int creat_userdir(const char *username); //创建用户文件

int userinfo_p_seletid(int userid, userinfo_t *data);  //获取用户信息

int userinfo_p_seletname(const char *username, userinfo_t *data); //用户名获取用户信息
//int message_p_select(const char *mesfilename);
int message_p_insert(message_t * mes);

int message_p_update(message_t * mes);

int message_p_all(message_t *box, int type);

int message_p_selet(int type, message_t *mes);

int print();

int friend_chat_one(int conn_fd, char * username);

int user_chat_one(send_t *precv);

int message_init(int conn_fd, char * username);

int message_store();

int message_box(int conn_fd, char *username, int type);

int chat_user_init(int conn_fd, char *username);

int chat_user_one(int conn_fd, char *username, char *name);

int chat_user_group(int conn_fd, char *username, char * name);

int chat_friend_ask(int conn_fd, char *username);

int friend_chat_ask(send_t * precv);

int chat_friend_show(send_t *precv);

int friend_chat_show(int conn_fd, char *username);

int chat_friend_del(send_t *precv);

int chat_group_show(send_t *precv);

int group_chat_show(int conn_fd, char *username);

int chat_stranger_show(int conn_fd, char *username);

int chat_friend_add(send_t *precv);

int chat_group_add(send_t *precv);

int friend_chat_send(int conn_fd, char* username);

int friend_chat_add(int conn_fd, char *username);

int file_ui_init(int conn_fd, char * username);

int download_file(send_t *precv);

int upload_file(int conn_fd, char *name);

int group_chat_add(int conn_fd, char *username);

int group_find_add(send_t *precv);

int friend_find_add(send_t *precv);

void chat_leave_check();

void client(send_t *precv);

void my_chdir();


#endif
