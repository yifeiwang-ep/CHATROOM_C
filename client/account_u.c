/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-08 09:09
 * Filename :        account_u.c
 * Description :
 * *****************************************************************************/

#include"client.h"

//system界面

void account_ui_init(int conn_fd,char *username){
    char choice;

    do{
        puts("");
        puts("\033[33mwhat do you want?\033[0m");
        puts("Update");
        puts("delet");
        puts("return");
        puts("exit");

        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        choice = l_getc();

        switch(choice){
            case 'u':
            case 'U':
                account_ui_update(conn_fd,username);
                break;
            case 'd':
            case 'D':
                //call S prepare to delet user informatio
                account_ui_delete(conn_fd, username);
                break;
            case 'r':
            case 'R':
                return;
                break;
        }
    }while(choice != 'e' && choice != 'E');
    account_ui_logoff(conn_fd,username);
}


//登录后界面
void userinfo_ui_init(int conn_fd, char *username){

    char choice;

    system("clear");
    do{
        print();
        puts("\033[33mwhat do you want?\033[0m");
        puts("Chat");
        puts("FTP");
        puts("Systerm");
        puts("Exit");

        puts("\033[33m.-.-.-.-.-.-.-.-.-.--.-.\033[0m");
        choice = l_getc();

        switch(choice){
            case 'c':
            case 'C':
                chat_user_init(conn_fd, username);
                break;
            case 'f':
            case 'F':
                file_ui_init(conn_fd, username);
                break;
            case 's':
            case 'S':
                account_ui_init(conn_fd,username);
                break;
        }
    }while(choice != 'e' && choice != 'E');

    account_ui_logoff(conn_fd, username);
}


void account_ui_update(int conn_fd, char *username){

    userinfo_t info;
    userinfo_t *data = &info;
    send_t send_buf;
    send_t *psend = &send_buf;
    char choice;
    char input_buf[32], passwd_buf[32];
    int ret = 0;


    userinfo_s_select(username, data);

    puts("\033[33myour information blew:\033[0m");

    printf("%s","name:");
    puts(data->username);
    printf("%s","sex:");
    switch(data->sex){
        case 1:
            puts("faleman");
            break;
        case 2:
            puts("man");
            break;
    }

    cl_stdin();
    puts("\033[33m\nwhat do you want\033[0m]");
    puts("updata sex");
    puts("updata password");
    puts("return");

    choice = l_getc();

    switch(choice){
        case 'p':
        case 'P':
            printf("new password:");
            getpwd(32, input_buf);
            printf("check if:");
            getpwd(32, passwd_buf);
            if(strcmp(passwd_buf, input_buf) == 0){
                strcpy(data->userpasswd, passwd_buf);
            }
            else{
                puts("wrong, not the same");
                return;
            }
            break;
        case 's':
        case 'S':
            puts("1.female");
            puts("2.man");
            scanf("%d",&ret);
            if(ret == 1){
                data->sex = USER_FEMALE;
            }
            else if(ret == 2){
                data->sex = USER_MAN;
            }
            break;
    }

    strcpy(psend->userinfo.userpasswd, data->userpasswd);
    psend->command_type = _USERCHA;
    strcpy(psend->userinfo.username, username);
    if(data->sex == USER_MAN){
       strcpy(psend->userinfo.usersex, "man");
    }
    else if(data->sex == USER_FEMALE){
       strcpy(psend->userinfo.usersex, "female");
    }

    send_data(conn_fd, psend);

    //userinfo_s_updata(data, username);

   //should be run to sign
   return;
}


void account_ui_logoff(int conn_fd,char *username){
    send_t send_buf;
    send_t *psend = &send_buf;

    memset(psend, 0, sizeof(send_t));
    strcpy(psend->userinfo.username, username);
    psend->conn_fd = conn_fd;
//    psend->userstatus = _OFFLINE;
    psend->command_type = _USEROFFL;
    send_data(conn_fd, psend);

    puts("\033[31mlog off ...\33[8m]");

    sleep(2);

    shutdown(conn_fd, SHUT_RD);
    exit(0);
}


void account_ui_delete(int conn_fd,char *username){

    send_t send_buf;
    send_t *psend = &send_buf;
    char choice;

    system("clear");
    puts("\033[31m<system>:ready for delection, can never return\033[1m");
    puts("delet(y/Y)?");

    choice = l_getc();
    if(choice == 'y' || choice == 'Y'){

        strcpy(psend->userinfo.username, username);
        psend->command_type = _USERDEL;
        send_data(conn_fd, psend);

        userinfo_s_delet(username);

        account_ui_logoff(conn_fd, username);
    }
    else if(choice == 'r' || choice == 'R'){
        return ;
    }
}
