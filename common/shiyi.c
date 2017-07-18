#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

int* thread(char s[])
{
    int pos = 0;

    while(1)
    {
        printf("\033[s");
        //设置光标位置
        printf("\033[%d;%dH", 5, 5);
        //清除从光标到行尾的内容

        printf("\033[K");
        printf("%s", s[pos]);
        pos = (pos+1)%3;
        printf("\033[u");

        fflush(stdout);
        sleep(1);
    }

    return NULL;
}

int main()
{
    pthread_t thid1, thid2;
    int i;
    char s[100];

    printf("\033[%d;%dH", 30, 5);

    //创建线程
    if( pthread_create(&thid,NULL,(void*)thread,(void*)&i)
       != 0 )
    {
        printf("Error!\n");
        exit(1);
    }
    if( pthread_create(&thid2,NULL,(void*)thread,(void*)&i)
       != 0 )
    {
        printf("Error!\n");
        exit(1);
    }


    while(1)
    {
        printf("\033[%d;%dH", 30, 5);
        scanf("%s", s);
        printf("\033[%d;%dH", 30, 5);
        printf("\033[K");
        printf("\033[%d;%dH%s", 20, 5, s);

    }

    return 0;
}
