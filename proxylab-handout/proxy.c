#include <stdio.h>
#include "csapp.h"
#include <pthread.h> 


/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

void doit(int fd);
int parse_uri(char *uri, char *hostname, char *filename, char *port);
void *thread(void *vargp);

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int main(int argc, char **argv)
{   int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    
    /* Check command line args */
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); 
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
    Pthread_create(&tid, NULL, thread, (void *)connfd);
	//doit(connfd);                                             
	//Close(connfd); 
    printf("%s disconnected\n", hostname);                                           
    }
    
    //printf("%s", user_agent_hdr);
    return 0;
}

void doit(int fd) 
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char newhead[MAXLINE],result[MAXLINE];
    rio_t rio, rio_2;
    int clientfd;
    char port[MAXLINE];
    int n=0,flag=0;
    char hostname[MAXLINE],filename[MAXLINE];


    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE); 

    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);      
    if (strcasecmp(method, "GET")) {                     
        sprintf(newhead,"%s: proxy does not implement this method\n", method);
        Rio_writen(fd, newhead, strlen(newhead));
        return;
    }                                                   
    if ( strcasecmp(version, "HTTP/1.0") && strcasecmp(version, "HTTP/1.0")) {
        sprintf(newhead,"%s: proxy does not support this version\n", version);
        Rio_writen(fd, newhead, strlen(newhead));
        return;
    }

    flag  = parse_uri(uri, hostname, filename, port);
    if (!flag){
        sprintf(newhead,"something is wrong");
        Rio_writen(fd, newhead, strlen(newhead));
        return;
    }
                          
    clientfd = Open_clientfd(hostname, port);
    Rio_readinitb(&rio_2, clientfd);

    sprintf(newhead,"GET /%s HTTP/1.0\r\n", filename);
    sprintf(newhead,"%sHost: %s\r\n",newhead, hostname);
    sprintf(newhead,"%s%s",newhead, user_agent_hdr);
    sprintf(newhead,"%sConnection: close\r\n", newhead);
    sprintf(newhead,"%sProxy-Connection: close\r\n\r\n", newhead);

    Fputs(newhead, stdout);

    Rio_writen(clientfd, newhead, strlen(newhead));
    
    while ((n = Rio_readlineb(&rio_2, result, MAXLINE))!= 0) {      
        Rio_readlineb(&rio_2, result, MAXLINE);
        //Fputs(result, stdout);
        Rio_writen(fd, result, strlen(result));
    }
    
}

int parse_uri(char *uri, char *hostname, char *filename, char *port)
{   
    
    char *ptr, *ptr2;

    strcpy(port , "80");

    ptr = strstr(uri, "//");

    if(!ptr)
        ptr = uri;
    else
        ptr = ptr + 2;

    ptr2 = strstr(ptr, ":");

    if(ptr2){
        *ptr2 = '\0';
        sscanf(ptr,"%s",hostname);
        sscanf(ptr2+1,"%d%s", (int *)port, filename);
    }
    else{
        ptr2 = strstr(ptr, "/");

        if(!ptr2){
        strcpy(hostname, ptr);
        filename = NULL;
        }
        else {
        strcpy(filename, ptr2+1);
        *ptr = '\0';
        strcpy(hostname, ptr);
        }
    }
    
    return 1;
}

void *thread(void *vargp)
{
    int connfd = (int) vargp;
    Pthread_detach(pthread_self());
    doit(connfd);                                           
	Close(connfd); 
    return NULL;
}
