/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-12 11:34
 * Filename :        chat.c
 * Description :
 * *****************************************************************************/

#include"client.h"

int chat_user_init(int conn_fd, char*username)
{
    char choice;
    send_t send_buf;
    send_t *psend = &send_buf;

    //标记下

    //chat choice
    do{

        print();
        puts("");
        puts("\033[33mwhat do you want?\033[0m");
        puts("friend list");
        puts("group list");
        puts("check message box");
        puts("return");
        puts("exit");

        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

        choice = l_getc();

        switch(choice){
            case 'f':
            case 'F':
                //show the frind list

                psend->conn_fd = conn_fd;
                psend->command_type = _FRIENDLIST;
                strcpy(psend->userinfo.username, username);

                send_data(conn_fd, psend);
                friend_chat_show(conn_fd, username);
                break;

            case 'g':
            case 'G':
                //show the group list

                psend->conn_fd = conn_fd;
                psend->command_type = _GROUPLIST;
                strcpy(psend->userinfo.username, username);

                send_data(conn_fd, psend);

                group_chat_show(conn_fd, username);

                break;

            case 'c':
            case 'C':

                message_init(conn_fd, username);
                break;
            case 'r':
            case 'R':
                return 0;
        }
    }while(choice != 'e' && choice != 'E');

    account_ui_logoff(conn_fd, username);

    return 0;
}

int chat_group_add(send_t *precv){

    int n;   //如果n>50,则n = 50;

    for(n = 0; precv->userinfo.grouplist[n] != 0; n++);

    if(n == 0){
        puts("no group now, return and build a group!");
        sleep(1);
        return 0;
    }
    else{

        //只显示在线用户！
        printf("[name]:%s\t\t[size]:%d\t\t[owner]%s\n", precv->group_info.groupname, precv->group_info.size, precv->group_info.owner);
    }
    return 0;
}



int group_chat_add(int conn_fd, char *username){

    send_t send_buf;
    send_t *psend = &send_buf;
    char choice;

    do{
        puts("");
        puts("what do you want?");
        puts("join group");
        puts("search one");
        puts("return");

        choice = l_getc();

        switch(choice){
            case 'j':
            case 'J':
            case 's':
            case 'S':
                printf("groupname:");
                input_check(32, psend->group_info.groupname);
                psend->conn_fd = conn_fd;
                strcpy(psend->userinfo.username, username);
                psend->command_type = _GROUPFIND;
                send_data(conn_fd, psend);
        }
    }while(choice != 'r' && choice != 'R');
    return 0;
}


int chat_friend_add(send_t *precv){

    if(precv->userstatus == INVALID_USERINFO){
        puts("\033[31m<systerm>\033[0m:no one is on line now, come back later");
        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        return 0;
    }
    else if(precv->userstatus == VALID_USERINFO){
        //只显示在线用户！
        printf("name:\033[33m%s\033[0m\t\tsex:\033[33m%s\033[0m\n", precv->friendinfo.username, precv->friendinfo.usersex);
    }
    return 0;
}

int friend_chat_add(int conn_fd, char *username){
    send_t send_buf;
    send_t *psend = &send_buf;
    char choice;

    //do{
        puts("");
        puts("\033[33mwhat do you want?\033[0m");
        puts("add friend");
        puts("search one");
    //    puts("return");
        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        choice = l_getc();

        switch(choice){
            case 'a':
            case 'A':
            case 's':
            case 'S':

                cl_stdin();
                printf("name:");
                input_check(32, psend->friendinfo.username);
                print();
                psend->conn_fd = conn_fd;
                strcpy(psend->userinfo.username, username);
                psend->command_type = _FRIENDFIND;
                send_data(conn_fd, psend);
                sleep(1);
                friend_chat_send(conn_fd, username);
                break;
        }
    //}while(choice != 'r' && choice != 'R');
    return 0;
}

int friend_chat_send(int conn_fd, char *username){

    send_t send_buf;
    send_t *psend = &send_buf;
    int ret;
    char  choice;
    message_t mes;
    memset(&mes, 0, sizeof(message_t));

    puts("y/n?");
    choice = l_getc();

    if(choice != 'y' && choice != 'Y'){
        return 0;
    }

    ret = message_p_selet(_FRIENDSEND, &mes);
    if (!ret){
        my_err("message_selet", __LINE__);
    }
    else{

        cl_stdin();
        printf("input a hello message, text:");
        input_check(32, mes.message_buf);

        strcpy(psend->message.send_name, mes.send_name);
        strcpy(psend->message.from_name, mes.from_name);
        strcpy(psend->message.message_buf, mes.message_buf);
        psend->conn_fd = conn_fd;
        strcpy(psend->userinfo.username, username);
        mes.date = psend->message.date = DateNow();
        mes.time = psend->message.time = TimeNow();

        psend->command_type = _FRIENDSEND;

        send_data(conn_fd, psend);
    }

    return 0;
}

