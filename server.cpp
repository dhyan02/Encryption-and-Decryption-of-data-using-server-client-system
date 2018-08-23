#include <bits/stdc++.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;
struct client{
int fd;
sockaddr_in address;
};
vector<client> clientlist;
int main( int argc, char *argv[])
{
if(argc!=2)
{
printf("\n usage ./a.out port_no");
return -1;
}
int sfd, nsfd;
char buffer[1000], buffer2[1050];
struct sockaddr_in serv_addr,cli_addr;
socklen_t cli_len;
cli_len = sizeof(cli_len);
int port_no=atoi(argv[1]);
if((sfd = socket(AF_INET,SOCK_STREAM,0))==-1)
perror("\n socket ");
bzero(&serv_addr,sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(port_no);
serv_addr.sin_addr.s_addr = INADDR_ANY;
if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
{
perror("\n bind : ");
return -1;
10
}
listen(sfd,10);
int retval, fd_max;
struct timeval tv;
fd_set rfds;
while(1)
{
FD_ZERO(&rfds);
FD_SET(sfd, &rfds);
fd_max = sfd + 1;
for(int i = 0; i < clientlist.size(); i++)
{
FD_SET(clientlist[i].fd, &rfds);
if( clientlist[i].fd >= fd_max)
fd_max = clientlist[i].fd + 1;
}
tv.tv_sec = 5;
tv.tv_usec = 0;
retval = select(fd_max, &rfds, NULL, NULL, &tv);
if (retval == -1)
perror("select()");
else if(retval > 0)
{
if(FD_ISSET(sfd, &rfds))
{
if((nsfd = accept(sfd , (struct sockaddr *)&cli_addr , &cli_len))==-1)
{
perror("\n accept ");
return -1;
}
client obj;
obj.fd = nsfd;
obj.address = cli_addr;
printf("client added.\n");
clientlist.push_back(obj);
}
else
{
for(int i = 0; i < clientlist.size(); i++)
11
{
if(FD_ISSET(clientlist[i].fd, &rfds))
{
recv(clientlist[i].fd, buffer, sizeof(buffer), 0);
sprintf(buffer2, "IP : %s, PORT : %d ::: %s%c", inet_ntoa(clientlist[i].address.sin_addr),
ntohs(clientlist[i].address.sin_port), buffer, '\0');
printf("%s\n", buffer2);
for(int j = 0; j < clientlist.size(); j++)
{
if(i == j)
continue;
send(clientlist[j].fd, buffer2, strlen(buffer2) + 1, 0);
}
}
}
}
}
}
return 0;
}
