/******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-09 15:10
 * Filename :        account.c
 * Description :
 * *****************************************************************************/

#include"service.h"

#define USER_FILE "/home/"

//在账号文件中删除与参数id匹配的账号，删除成功 return 1；否则 return 0；
int userinfo_p_deletid(int id) {
    chdir(USER_FILE);
    if(rename("USER", "USER_TEMP")<0){
        printf("Cannot open file %s!\n", "USER");
        return 0;
    }

    FILE *fp1;
    fp1= fopen("USER_TEMP", "rb");
    if (fp1 == NULL){
        printf("Cannot open file %s!\n", "USER");
        return 0;
    }
    FILE *fp2;
    fp2 = fopen("USER", "wb");
    if (fp2 == NULL) {
        printf("Cannot open file %s!\n", "USER_TEMP");
        return 0;
    }
    userinfo_service_t buf;
    int found = 0;
    while (!feof(fp1)) {
        if (fread(&buf, sizeof(userinfo_service_t), 1, fp1)) {
            if (id == buf.userid) {
                found = 1;
                continue;
            }
            fwrite(&buf, sizeof(userinfo_service_t), 1, fp2);
        }
    }
    fclose(fp1);
    fclose(fp2);
    remove("USER_TEMP");     //删除临时文件
    return found;

}

