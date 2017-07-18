/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-09 10:52
 * Filename :        service.h
 * Description :
 * *****************************************************************************/
#ifndef SERVICE_H
#define SERVICE_H


#include"../common/common.h"
#include "list.h"


typedef struct {
	char entyName[32];
	long key;
} entity_key_t;

typedef struct entity_key_node{
	entity_key_t data;
	struct entity_key_node *prev;
	struct entity_key_node *next;
}entkey_node_t, *entkey_list_t;


int *servive(thread_t *data);

int serve_user_login(send_t *precv);  //登录函数

int serve_user_logoff(send_t *precv); //下线函数

int serve_user_register(send_t *precv);  //注册函数

int serve_user_update(send_t *precv);  //基本信息更改函数

int serve_user_delet(send_t *precv); //清楚用户

int server_message_leave(int conn_df);   //用户查看离线消息

int server_friend_delet(send_t * precv);

int server_friend_show(send_t * precv);   //用户查看好友列表

int server_friend_combain(send_t *precv);

int server_friend_add(send_t *precv);   //用户添加好友请求

int server_group_show(int conn_fd);   //show group list

int server_friend_find(send_t * precv);

int server_friend_send(send_t *precv);

int server_group_add(int conn_fd);  //add group &search group

int server_message_temp(int conn_fd);   //show uncheck message since user sign in

int server_chat_one(send_t *recv, int type);        //chat one to one

int server_chat_group(int conn_fd);             //chat to group

int server_group_owner(int conn_fd);            //build a group

int server_file_check(char * filename);         //check file exit, or creat a new file;:w

int server_file_select(char *filename, fileinfo_message_t * fdata); //init pointer fdata by filename

int server_message_selet(char * filename, message_t *data); //read message by filename

int server_group_selet(int groupid, group_info_t *data);

int userinfo_p_checkfile(const char * username); //检查文件是否存在

int message_p_update(message_t * data);

int userinfo_p_update(userinfo_service_t *data); //修改用户信息

int message_tip(message_t *mes);

int message_p_insert(message_t *data);

int userinfo_p_insert(const userinfo_service_t *data);  //写入用户信息

int userinfo_p_delet(int id); //注销用户

int userinfo_p_deletid(int id);

int userinfo_p_selectall(userinfo_service_t users[]);//get all user infomation

int creat_userdir(const char *username); //创建用户文件

int server_name_check(char *username);

int userinfo_p_selet(int id, userinfo_service_t *data);  //获取用户信息

int userinfo_p_seletname(const char *username, userinfo_service_t *data); //用户名获取用户信息

int leave_p_insert(send_t *precv);

//int message_p_select(const char *mesfilename);

inline long EntKey_Srv_CompNewKey(char entName[]);   //根据传入的实体名entName，为新实体分配一个唯一的主键。函数返回值为取新实体的主键值

inline long EntKey_Srv_CompNewKeys(char entName[], int count);    //根据传入的实体名entName及实体个数count，为这个count个新实体分配一个长度为count的主键值区间，使得每个新实体在该区间内都可以分配到 唯一的 主键。返回值为该主键区间的最小值

inline void EntKey_Srv_Add2List(entkey_list_t keyList, long key);   //将主键key保存到主键链表keyList中

inline int EntKey_Srv_CheckExist(entkey_list_t keyList, long key);    //在主键链表keyList中检查key是否存在，返回1存在，否则0

long EntKey_Perst_GetNewKeys(char entName[], int count);    //根据传入的实体名entName及实体个数count，为这个count个新实体分配一个长度为count的主键值区间，使得每个新实体在该区间内都可以分配到 唯一的 主键。返回值为该主键区间的最小值

int file_upload(send_t *precv);

int file_download(send_t *precv);

#endif
