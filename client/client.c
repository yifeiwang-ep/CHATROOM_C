 /*******************************************************************************
 * Author :          Sequin_YF
 * Email :           Catherine199787@outlook.com
 * Last modified :   2016-08-03 15:24
 * Filename :        client.c
 * Description :     client
 * *****************************************************************************/
#include"client.h"
char h_username[32];
//sigin in

int signin_ui(int conn_fd){

    int ret = 0;
    send_t send_buf, recv_buf;
    send_t *psend = &send_buf;
    send_t *precv = &recv_buf;

    system("clear");

    printf("\033[31m <systerm> \033[0m:\033[33m welcome \033[0m,log in please\n");
    printf("username:");

    input_check(32, psend->userinfo.username);      //send and keep username

    printf("password:");
    getpwd(32, psend->userinfo.userpasswd);

    psend->command_type = _USERONL;
    psend->conn_fd = conn_fd;
    send_data(psend->conn_fd, psend);

    recv_data(conn_fd, precv);

    switch(precv->userstatus){
        case INVALID_USERINFO:
            if(precv->input_check == _INPUTNAME){
                puts("name not exist");
            }
            else if(precv->input_check == _INPUTPASSWD){
                puts("wrong password");
            }
            else if(precv->input_check == _ONLINE){
                puts("user already signed in");
            }
            break;
        case VALID_USERINFO:
            strcpy(h_username, precv->userinfo.username);
            ret = 1;
            puts("Sign in...");
            if(userinfo_s_login(precv->userinfo.username) < 0){
                my_err("file wrong", __LINE__);
            }
            break;
    }
    sleep(1);
    return ret;
}


//register
int register_ui(int conn_fd){

    int ret = 0;
    char sex_buf[10];
    char passwd_buf[32];
    char input_buf[32];

    send_t send_buf, recv_buf;
    send_t *psend = &send_buf;
    send_t *precv = &recv_buf;

    userinfo_t info;
    userinfo_t *data = &info;

    puts(" ");
    puts("\033[31m<system>\033[0m:complete you information first");

    //username
    printf("username:");

    input_check(32, psend->userinfo.username);      //send and keep username

    strcpy(info.username, psend->userinfo.username);

    //password
    while(1){
        printf("password:");

        getpwd(32,input_buf);  //implicit input
        printf("check it:");    //input twice
        getpwd(32, passwd_buf);

        //when input same strings
        if(strcmp(input_buf, passwd_buf) == 0){
            strcpy(psend->userinfo.userpasswd, passwd_buf);
            strcpy(info.userpasswd,input_buf);

            break;  //while
        }
        else{
            puts("wrong!input again,");
            continue;
        }
    }

    //sex
    while(1){
        printf("your sex[man/female]:");
        input_check(10, sex_buf);

        //limit string
        if(strcmp(sex_buf, "man") == 0){
            info.sex = USER_MAN;
            strcpy(psend->userinfo.usersex, sex_buf);

            break;
        }
        else if(strcmp(sex_buf, "female") == 0){
            info.sex = USER_FEMALE;
            strcpy(psend->userinfo.usersex, sex_buf);

            break;
        }
        else{
            puts("wrong input");
            continue;
        }
    }


    psend->conn_fd = conn_fd;
    psend->command_type = _USERADD;

    send_data(psend->conn_fd,psend);

    recv_data(conn_fd, precv);

    switch(precv->userstatus){
        case VALID_USERINFO:
            ret = 1;
            break;
        case INVALID_USERINFO:
            if(precv->input_check == _INPUTNAME){
                puts("name exist");
            }
            break;
    }

    if(ret == 1){
        //store infomation
        userinfo_s_add(data);
        puts("register successe, sign in now!");
    }
    sleep(1);
    return ret;
}