int message_p_selet(int type, message_t * mes){

    my_chdir();
    int ret = 0;
    message_t buf;
    memset(&buf, 0, sizeof(message_t));
    FILE *fp;

    fp = fopen("message", "rb");
    if(fp == NULL){
        my_err("message_selet", __LINE__);
    }
    else{
        while(!feof(fp)){
            fread(&buf, sizeof(message_t), 1, fp);
            if(buf.type == type){
                *mes = buf;
                ret = 1;
            }
        }
    }

    fclose(fp);
    return ret;
}

int group_find_add(send_t *precv){
    send_t send_buf;
    send_t *psend = &send_buf;
    memset(psend, 0, sizeof(send_buf));
    char choice;

    printf("\nname:%s\n", precv->group_info.groupname);
    printf("sex :%d\n", precv->group_info.size);
    printf("owner:%s", precv->group_info.owner);
    puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

    puts("join(y/n)?");

    choice = l_getc();

    if(choice == 'y' || choice == 'Y'){
        psend->conn_fd = precv->conn_fd;
        strcpy(psend->userinfo.username, precv->userinfo.username);
        strcpy(psend->group_info.groupname, precv->group_info.groupname);
        psend->command_type = _GROUPSEND;
        //发送用户名由服务端连接入群请求
        send_data(precv->conn_fd, psend);
    }
    //取消加入请求，返回主目录
    return 0;
}


int friend_find_add(send_t *precv){
    message_t mes;
    memset(&mes, 0, sizeof(message_t));

    if(precv->userstatus == INVALID_USERINFO){
        puts("not a leagel name");
        return 0;
    }
    else{

        printf("name:%s\n", precv->friendinfo.username);
        printf("sex :%s\n", precv->friendinfo.usersex);
        if(precv->friendinfo.status == _ONLINE){
            puts("status: online");
        }
        else if(precv->friendinfo.status == _OFFLINE){
            puts("status: offline");
            printf("User not online,  will send it when user online\n");
        }
        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");


        /*
        if(userinfo_p_checkfile("message") < 0){
            creat_userdir("message");
        }
        */


        strcpy(mes.from_name, precv->userinfo.username);
        strcpy(mes.send_name, precv->friendinfo.username);
        mes.type = _FRIENDSEND;
        mes.date = DateNow();
        mes.time = TimeNow();

        message_p_insert(&mes);
    }

    return 0;
    //取消加好友请求，返回主目录
}

//发送用户名由服务端连接好友请求
//同意返回对方性别提示，不同意则直接返回



//同意返回对方性别提示，用户未在线则服务器保存请求等待用户上线,不同意则直接返回

int message_p_all(message_t *box, int type){
    //now set two types,when send leave_message signal, return leave_message file info. And so on
    my_chdir();
    FILE *fp;
    int ret = 0;
    message_t buf;
    memset(&buf, 0, sizeof(message_t));

    fp = fopen("message", "rb");
    if(fp == NULL){
        my_err("all_message", __LINE__);
    }

    while(!feof(fp)){
        fread(&buf, sizeof(message_t), 1, fp);
        if(buf.type == type){
            box[ret] = buf;
            ret++;
        }
    }

    fclose(fp);
    return ret--;
}

int message_init(int conn_fd, char *username){
    char choice;
    int type;

    do{
        print();
        puts(" ");
        puts("\033[33mwhat do you want?\033[0m");
        puts("friend ask");
        puts("message");
        puts("return");
        puts("exit");

        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

        choice = l_getc();

        switch(choice){
            case 'f':
            case 'F':
                type = _FRIENDASK;
                message_box(conn_fd, username, type);
                break;
            case 'm':
            case 'M':
                //
                break;
            case 'r':
            case 'R':
                return 0;
        }
    }while(choice != 'e' && choice != 'E');

    account_ui_logoff(conn_fd, username);
}

