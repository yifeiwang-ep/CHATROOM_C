/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-13 09:26
 * Filename :        chat.c
 * Description :
 * *****************************************************************************/

#include "service.h"
/*
   int server_message_leave(int conn_fd){

   char recv_buf[BUFSIZE];
   char username[32] = {"\0"};
   userinfo_service_t info;
   userinfo_service_t *data = &info;
   fileinfo_message_t finfo;
   fileinfo_message_t *fdata = &finfo;
   message_t minfo;
   message_t *mdata = &minfo;


   strcpy(username, recv_buf);

   userinfo_p_seletname(recv_buf, data);

   if(data->leavemes_flag == _LEAVEMES){

//user decide check leave_message or not
if(strcmp(recv_buf, "y")){

server_file_select(username, fdata);   //search leave_message file by username

send(conn_fd, fdata, sizeof(finfo),0);

while(fdata->count--){

server_message_selet((char*)("leave_message"), mdata);  //get message

send(conn_fd, mdata, sizeof(message_t), 0);        //send
}

}
else{
return 0;
}
}

return 0;
}


int server_message_temp(int conn_fd){

char username[32] = {"\0"};
fileinfo_message_t finfo;
fileinfo_message_t *fdata = &finfo;
message_t minfo;
message_t *mdata = &minfo;

my_recv(conn_fd, username, sizeof(username));

server_file_select(username, fdata);   //search temp file information by username

send(conn_fd, fdata, sizeof(finfo),0);

//no temp message, return
if(fdata->count == 0){
return 0;
}

while(fdata->count--){

server_message_selet((char*)("temp_message"), mdata);  //get message

send(conn_fd, mdata, sizeof(message_t), 0);        //send
}

return 0;
}
*/
int message_tip(message_t *mes){

    send_t send_buf;
    send_t *psend = &send_buf;
    userinfo_service_t data;
    userinfo_service_t temp_data;
    int sendlist[FRIENDSIZE];
    char namelist[FRIENDSIZE][32];
    int i, n = 0;

    memset(&data, 0, sizeof(data));

    userinfo_p_seletname(mes->from_name, &data);
    for(i = 0; data.friendslist[i] != 0; i++){

        memset(&temp_data, 0, sizeof(temp_data));
        userinfo_p_selet(data.friendslist[i], &temp_data);
        if(temp_data.status == _OFFLINE){
            continue;
        }
        else{
            sendlist[n] = temp_data.conn_fd;
            strcpy(namelist[n], temp_data.username);
            n++;
        }
    }

    i = 0;
    switch(mes->type){
        case _ONLINE:
        case _OFFLINE:

            while(n > i){
                psend->command_type = _MESSTIP;
                strcpy(psend->message.from_name, mes->from_name);
                strcpy(psend->message.send_name, namelist[i]);
                strcpy(psend->message.message_buf, mes->message_buf);
                psend->message.date = mes->date;
                psend->message.time = mes->time;
                psend->message.type = mes->type;

                send_data(sendlist[i], psend);
                i++;
            }
            break;
    }
    return 0;
}


int message_p_insert(message_t *mes){

    chdir("/home/");
    int ret = 0;
    FILE *fp;
    fp = fopen(mes->send_name,"ab");
    if(fp == NULL){
        return 0;
    }
    ret = fwrite(mes ,sizeof(message_t),1,fp);

    fclose(fp);

    return ret;
}

int leave_p_insert(send_t *precv){

    chdir("/home/");
    int ret = 0;
    FILE *fp;
    fp = fopen("LEAVE","ab");
    if(fp == NULL){
        return 0;
    }
    ret = fwrite(precv ,sizeof(send_t),1,fp);

    fclose(fp);

    return ret;
}

int server_friend_delet(send_t * precv){
    send_t send_buf;
    send_t *psend = &send_buf;
    userinfo_service_t data;
    int id = 0;
    int i;
    memset(psend, 0, sizeof(send_t));

    psend->conn_fd = precv->conn_fd;
    psend->command_type = _FRIENDDEL;
    strcpy(psend->userinfo.username, precv->userinfo.username);

    if(server_name_check(precv->friendinfo.username) < 0){
        psend->userstatus = INVALID_USERINFO;

        send_data(psend->conn_fd, psend);
    }

    userinfo_p_seletname(precv->friendinfo.username, &data);

    id = data.userid;
    memset(&data, 0, sizeof(userinfo_service_t));

    userinfo_p_seletname(psend->userinfo.username, &data);

    for(i = 0; data.friendslist[i] != 0; i++){

        if(data.friendslist[i] == id){
            data.friendslist[i] = data.friendslist[i+1];
        }
    }

    psend->userstatus = VALID_USERINFO;

    userinfo_p_update(&data);

    send_data(psend->conn_fd, psend);
    return 0;
}


