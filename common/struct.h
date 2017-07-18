/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-13 21:12
 * Filename :        struct.h
 * Description :
 * *****************************************************************************/


#define ACCOUNTSIZE 1000  //最大连接数量
#define FRIENDSIZE 50    //最大好友数
#define BUFSIZE 1024    //最大缓冲区
#define GROUPSIZE 50 //最大群规模
#define GROUPCOUNT 20 //最大群数量

#define INVALID_USERINFO 'n'   //无效信息
#define VALID_USERINFO 'y'           // 有效信息


//用户操作类型

#define _USERADD 11         //用户注册
#define _USERCHA 12         //更改用户基本信息
#define _USERDEL 13         //删除用户
#define _USERONL 14          //上线操作
#define _USEROFFL 15        //下线操作

#define _LEAVEMES 's'           //有离线消息
#define _NOMESSAGE 'e'          //无离线消息
#define _ONLINE 'o'             //为上线状态
#define _OFFLINE 'd'            //为离线状态

#define _FRIENDLIST 21          //用户好友列表
#define _FRIENDFIND 22           //查找用户
#define _FRIENDASK 23          //好友请求
#define _FRIENDSEND 24          //add friend
#define _FRIENDDEL 25      //delet friend
#define _USERSALL 26

#define _GROUPLIST 31          //用户群列表
#define _GROUPFIND 32           //查找群
#define _GROUPOWN  33            //creat a group
#define _GROUPSEND 33       //add group
#define _GROUPDEL  34              //group quit
#define _GROUPDISMISS 35         //group dismiss
#define _GROUPSALL 36

#define _RECALL  99              //返回到主目录
#define _SYSTEMTIP 91               //system tip
#define _DENY 92

#define _INPUTNAME 41
#define _INPUTPASSWD 42
#define _INPUTSEX 43
#define _INPUTFRIEND 44
#define _INPUTGROUP 45

#define _CHATGRO 52
#define _CHATONE 51
#define _LEAVEMESS 53           //查看离线消息
#define _MESSTIP 54

#define LOADSTART 't'
#define LOADOVER 'r'
#define LOADWRONG 'g'

#define _UPLOAD 61
#define _DOWNLOAD 62
#define _LOADERR 63

#define LISTENQ 1024
#define SERV_PORT 4507

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
    int type;
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

