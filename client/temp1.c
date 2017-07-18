#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
//#include "/usr/include/c++/4.8.2/iostream"
using namespace std;
/*
typedef union epoll_data
{
  void        *ptr;
  int          fd;
  __uint32_t   u32;
  __uint64_t   u64;
} epoll_data_t;

struct epoll_event
{
  __uint32_t   events; // Epoll events /
  epoll_data_t data;   // User data variable /
};
*/
int num = 0;
#define MAX_EVENTS 10
struct epoll_event ev, clientevents[MAX_EVENTS],events[MAX_EVENTS+1];
int listenfd, connfd, nfds, epollfd;
void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
		//exit(0);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
      //  exit(1);
    }
}
//初始化event，data.u32为0表示未使用
void InitEvents()
{
	int i;
	for(i=0;i<MAX_EVENTS;i++)
	{
		clientevents[i].data.u32=0;
	}
}
//获取未被使用的clientevents序号
int GetEmptyEventsId()
{
	int i;
	for(i=0;i<MAX_EVENTS;i++)
	{
		if(clientevents[i].data.u32==0)
		return i;
	}
	return -1;
}
//对断开的套接字清理
void DelEventsFd(int fd)
{
	int i;
	for(i=0;i<MAX_EVENTS;i++)
	{
		if(clientevents[i].data.fd==fd)
		{
		epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&clientevents[i]);
		clientevents[i].data.u32=0;
		close(fd);
		}
		break;
	}
}


int main()
{
	//////////创建listen////////////////////
	short port = 4507;
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	setnonblocking(listenfd);
	sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    bind(listenfd, (struct sockaddr*)&sin, sizeof(sin));
    listen(listenfd, 5);
	//////////////创建epoll///
	InitEvents();
	epollfd=epoll_create(MAX_EVENTS);
	if(epollfd<=0)
	{
		printf("create errr\n");
		return 0;
	}else
	{	printf("EpollFd:%d\n",epollfd);
	}
	//////////////////设置属性加入信号///////////////
	ev.events=EPOLLIN;
	ev.data.fd=listenfd;
	if(epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev)==0)
		printf("Set nfds OK\n");
		int i;
	/////////////////////////////////////
	while(true)
	{
	nfds=epoll_wait(epollfd,events,MAX_EVENTS,-1);
		if(nfds>0)
			printf("nfds:%d\n",nfds);

	for(i=0;i<nfds;i++)
	{
		if(events[i].data.fd==listenfd){
			sockaddr_in local;
			socklen_t addrlen=sizeof(local);
			connfd=accept(listenfd,(struct sockaddr*)&local,&addrlen);
			printf("New Link Fd:%d   %d\n",connfd, num);
            num++;
			int id=GetEmptyEventsId();
			if(id==-1)
			{
				printf("Not Empty\n");
				continue;
			}
			clientevents[id].data.u32=1;
			setnonblocking(connfd);
			clientevents[id].data.fd=connfd;
			clientevents[id].events=EPOLLIN|EPOLLET;
			if(epoll_ctl(epollfd,EPOLL_CTL_ADD,connfd,&clientevents[id])==0)
				printf("Set OK linkfd:%d eventid:%d\n",connfd,id);
		}else
		{
			char temp[256]={0};
			int n=recv(events[i].data.fd,temp,256,0);
			printf("Message from:%d len:%d data:%s\n",events[i].data.fd,n,temp);
			if(n<=0)
			{
				printf("fd:%d closed\n",events[i].data.fd);
				DelEventsFd(events[i].data.fd);
			}
		}
	}
	}
	//////////关闭///////////////////
	close(epollfd);
	return 0;
}