int server_friend_show(send_t *precv){

    int i = 0;

    userinfo_service_t info[FRIENDSIZE];
    userinfo_service_t data;

    int list[FRIENDSIZE];

    send_t send_buf;
    send_t *psend = &send_buf;
    memset(psend, 0, sizeof(send_t));
    memset(&data, 0, sizeof(userinfo_service_t));
    memset(info, 0, sizeof(info));
    memset(list, 0, sizeof(list));

    userinfo_p_seletname(precv->userinfo.username, &data);   //get user information


    int n = 0, flag = 0;
    for(i = 0; i<FRIENDSIZE; i++){
        //copy friendlist, reblind pointer
        if(data.friendslist[i] != 0){
            list[n] = data.friendslist[i];
            n++;
            flag = 1;
        }
    }

    //no friend, return

    i = 0;

    if(flag == 0){
        psend->conn_fd = precv->conn_fd;
        psend->userstatus = INVALID_USERINFO;
        psend->command_type = _FRIENDLIST;
        send_data(psend->conn_fd, psend);
        return 0;
    }
    else{
        while(i < n){
            //get friend all information
            userinfo_p_selet(list[i], &info[i]);
            psend->conn_fd = precv->conn_fd;

            psend->userstatus = VALID_USERINFO;
            //get friend_info_t information
            strcpy(psend->friendinfo.username, info[i].username);
            strcpy(psend->friendinfo.usersex, info[i].usersex);
            psend->friendinfo.status = info[i].status;
            psend->command_type = _FRIENDLIST;

            send_data(psend->conn_fd, psend);
            i++;
        }
    }

    return 0;
}

/*

   int server_group_show(int conn_fd){

   char username[32] = {"\0"};
   int group_buf[FRIENDSIZE];
   int i = 0;

   group_info_t ginfo;
   group_info_t *gdata = &ginfo;

   userinfo_service_t info;
   userinfo_service_t *data = &info;

//

userinfo_p_seletname(username, data);   //get user information

send(conn_fd, data->friendslist, sizeof(data->friendslist),0);

for(i = 0; data->friendslist[i] != 0; i++){
//copy grouplist, reblind pointer
group_buf[i] = data->grouplist[i];
}

//no group, return
if(i == 0){
return 0;
}

while(i--){

//get group_info_t information
server_group_selet(group_buf[i], gdata);

send(conn_fd, gdata, sizeof(group_info_t) ,0);  //send friend information

}

return 0;
}
*/

int server_friend_find(send_t *precv){
    userinfo_service_t data, temp_data;

    int ret = 1;
    int i;
    send_t send_buf;
    send_t *psend = &send_buf;

    strcpy(psend->friendinfo.username, precv->friendinfo.username);
    psend->command_type = _FRIENDFIND;
    psend->conn_fd = precv->conn_fd;
    strcpy(psend->userinfo.username, precv->userinfo.username);

    //name not exist
    ret = userinfo_p_seletname(precv->friendinfo.username, &data);

    userinfo_p_selet(data.userid, &temp_data);
    for(i = 0; temp_data.friendslist[i] != 0; i++){
        if(data.userid == temp_data.friendslist[i]){

            psend->userstatus = INVALID_USERINFO;
            send_data(precv->conn_fd, psend);
            return 0;
        }
    }

    if(!ret){
        psend->userstatus = INVALID_USERINFO;
    }
    else{
        psend->userstatus = VALID_USERINFO;
        psend->friendinfo.status = data.status;
        strcpy(psend->friendinfo.usersex, data.usersex);
    }

    send_data(precv->conn_fd, psend);
    return 0;
}

int server_chat_one(send_t *precv, int type){

    int ret;
    message_t mes;
    send_t send_buf;
    send_t *psend = &send_buf;
    userinfo_service_t data;

    memset(&data, 0, sizeof(userinfo_service_t));
    memset(&mes, 0, sizeof(message_t));


    ret = userinfo_p_seletname(precv->message.send_name, &data);
    if(!ret){
        psend->userstatus = INVALID_USERINFO;
        strcpy(psend->message.send_name, precv->message.send_name);
        psend->conn_fd = precv->conn_fd;
        psend->message.type = INVALID_USERINFO;
        psend->command_type = _CHATONE;
        return 0;
    }

    if(data.status == _OFFLINE){
        data.leavemes_flag = _LEAVEMES;
        userinfo_p_update(&data);

        ret = userinfo_p_checkfile(precv->message.send_name);
        if(ret < 0){
            creat_userdir(precv->message.send_name);
        }
        leave_p_insert(precv);
    }
    else{
        strcpy(psend->message.from_name, precv->message.from_name);
        strcpy(psend->message.send_name, precv->message.send_name);
        strcpy(psend->message.message_buf, precv->message.message_buf);
        psend->message.date = precv->message.date;
        psend->userstatus = VALID_USERINFO;
        psend->message.time = precv->message.time;
        psend->message.type = _CHATONE;
        psend->command_type = type;
        psend->conn_fd = data.conn_fd;

        send_data(psend->conn_fd, psend);
    }

    return 0;
}