int main(){

    int conn_fd;
    int serv_port;
    struct sockaddr_in serv_addr;
    send_t send_buf, recv_buf;
    send_t *psend = &send_buf;
    send_t *precv = &recv_buf;
    pthread_t pid;
    userinfo_t data;
    memset(&data, 0, sizeof(userinfo_t));


    //初始化服务器地址结构
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;

    //服务器的端口与地址
    serv_port = 4507;
    serv_addr.sin_port = htons(serv_port);
    if(inet_aton("192.168.30.141", &serv_addr.sin_addr) == 0){
        my_err("invalid server ip address,please check the service IP\n",__LINE__);
    }

    //创建一个TCP套接字
    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0){
        my_err("socket", __LINE__);
    }

    //向服务器发送连接请求
    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0){
        my_err("connect", __LINE__);
        exit(0);
    }


    user_init_(conn_fd);  //return when sign in

    precv->conn_fd = conn_fd;

    pthread_create(&pid, NULL, (void*)thread_fun, (void *)conn_fd);

    userinfo_ui_init(conn_fd, h_username);  //main meune

    shutdown(conn_fd, SHUT_RD);
}

int * thread_fun(int conn_fd){

    int ret;
    pthread_t pid;
    send_t recv_buf;
    send_t *precv = &recv_buf;
    memset(precv, 0, sizeof(recv_buf));
    void *flag;

    while(1){

        if((ret = recv_data(conn_fd, precv)) <= 0){
            print();
            puts("\033[31m404 not FOUND...\033[0m");
            sleep(1);
            account_ui_logoff(conn_fd, precv->userinfo.username);
            break;
        }
        //else if(ret == 0){
          //  break;
        //}
        else{

            pthread_create(&pid, NULL, (void*)&client, precv) ;
            pthread_join(pid, &flag);

        }
    }
    pthread_exit(0);
}


int user_init_(int conn_fd){

    char choice;
    do{
        int ret= 0;
        //登陆界面（int conn_fd）
        system("clear");
        puts("\033[33m welcome ,what do you want?\033[0m");
        puts("Sign in");
        puts("Register");
        puts("Exit");
        puts("");
        choice = l_getc();

        cl_stdin();
        switch (choice) {
            case 'S':
            case 's':
                ret = signin_ui(conn_fd);
                break;

            case 'r':
            case 'R':
                register_ui(conn_fd);
                break;
        }
        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        if(ret == 1){
            return 0;
        }

    }while(choice != 'e' && choice != 'E');

    account_ui_logoff(conn_fd, (char *)("user"));
}



void client(send_t * precv){

    if(precv->userinfo.leavemes_flag == _LEAVEMES){
        //chat_leave_check();
    }

    switch(precv->command_type){

        case _FRIENDDEL:

            chat_friend_del(precv);

            break;
        case _FRIENDLIST:

            chat_friend_show(precv);
            break;

        case _GROUPLIST:

            chat_group_show(precv);
            break;

        case _FRIENDFIND:

            friend_find_add(precv);
            break;
        case _GROUPSEND:

            group_find_add(precv);

            break;
        case _USERSALL:

            chat_friend_add(precv);

            break;
        case _GROUPSALL:

            chat_group_add(precv);

            break;

        case _DOWNLOAD:
            download_file(precv);
            break;
        case _FRIENDASK:
            printf("\33[s");
            friend_chat_ask(precv);
            printf("\33[u");
            fflush(stdout);
            break;
        case _MESSTIP:
            printf("\033[s");
            message_tips(precv);
            printf("\33[u");
            fflush(stdout);
            break;

        case _CHATONE:
            printf("\033[s");
            user_chat_one(precv);
            printf("\33[u");
            fflush(stdout);
            break;
    }

}


int print(){

    int i = 35;

    printf("\33[10;1H");

    while(i--){
        printf("\33[K");
        printf("\33[1B");
    }

    printf("\33[10;1H");

    return 0;
}


void my_chdir(){

    userinfo_t data;
    userinfo_p_seletname(h_username, &data);

    chdir(data.path);
}