//common message view function
int message_box(int conn_fd, char* username, int type){
    char choice;
    int n;
    int ret = -1;
    send_t send_buf;
    send_t *psend = &send_buf;
    message_t mes;
    message_t *box;
    memset(&mes, 0, sizeof(message_t));

    box = (message_t *)malloc(sizeof(message_t)*100);

    print();

    ret = message_p_all(box, type);
    if(ret == 0){
        puts("empty box");
        return 0;
    }

    ret--;

    do{
        printf("\033[33m%d\033[0m pieces of \033[33mFriend Ask\033[0m show you below\n\n", ret);

        for(n = 0; n < ret; n++){
            printf("%d-%d-%d %d:%d:%d\n", box[n].date.year, box[n].date.month,box[n].date.day, box[n].time.hour, box[n].time.minute, box[n].time.second);
            printf("From [\033[33m%s\033[0m]:\033[33m%s\033[0m\n", box[n].from_name, box[n].message_buf);

            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        }

        puts(" ");
        puts("\033[33mwhat do you want?\033[0m");
        puts("ask reply");
        puts("clean box");
        puts("return");
        puts("exit");

        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

        choice = l_getc();
        cl_stdin();

        switch(choice){
            case 'a':
            case 'A':
                puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
                puts("add");
                puts("refuse");
                puts("ignore");
                puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
                choice = l_getc();
                cl_stdin();

                switch(choice){
                    case 'a':
                    case 'A':
                        printf("name:");
                        input_check(32, psend->message.send_name);
                        psend->command_type = _FRIENDASK;
                        strcpy(psend->message.from_name, username);
                        psend->userstatus = VALID_USERINFO;
                        psend->message.date = DateNow();
                        psend->message.time = TimeNow();

                        send_data(conn_fd, psend);
                        break;
                    case 'r':
                    case 'R':
                        printf("name:");
                        input_check(32, psend->message.send_name);
                        psend->command_type = _FRIENDASK;
                        strcpy(psend->message.from_name, username);
                        psend->userstatus = INVALID_USERINFO;
                        psend->message.date = DateNow();
                        psend->message.time = TimeNow();

                        send_data(conn_fd, psend);

                        break;
                    case 'i':
                    case 'I':

                        /*
                           psend->command_type = _FRIENDASK;
                           strcpy(psend->friendinfo.username, mes.from_name);
                           strcpy(psend->userinfo.username, username);
                           psend->userstatus = INVALID_USERINFO;
                           */
                        puts("\033[31m<system>\033[0mkeep it in messagebox in case");
                        //message_keep(message_t mes);
                        break;
                }

                break;
            case 'c':
            case 'C':
                userinfo_p_delet("message");
                break;
            case 'r':
            case 'R':
                return 0;
        }
        free(box);
        print();
    }while(choice != 'e' && choice != 'E');

    account_ui_logoff(conn_fd, username);
    return 0;
}



int chat_group_show(send_t *precv){
    int n;

    for(n = 0; precv->userinfo.grouplist[n] != 0; n++);

    if(n == 0){
        puts("you dont belong any group now, go chatting and joinng a group chat");
    }
    else{
        printf("name:%s\t\tsize:%d\t\townwe:%s\n", precv->group_info.groupname, precv->group_info.size, precv->group_info.owner);
    }
    return 0;
}

int group_chat_show(int conn_fd, char* username){
    char choice;
    send_t send_buf;
    send_t *psend = &send_buf;

    memset(psend, 0, sizeof(send_t));
    do{
        print();
        puts("");
        puts("what do you want?");
        puts("chat");
        puts("add group");
        puts("build a group");
        puts("return");

        choice = l_getc();

        switch(choice){
            case 'c':
            case 'C':
                input_check(32, psend->group_info.groupname);
                psend->conn_fd = conn_fd;
                strcpy(psend->userinfo.username, username);
                psend->command_type = _CHATGRO;
                send_data(conn_fd, psend);

                break;
            case 'd':
            case 'D':
                input_check(32, psend->group_info.groupname);
                psend->conn_fd = conn_fd;
                strcpy(psend->userinfo.username, username);
                psend->input_check = _INPUTNAME;
                psend->command_type = _GROUPDEL;
                send_data(conn_fd, psend);

                break;
            case 'a':
            case 'A':
                strcpy(psend->userinfo.username, username);
                psend->command_type = _GROUPSALL;
                send_data(conn_fd, psend);

                group_chat_add(conn_fd, username);
                break;
            case 'b':
            case 'B':
                //build a group
                break;
        }
        //

    }while(choice != 'r' && choice != 'R');

    return 0;
}

int user_chat_one(send_t *precv){
    message_t mes;
    memset(&mes, 0, sizeof(message_t));

    int ret;

    strcpy(mes.from_name, precv->message.from_name);
    strcpy(mes.send_name,precv->message.send_name);
    strcpy(mes.message_buf, precv->message.message_buf);
    mes.date = precv->message.date;
    mes.time = precv->message.time;
    mes.type = precv->message.type;

    /*
    ret = userinfo_p_checkfile("message");
    if(ret < 0){
        creat_userdir("message");
    }
    */

    message_p_insert(&mes);

    message_tips(precv);
    message_store();
    return 0;
}

