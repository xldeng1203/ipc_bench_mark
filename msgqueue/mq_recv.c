
/* 
    name:msgqueue_recv
    auth:neil deng 
    data:2019-11-28
    desc:SystemV 消息队列接收端 
*/

#include "mq.h"

int main(int argc, char*argv[])
{
    int msgid =  msgget(MSGKEY, IPC_CREAT | 0666);
    if(msgid == -1)
    {
        perror("mq_recv msgid == -1");
        return -1;
    }

    struct msg_get data;
    long int msgtype = 0; //（1）msgtype > 0接收消息类型为msgtype的消息（2）msgtype==0接收消息队列中最前面的那个消息
    int num = 0;
    int  count = 0;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    while (1)
    {
        num = msgrcv(msgid, (void*)&data, sizeof(data.buf), msgtype, 0);
        if(num == -1)
        {
            //perror("mq_recv failedn num == -1");
             printf("mq_recv failedn num == -1\n");
            break;
        }
        if (num == 0)
        {
            break;
        }
        printf("mq_recv read i = %d\n", (*(int*)data.buf));
        ++count;
    }

    gettimeofday(&end_time, NULL);
    double tm = getCostTime(&start_time, &end_time);
    printf("msq_recv :%.0fMB/s %.0fmsg/s  costtime:%lfs \n",
        count * MAX_TEXT * 1.0 / (tm * 1024 * 1024),
        count * 1.0 / tm, 
    tm);

    printf("mq_recv recv count =%d\n", count);
    return 0;
}
