
/* 
    desc:全双工管道，开启两个管道父进程写，子进程读
    auth:neil deng 
    data:2019-11-28


    
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>


//获取消耗时长
double getCostTime(const struct timeval *begin, const struct timeval *end)
{
    return (end->tv_sec + end->tv_usec * 1.0 / 1000000) -
           (begin->tv_sec + begin->tv_usec * 1.0 / 1000000);
}

int mypipe(char*argv[]);    //管道处理


int main(int argc, char*argv[])
{
    if(argc != 3)
    {
        perror("usage: ./pipe_full <dataSize> <count>");
        return -1;
    }

    int nRet = mypipe(argv);
    return nRet;
}

int mypipe(char*argv[])
{
    if (argv == NULL)
    {
        perror("pipe_full argv == NULL");
        return -1 ;
    }
    int size = atoi(argv[1]);
    int count = atoi(argv[2]);    
    int outfds[2] = {0};    //0为读，1为写
    int infds[2] = {0};    //0为读，1为写
    char * buf = NULL;
    int i = 0;

    if(size <= 0 || count <= 0 )
    {
        printf("pipe_full size:%d, nCount:%d", size,count );
        return -1;
    }

    buf = malloc(size);
    if(buf == NULL)
    {
        printf("pipe_full buf malloc failed size:%d", size );
        return -1;
    }

    struct timeval start_time, end_time;

    if(pipe(outfds) == -1)
    {
        perror("pipe_full outfds failed" );
        return -1;
    }

    if(pipe(infds) == -1)
    {
        perror("pipe_full infds failed" );
        return -1;
    }

    if(fork() == 0 )    //child
    {   
        gettimeofday(&start_time, NULL);  
        for(i = 0; i < count; ++i)
        {
            if(read(infds[0], buf, size) != size)
            {
                perror("pipe_full child read failed");
                return -1;
            }

            if(write(outfds[1], buf, size) != size)
            {
                perror("pipe_full child write failed");
                return -1;
            }
        }

        gettimeofday(&end_time, NULL);
        double tm = getCostTime(&start_time, &end_time);
        printf("pipe_recv :%.0fMB/s %.0fmsg/s  costtime:%lffs \n",
            count * size * 1.0 / (tm * 1024 * 1024),
            count * 1.0 / tm, 
            tm);
    }
    else    //parent
    {
        gettimeofday(&start_time, NULL);
        for(i = 0; i < count; ++i)
        {
            if(write(infds[1], buf, size) != size)
            {
                perror("pipe_full parent write failed");
                return -1; 
            }

            if(read(outfds[0], buf, size) != size)
            {
                perror("pipe_full parent read failed");
                return -1; 
            }
        }
         gettimeofday(&end_time, NULL);

        double tm = getCostTime(&start_time, &end_time);
        // printf("pipe_full:全双工   父进程 infds =======> 子进程");
        // printf("                   父进程 outfds <======= 子进程");
        printf("pipe_send :%.0fMB/s %.0fmsg/s  costtime:%lffs \n",
            count * size * 1.0 / (tm * 1024 * 1024),
            count * 1.0 / tm, 
            tm);
    }

    free(buf);
    buf = NULL;
    return 0;
}