int server_friend_send(send_t *precv){
    int ret = 0;
    userinfo_service_t data;
    message_t mes;


    send_t send_buf;
    send_t *psend = &send_buf;

    memset(&mes, 0, sizeof(data));
    memset(&data, 0, sizeof(data));

    strcpy(mes.from_name, precv->message.from_name);
    strcpy(mes.send_name, precv->message.send_name);
    strcpy(mes.message_buf, precv->message.message_buf);
    mes.type = _FRIENDASK;
    mes.date = precv->message.date;
    mes.time = precv->message.time;

    userinfo_p_seletname(precv->message.send_name, &data);
    psend->conn_fd = data.conn_fd;

    if(data.status == _OFFLINE){
        data.leavemes_flag = _LEAVEMES;
        userinfo_p_update(&data);


        ret = userinfo_p_checkfile("LEAVE");
        if(ret < 0){
            creat_userdir("LEAVE");
        }

        leave_p_insert(precv);

    }
    else{
        strcpy(psend->message.from_name, precv->message.from_name);
        strcpy(psend->message.send_name, precv->message.send_name);
        strcpy(psend->message.message_buf, precv->message.message_buf);
        psend->message.date = mes.date;
        psend->message.time = mes.time;
        psend->message.type = _FRIENDASK;
        psend->command_type = _MESSTIP;

        send_data(psend->conn_fd, psend);
    }


    return 0;
}


int server_friend_add(send_t *precv){

    userinfo_service_t *users;
    int ret = -1;
    send_t send_buf;
    send_t *psend = &send_buf;

    memset(psend, 0, sizeof(send_t));
    users = (userinfo_service_t *)malloc(sizeof(userinfo_service_t)*ACCOUNTSIZE);

    strcpy(psend->userinfo.username, precv->userinfo.username);
    psend->command_type = _USERSALL;
    psend->conn_fd = precv->conn_fd;

    //get friend all information
    ret = userinfo_p_selectall(users);
    int i = 0;

    if(ret == 0){
        //no friend, return
        psend->userstatus = INVALID_USERINFO;
        send_data(psend->conn_fd, psend);
    }
    else{

        do{
            psend->userstatus = VALID_USERINFO;
            strcpy(psend->userinfo.username, precv->userinfo.username);
            psend->command_type = _USERSALL;
            psend->conn_fd = precv->conn_fd;


            //get friend_info_t information
            strcpy(psend->friendinfo.username, users[i].username);
            strcpy(psend->friendinfo.usersex, users[i].usersex);
            psend->friendinfo.status = users[i].status;
            send_data(psend->conn_fd, psend);

            i++;
        }while(--ret);
    }

    free(users);
    return 0;
}


int server_friend_combain(send_t *precv){

    int i, j;
    int flag = 0;
    int ret, ret1;
    int user1, user2;
    userinfo_service_t data, data1;

    memset(&data, 0, sizeof(data));
    memset(&data1, 0, sizeof(data1));

    if(precv->userstatus == VALID_USERINFO){
        ret = userinfo_p_seletname(precv->message.from_name, &data);
        if(ret < 0){
            my_err("name error", __LINE__);
        }

        ret1 = userinfo_p_seletname(precv->message.send_name, &data1);
        if(ret1 < 0){
            my_err("name error", __LINE__);
        }

        user2 = data1.userid;
        user1 = data.userid;
        for(i = 0; data.friendslist[i] != 0; i++){
            if(data.friendslist[i] == user2){
                flag = 1;
            }
        }
        data.friendslist[i] = user2;

        for(j = 0; data1.friendslist[j] != 0; j++){
            if(data.friendslist[j] == user1){
                flag = 1;
            }
        }
        data1.friendslist[j] = user1;

        userinfo_p_update(&data);
        userinfo_p_update(&data1);


    }
    //
    server_chat_one(precv, _FRIENDASK);
    return 0;
}
