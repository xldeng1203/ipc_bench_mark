
/* 
    name:msgqueue_send
    auth:neil deng 
    data:2019-11-28
    desc:SystemV 消息队列发送端
*/

#include "mq.h"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        perror("usage: ./msg_send  <count>");
        return -1;
    }

    if (argv == NULL)
    {
        perror("msq_send argv == NULL");
        return -1 ;
    }
    int count = atoi(argv[1]);    

    int msgid =  msgget(MSGKEY, IPC_CREAT | 0666);
    if(msgid == -1)
    {
        perror("msq_send msgid == -1");
        return -1;
    }

    struct msg_get data;
    long int msgtype = 0; //注意1 发消息时的msgtype用法：指定发送的消息类型 ，消息类型大于0
    data.msg_type = 1;  //这里不能为0 否则出现msq_send failedn num == -1: Invalid argument错误
    int num = 0;
    int n = 0;
    int i = 0;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    for(i = 0; i < count; ++i)
    {
         ++n;
         *(int*)data.buf = i;
        num = msgsnd(msgid, (void*)&data, sizeof(data.buf), msgtype);
        if(num == -1)
        {
            perror("msq_send failedn num == -1");
            return -1;
        }
    }

    gettimeofday(&end_time, NULL);
    double tm = getCostTime(&start_time, &end_time);
    printf("msq_send :%.0fMB/s %.0fmsg/s  costtime:%lfs \n",
        count * MAX_TEXT * 1.0 / (tm * 1024 * 1024),
        count * 1.0 / tm, 
    tm);

    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        perror("msq_send msgctl IPC_RMID failed ");
        return -1;
    }

    printf("msq_send recv n =%d , count = %d\n", n, count);
    return 0;
}
