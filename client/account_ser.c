/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-05 13:52
 * Filename :        account_ser.c
 * Description :
 * *****************************************************************************/

#include "client.h"

int userinfo_s_add(userinfo_t *data){
    int ret = 0;

    ret = userinfo_p_checkfile(data->username);
    if(ret < 0){
        creat_userdir(data->username);
    }

    ret = userinfo_p_insert(data);

    return ret;
}

int userinfo_s_delet(const char *username)
{
    int ret;

    ret = userinfo_p_delet(username);

    return ret;
}

int userinfo_s_updata(userinfo_t *data, const char* username)
{
    int ret = 0;

    userinfo_p_delet(username);

    ret = userinfo_s_add(data);

    return ret;
}

int userinfo_s_select(const char *username, userinfo_t *data)
{
    int ret = 0;

    ret = userinfo_p_seletname(username, data);

    return ret;
}

int userinfo_s_login(const char *username){

    int ret = 0;

    chdir("/home/wyf/");
    if(access(username, 0) < 0){
        if(creat_userdir(username) < 0){
            my_err("file make", __LINE__);
        }
    }

    ret = 1;

    return ret;
}
