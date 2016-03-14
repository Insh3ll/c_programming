#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //线程函数的头文件 编译时加上-lpthread
#include <semaphore.h> //包含信号量定义的头文件

#define THREAD_NUMBER 3
#define REPEAT_NUMBER 3
#define DELAY_TIME_LEVELS 10.0

sem_t sem[THREAD_NUMBER];

void *thrd_func(void *arg)
{
	int thrd_num=(int)arg;
	int delay_time=0;
	int count=0;

	sem_wait(&sem[thrd_num]);

	printf("Thread %d is starting\n",thrd_num);
	for(count=0;count<REPEAT_NUMBER;count++)
	{
		delay_time=(int)(rand()*DELAY_TIME_LEVELS/(RAND_MAX))+1;
		sleep(delay_time);
		printf("Thread %d: job %d delay=%d\n",thrd_num,count,delay_time);
	}
	printf("Thread %d finished\n",thrd_num);
	pthread_exit(NULL);
}

void main()
{
	pthread_t thread[THREAD_NUMBER];
	int no=0,res;
	void * thrd_ret;
	
	srand(time(NULL));

	for(no=0;no<THREAD_NUMBER;no++)
	{
		sem_init(&sem[no],0,0);
		res=pthread_create(&thread[no],NULL,thrd_func,(void *)no);
		if(res!=0)
		{
			printf("create threda %d failed\n",no);
			exit(res);
		}
	}
	printf("create thread success\n waiting for threads to finsh...\n");
	sem_post(&sem[THREAD_NUMBER-1]);
	for(no=THREAD_NUMBER-1;no>=0;no--)
	{
		res=pthread_join(thread[no],&thrd_ret);
		if(!res)
			printf("Thread %d joined\n",no);
		else
			printf("Thread %d join failed\n",no);
		sem_post(&sem[(no+THREAD_NUMBER-1)%THREAD_NUMBER]);
	}
	for(no=0;no<THREAD_NUMBER;no++)
		sem_destroy(&sem[no]);
}
