
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>  
#include <unistd.h>
#include <sys/time.h>

#define MAX_TEXT  4096
#define MSGKEY (key_t)1234

struct msg_get
{
    long int msg_type;
    char buf[MAX_TEXT];
};

//获取消耗时长
double getCostTime(const struct timeval *begin, const struct timeval *end)
{
    return (end->tv_sec + end->tv_usec * 1.0 / 1000000) -
           (begin->tv_sec + begin->tv_usec * 1.0 / 1000000);
}
