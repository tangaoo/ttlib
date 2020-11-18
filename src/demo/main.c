/*
 * @Copyright (C)  2020  .Harman. all right reserved
 * @file       main.c
 * @ingroup    main
 * @author     tango
 * @date       2020-10 
 * @brief      main.c file
 */

#define TT_TRACE_MODULE_NAME     "MAIN"
#define TT_TRACE_DEBUG           (1)

#if 1
#include <stdlib.h>
#include <stdio.h>
#include "../ttlib/ttlib.h" ///need opt
#include "demo.h"

/// demo 
extern tt_uint32_t demo_utils_main(tt_void_t);
extern tt_void_t demo_circular_buffer(tt_void_t);
extern tt_void_t tt_demo_single_list_entry_main(tt_void_t);
extern tt_void_t tt_demo_list_entry_main(tt_void_t);
extern tt_void_t demo_crc8_main(tt_void_t);
extern tt_void_t tt_demo_queue_entry_main(tt_void_t);





int main(void)
{
#if 1
	if (!tt_lib_init(tt_null, tt_native_allocator())) return -1;
#else
	if (!tt_init(tt_null, tt_static_large_allocator_init((tb_byte_t*)malloc(1 * 1024 * 1024), 1 * 1024 * 1024, 8))) return -1;
#endif		
	
//	tt_assert(1);
//	tt_abort();
	// tt_trace_d("long size, %d", sizeof(long));
	// long tdata = -1;
	// tt_trace_d("tdata, %ld", tdata);

	// tt_demo_queue_entry_main();
#if 0
	tt_int32_t* pdata =  (tt_int32_t*)tt_malloc(sizeof(tt_int32_t));

	tt_trace_raw("\n\n");
	tt_trace_i("demo_utils_main**************************************");
	demo_utils_main();

	tt_trace_raw("\n\n");
	tt_trace_i("demo_circular_buffer*********************************");
	demo_circular_buffer();

	tt_trace_raw("\n\n");
	tt_trace_i("demo_single_list_entry*******************************");
	tt_demo_single_list_entry_main();

	tt_trace_raw("\n\n");
	tt_trace_i("demo_list_entry*******************************");
	tt_demo_list_entry_main();

	tt_trace_raw("\n\n");
	tt_trace_i("demo_crc8*******************************");
	demo_crc8_main();

	tt_trace_d("long, %d", sizeof(long));
	tt_trace_d("long double, %d", sizeof(long double));
#endif	
	system("pause"); //stop
	return 0;
}

#else if

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <windows.h>
#include <winbase.h>
#include <semaphore.h>

#define LEN 100000
#define NUM 5
int queue[NUM];
sem_t psem, csem;
int g_num = 0;

pthread_cond_t condv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg)
{
	int n = NUM;
	while (n--)
	{
		Sleep(1000);
		printf("producer post sem, %d\n", NUM - n);
		sem_post(&psem);
		
	}

	return NULL;
}

void *consumer(void *arg)
{
	int n = 0;
	while (1)
	{
		sem_wait(&psem);
		printf("recv producer sem, %d\n", ++n);
		if (n == NUM)
			break;
	}
}

void *thread_fun(void *arg)
{
	pthread_mutex_t *pmutex = (pthread_mutex_t *)arg;
	int i;

	for (i = 0; i < LEN; i++)
	{ 
		pthread_mutex_lock(pmutex);
		g_num += 1;
		pthread_mutex_unlock(pmutex);
	}

	return NULL;
}

char *thread_func1(void *arg)
{
	char *msg = "thread_func1 return";
	pid_t pid = getpid();
	pthread_t tid = pthread_self();

	printf("%s, pid,%u, tid,%u\n", arg, pid, tid);

	return msg;
}

void *thread_func2(void *arg)
{
	pid_t pid = getpid();
	pthread_t tid = pthread_self();

	printf("%s, pid,%u, tid,%u\n", arg, pid, tid);
	while (1)
	{
		printf("thread func2 running\n");
		Sleep(1000);
	}

	return NULL;
}




int main()
{
	pthread_t pid1, pid2;
	char* rev = NULL;

	sem_init(&psem, 0, 0);

	pthread_create(&pid1, NULL, (void *)producer, NULL);
	pthread_create(&pid2, NULL, (void *)consumer, NULL);
	
	pthread_join(pid1, &rev);
	pthread_join(pid2, &rev);

	sem_destroy(&psem);

	return 0;
}

#endif