int chat_friend_show(send_t * precv){

    // get friendlist
    //get friend count
    if(precv->userstatus == INVALID_USERINFO){
        puts("\033[31m<systerm>\033[0m:seems you have no friend now, go chatting and adding some friends");
    }
    else{
        printf("[name]:\033[33m%s\033[0m\t\t[sex]:\033[33m%s\033[0m\t\t", precv->friendinfo.username, precv->friendinfo.usersex);
        if(precv->friendinfo.status == _ONLINE){
            printf("[status]:\033[33m%s\033[0m\n", "ONLINE");
        }
        else if(precv->friendinfo.status == _OFFLINE){
            printf("[status]:\033[33m%s\033[0m\n", "OFFLINE");
        }
    }
    puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
    return 0;
}

int friend_chat_show(int conn_fd, char* username){
    char choice;
    send_t send_buf;
    send_t *psend = &send_buf;

    memset(psend, 0, sizeof(send_buf));
    do{
        print();
        puts(" ");
        puts("\033[33mwhat do you want?\033[0m");
        puts("chat");
        puts("add more friend");
        puts("delet friend");
        puts("return");
        puts("exit");

        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

        choice = l_getc();

        switch(choice){
            case 'c':
            case 'C':
                friend_chat_one(conn_fd, username);
                break;
            case 'd':
            case 'D':
                printf("name:");
                input_check(32, psend->friendinfo.username);

                psend->conn_fd = conn_fd;
                strcpy(psend->userinfo.username, username);
                psend->command_type = _FRIENDDEL;
                send_data(conn_fd, psend);

                // chat_friend_del(precv)
                break;
            case 'a':
            case 'A':
                psend->conn_fd = conn_fd;
                strcpy(psend->userinfo.username, username);
                psend->command_type = _USERSALL;
                send_data(conn_fd, psend);
                sleep(1);
                friend_chat_add(conn_fd, username);
                break;
            case 'r':
            case 'R':
                return 0;
        }
    }while(choice != 'e' && choice != 'E');

    account_ui_logoff(conn_fd, username);
    return 0;
}

int message_store(){

    my_chdir();
    int i, count = 0;
    message_t *box;
    int ret = -1;


    box = (message_t *)malloc(sizeof(message_t)*100);
    ret = message_p_all(box, _CHATONE);

    print();
    puts("\033[33m=========================================================\033[0m");

    ret--;

    for(i = ret; i >= 0 && count < 5; i--, count++){
        printf("%d-%d-%d %d:%d:%d\n", box[i].date.year, box[i].date.month, box[i].date.day, box[i].time.hour, box[i].time.minute, box[i].time.second );
        printf("\033[33m%s\033[0m:", box[i].from_name);
        puts(box[i].message_buf);
        puts(" ");
    }

    printf("\n\n\n\n\n\033[33m=========================================================\033[0m");

    free(box);

    return 0;
}

int friend_chat_one(int conn_fd, char *username){

    send_t *psend;
    char namebuf[32];
    message_t mes;
    userinfo_t data;

    memset(&data, 0, sizeof(userinfo_t));

    psend = (send_t *)malloc(sizeof(send_t));
    cl_stdin();

    printf("name:");
    input_check(32, namebuf);
    strcpy(psend->friendinfo.username, namebuf);
    strcpy(psend->userinfo.username, username);
    psend->conn_fd = conn_fd;
    psend->command_type = _FRIENDLIST;

    userinfo_p_seletname(username, &data);
    if(data.type == INVALID_USERINFO){
        return 0;
    }

    while(1){

        message_store();

        memset(&mes, 0, sizeof(message_t));
        psend->conn_fd = conn_fd;
        mes.date = DateNow();
        mes.time = TimeNow();
        strcpy(psend->message.from_name, username);
        strcpy(psend->message.send_name, namebuf);
        psend->command_type = _CHATONE;

        psend->message.date = mes.date;
        psend->message.time = mes.time;

        printf("\033[35;1H");
        puts("\033[33m-------------------------------------------------------------\033[0m");
        printf("\033[36;1H");

        printf(" name:[%s]\t\t\ttime:%d-%d-%d %d:%d:%d\n", namebuf, mes.date.year, mes.date.month, mes.date.day, mes.time.hour, mes.time.minute, mes.time.second);
        puts("\033[33m-------------------------------------------------------------\033[0m");

        input_check(BUFSIZE, psend->message.message_buf);

        printf("\033[3A");
        printf("\033[K");
        printf("\033[2B");
        printf("\033[K");
        //fflush(stdout);

        strcpy(mes.message_buf, psend->message.message_buf);
        strcpy(mes.send_name, namebuf);
        strcpy(mes.from_name, username);
        mes.type = _CHATONE;

        message_p_insert(&mes);

        send_data(conn_fd, psend);

    }

    return 0;
}