int message_p_update(message_t * data) {

    chdir(USER_FILE);
    FILE *fp = fopen(data->send_name,"ab+");
    if(fp == NULL){
        my_err("open_USER", __LINE__);
    }

    message_t buf;     //结构体
    int found = 0;
    while(!feof(fp)){
        if(fread(&buf,sizeof(message_t),1,fp)){
            fwrite(data, sizeof(message_t), 1, fp);
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}



//在账号文件中查找与参数账号匹配的账号，找到 return 1；否则 return 0；并进行覆盖重写
int userinfo_p_update(userinfo_service_t * data) {
    chdir(USER_FILE);
    FILE *fp = fopen("USER","rb+");
    if(fp == NULL){
        my_err("open_USER", __LINE__);
    }
    userinfo_service_t buf;     //结构体
    int found = 0;
    while(!feof(fp)){
        if(fread(&buf,sizeof(userinfo_service_t),1,fp)){
            if (buf.userid == data->userid) {
                fseek(fp, -sizeof(userinfo_service_t), SEEK_CUR);
                fwrite(data, sizeof(userinfo_service_t), 1, fp);
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}


//检查用户文件是否存在
int userinfo_p_checkfile(const char *string){

    chdir(USER_FILE);

    if(access(string, 0) == 0){
        return 1;
    }
    else{
        return -1;
    }
}

//创建用户文件
int creat_userdir(const char *string){
    int ret = 0;
    int fd;

    chdir(USER_FILE);
    if((fd = open(string, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP )) < 0){
        my_err("creat_info",__LINE__);
    }
    close(fd);
    ret = 1;
    return ret;
}

//写入用户信息
int userinfo_p_insert(const userinfo_service_t * data){

    chdir(USER_FILE);

    int ret = 0;
    FILE *fp;
    fp = fopen("USER","ab");
    if(fp == NULL){
        return 0;
    }
    ret = fwrite(data,sizeof(userinfo_service_t),1,fp);

    fclose(fp);

    return ret;
}

//根据用户id获取用户信息并通过data传出

int userinfo_p_selet(int id, userinfo_service_t * data){
    chdir(USER_FILE);

    FILE *fp;
    int found = 0;
    userinfo_service_t buf;
    fp = fopen("USER","rb");
    if(fp == NULL){
        return 0;
    }
    while(!feof(fp)){
        if(fread(&buf,sizeof(userinfo_service_t),1,fp)){
            if(buf.userid == id){
                *data = buf;
                found = 1;
                break;
            }
            else if(buf.conn_fd == id){
                *data = buf;
                found = -1;
                break;
            }
        }
    }
    fclose(fp);
    return found;

}

//根据用户名获取用户信息并通过data传出

int userinfo_p_seletname(const char* username, userinfo_service_t * data){

    chdir(USER_FILE);
    FILE *fp;
    int found = 0;
    userinfo_service_t buf;
    fp = fopen("USER","rb");
    if(fp == NULL){
        return 0;
    }
    while(!feof(fp)){
        if(fread(&buf,sizeof(userinfo_service_t),1,fp)){
            if(strcmp(buf.username,username) == 0){
                *data = buf;
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}



int userinfo_p_delet(int id){
    int ret = id;
    return ret;
}


//注册函数

int serve_user_register(send_t *precv){

    int i;
    int ret = 0;    //check flag
    send_t send_buf;
    send_t *psend = &send_buf;
    memset(psend, 0, sizeof(send_buf));
    userinfo_service_t data;

    //check USER file
    ret = userinfo_p_checkfile((char*)("USER"));

    if(ret < 0){
        creat_userdir("USER");
    }

    psend->conn_fd = precv->conn_fd;

    if(server_name_check(precv->userinfo.username) > 0){

        psend->userstatus = INVALID_USERINFO;
        psend->input_check = _INPUTNAME;

        send_data(psend->conn_fd, psend);

        return 0;
    }

    strcpy(data.username, precv->userinfo.username);

    //基本信息录入
    strcpy(data.userpasswd, precv->userinfo.userpasswd);

    strcpy(data.usersex, precv->userinfo.usersex);

    //id分配
    data.userid = EntKey_Srv_CompNewKey((char*)("USER"));

    //initialize no leave_message
    data.leavemes_flag = _NOMESSAGE;

    data.status = _OFFLINE;

    for(i = 0; i < FRIENDSIZE; i++){
        data.friendslist[i] = 0;
    }

    for(i = 0; i < GROUPCOUNT; i++){
        data.grouplist[i] = 0;
    }



    //log time initialize
    data.ontime.date.year = 0;
    data.ontime.date.month = 0;
    data.ontime.date.day = 0;
    data.offtime.date.year = 0;
    data.offtime.date.month = 0;
    data.offtime.date.day = 0;
    data.ontime.time.hour = 0;
    data.ontime.time.second = 0;
    data.ontime.time.minute = 0;
    data.offtime.time.hour = 0;
    data.offtime.time.second = 0;
    data.offtime.time.minute = 0;


    //用户信息录入
    ret = userinfo_p_insert(&data);

    if(ret > 0){
        printf("\nNO. %d user registed, informatiom below:\n", data.userid);
        printf("name: %s\n", data.username);
        printf("sex: %s\n\n", data.usersex);

        psend->userstatus = VALID_USERINFO;


        send_data(precv->conn_fd, psend);
    }
    // return informatiom lenth
    return ret;
}



//登录函数

int serve_user_login(send_t *precv){


    send_t send_buf;
    send_t *psend = &send_buf;
    userinfo_service_t info;
    userinfo_service_t *data = &info;
    message_t mes;

    memset(&mes, 0, sizeof(message_t));
    memset(psend, 0, sizeof(send_buf));

    psend->conn_fd = precv->conn_fd;

    if(server_name_check(precv->userinfo.username) < 0){

        psend->userstatus = INVALID_USERINFO;
        psend->input_check = _INPUTNAME;
        send_data(psend->conn_fd, psend);

        return 0;
    }
    else{
        strcpy(psend->userinfo.username, precv->userinfo.username);
    }
    //用户名密码校验

    userinfo_p_seletname(psend->userinfo.username, data);

    if(data->status ==  _ONLINE){
        printf("<!> name [%s] sign conflict, system refused sign in\n", precv->userinfo.username);
        psend->userstatus = INVALID_USERINFO;
        psend->input_check = _ONLINE;
        send_data(psend->conn_fd, psend);

        return 0;
    }
    //密码校验
    if((strcmp(data->userpasswd, precv->userinfo.userpasswd)) == 0){

        //system tips
        printf("\nNO.%d log in, the informatiom below\n", data->userid);  //service tips
        printf("name: %s\n", data->username);
        printf("sex: %s\n", data->usersex);
        printf("last log in time: %d-%d-%d %d:%d:%d\n\n",data->ontime.date.year,data->ontime.date.month,data->ontime.date.day,data->ontime.time.hour,data->ontime.time.minute,data->ontime.time.second);

        psend->userstatus = VALID_USERINFO;

    }
    else{
        psend->userstatus = INVALID_USERINFO;
        psend->input_check = _INPUTPASSWD;
        send_data(psend->conn_fd, psend);

        return 0;
    }

    data->conn_fd = precv->conn_fd;
    //更改系统信息
    data->status = _ONLINE;
    data->ontime.date = DateNow();
    data->ontime.time = TimeNow();

    data->conn_fd = psend->conn_fd;
    //保存
    userinfo_p_update(data);

    send_data(psend->conn_fd, psend);

    strcpy(mes.from_name, precv->userinfo.username);
    mes.date = data->ontime.date;
    mes.time = data->ontime.time;
    mes.type = _ONLINE;

    message_tip(&mes);

    return 0;
}

/*
int users_tips(char *username){

    int i;
    send_t send_buf;
    send_t *psend = &send_buf;
    userinfo_service_t data;
    userinfo_service_t buf;

    userinfo_p_seletname(username, &data);

    for(i = 0; data.friendslist[i] != 0; i++){

        memset(&buf, 0, sizeof(userinfo_service_t));
        userinfo_p_selet(data.friendslist[i], &buf);

        if(buf.status == _ONLINE){
            psend->command_type = _SYSTEMTIP;
            strcpy(psend->message.from_name, "system");
            strcpy(psend->message.message_buf, "your friend: ");
            strcat(psend->message.message_buf, username);
            strcat(psend->message.message_buf, "sign in .");

            send_data(buf.conn_fd, psend);

        }
    }
    return 0;
}
*/

//下线函数

int serve_user_logoff(send_t *precv){

    message_t mes;
    userinfo_service_t info;
    userinfo_service_t *data = &info;
    int ret;

    memset(&mes, 0, sizeof(message_t));

    ret = userinfo_p_selet(precv->conn_fd, data);
    if(!ret){
        return 0;
    }

    if(data->status == _ONLINE){
        data ->status = _OFFLINE;
    }
    else{
        return 0;
    }

    //用户状态改为下线d

    //下线时间赋值
    data->offtime.date = DateNow();
    data->offtime.time = TimeNow();


    //system
    printf("\nNO.%d log off, the informatiom below\n", data->userid);
    printf("name: %s\n", data->username);
    printf("sex: %s\n", data->usersex);
    printf("log in time: %d-%d-%d %d:%d:%d\n\n",data->ontime.date.year,data->ontime.date.month,data->ontime.date.day,data->ontime.time.hour,data->ontime.time.minute,data->ontime.time.second);

    //系统信息进行保存
    userinfo_p_update(data);

    strcpy(mes.from_name, precv->userinfo.username);
    mes.date = data->offtime.date;
    mes.time = data->offtime.time;
    mes.type = _OFFLINE;

    //
    message_tip(&mes);
    return 0;
}

int serve_user_update(send_t *precv){

    char name[32];
    int check = 0;
    userinfo_service_t info;
    userinfo_service_t *data = &info;

    //获取用户名
    strcpy(name, precv->userinfo.username);

    check = userinfo_p_seletname(name, data);

    //基本信息录入
    strcpy(data->userpasswd, precv->userinfo.userpasswd);

    strcpy(data->usersex, precv->userinfo.usersex);


    //保存
    userinfo_p_update(data);
    return 0;

}

//not ok not ok

int serve_user_delet(send_t * precv){

    userinfo_service_t info;
    userinfo_service_t *data = &info;


    userinfo_p_seletname(precv->userinfo.username, data);

    printf("\nlose user: NO.%d\n, the informatiom below\n", data->userid);
    printf("name: %s\n", data->username);
    printf("sex: %s\n", data->usersex);

    userinfo_p_deletid(data->userid);

    return 0;
}



int userinfo_p_selectall(userinfo_service_t * users){

    chdir(USER_FILE);
    FILE *fp;
    int count = 0;
    userinfo_service_t buf;
    fp = fopen("USER","rb");
    if(fp == NULL){
        return 0;
    }
    while(!feof(fp)){
        if(fread(&buf,sizeof(userinfo_service_t),1,fp)){
            if(buf.status == _OFFLINE)
            {
                continue;
            }
            else{
                users[count++] = buf;
            }
        }
    }
    fclose(fp);
    return count;
}

int server_name_check(char *username){    //重名检测

    chdir(USER_FILE);
    int check = 0;
    userinfo_service_t data;
    userinfo_service_t *info = &data;
    int ret;

    //check USER file
    ret = userinfo_p_checkfile((char*)("USER"));
    if(ret < 0){
        creat_userdir("USER");
    }


    //userfile check
    check = userinfo_p_seletname(username, info);

    if(check){  //name exist

        //send from command_type
        return 1;
    }
    else{
        return -1;
    }

}