/*
   void chat_leave_check(send_t *precv){
   char choice;
   system("clear");
   puts("<system>:you have some messages when you gone,do you wanna check it( y)?");

   choice = l_getc();

   if(choice == 'y'){
   message_box(precv, (char *)("MESSAGE when you LEAVE:"));
   }
   }
   */

int friend_chat_ask(send_t* precv){

    int i = 8;
    message_t mes;
    memset(&mes, 0, sizeof(message_t));

    printf("\33[1;1H");
    while(i--){
        printf("\033[1B");
        printf("\033[K");
    }
    printf("\33[1;1H");

    if(precv->userstatus == VALID_USERINFO){
        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        puts("\033[33mNew Friend\033[0m");
        printf("%d-%d-%d %d:%d:%d\n", precv->message.date.year, precv->message.date.month, precv->message.date.day, precv->message.time.hour, precv->message.time.minute, precv->message.time.second);
        printf("[\033[33m%s\033[0m] became your friend\n", precv->message.from_name);
    }
    else{
        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        printf("[\033[33m%s\033[0m] reject your friend ask\n", precv->message.from_name);
    }

    return 0;
}

int message_tips(send_t *precv){

    int ret, i = 8;
    userinfo_t data;
    message_t mes;
    memset(&data, 0, sizeof(userinfo_t));
    memset(&mes, 0, sizeof(message_t));

    printf("\33[1;1H");
    while(i--){
        printf("\033[1B");
        printf("\033[K");
    }
    printf("\33[1;1H");

    switch(precv->message.type){
        case _OFFLINE:

            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
            printf("%d-%d-%d %d:%d:%d\n", precv->message.date.year, precv->message.date.month, precv->message.date.day, precv->message.time.hour, precv->message.time.minute, precv->message.time.second);
            printf("\033[31m<systerm>\033[0m:your friend [\033[33m%s\033[0m] offline\n", precv->message.from_name);
            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

            break;
        case _ONLINE:

            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
            printf("%d-%d-%d %d:%d:%d\n", precv->message.date.year, precv->message.date.month, precv->message.date.day, precv->message.time.hour, precv->message.time.minute, precv->message.time.second);
            printf("\033[31m<systerm>\033[0m:your friend [\033[33m%s\033[0m] online\n", precv->message.from_name);
            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

            break;
        case _FRIENDASK:

            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
            puts("\033[31m<systerm>\033[0m:recive a \033[33mfriend ask\033[0m");
            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

            strcpy(mes.from_name, precv->message.from_name);
            strcpy(mes.send_name,precv->message.send_name);
            strcpy(mes.message_buf, precv->message.message_buf);
            mes.date = precv->message.date;
            mes.time = precv->message.time;
            mes.type = precv->message.type;

            /*
            ret = userinfo_p_checkfile("message");
            if(ret < 0){
                creat_userdir("message");
            }
            */


            message_p_insert(&mes);

            break;

        case _CHATONE:

            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
            puts("\033[31m<systerm>\033[0m:recive a \033[33mfriend message\033[0m");
            puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");

            break;

        case INVALID_USERINFO:

            printf("\n\n\033[31m<systerm>\033[0m:name [\033[33m%s\033[0m] is not your friend\n\n", precv->message.send_name);
            userinfo_p_seletname(precv->userinfo.username, &data);
            data.type = INVALID_USERINFO;
            userinfo_p_insert(&data);
            break;

    }

    //    fflush(stdout);
    return 0;
}

int message_p_insert(message_t * mes){

    my_chdir();

    int ret;
    FILE *fp;

    fp = fopen("message", "ab");
    if(fp == NULL){
        my_err("message_insert", __LINE__);
    }

    ret = fwrite(mes, sizeof(message_t), 1, fp);
    if(!ret){
        my_err("message_p_insert", __LINE__);
    }

    fclose(fp);
    return ret;
}



int chat_friend_del(send_t *precv){

    if(precv->userstatus == INVALID_USERINFO){
        puts("input woring ,name not exist");
    }
    else if(precv->userstatus == VALID_USERINFO){
        puts("delet friend succeed");
    }

    sleep(1);
}
