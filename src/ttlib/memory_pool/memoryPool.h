/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       memoryPool.h
 * @ingroup    memorypool
 * @author     tango
 * @date       2020-11-19 
 * @brief      memoryPool.h file
 */

//@ mempool demo
/*
void MemPoolMain(void)
{
	unsigned char *p[20];
	int buffer[100] = { 0 }, i;

	tt_trace_i("buffer, %X", buffer);
	MemPoolInit(&g_MemPool, buffer, 8, 10);

	while (1)
	{
		for (i = 0; i < sizeof(p) / sizeof(p[0]); i++)
		{
			p[i] = MemAlloc(&g_MemPool);
			if (p[i] != NULL)
			memset(p[i], i, 8);
			tt_trace_i("i, %d, addr, %X", i, p[i]);
		}

		for (i = 0; i < sizeof(p) / sizeof(p[0]); i++)
		{
			MemFree(&g_MemPool, p[i]);
		}
	}

}
*/

//TODO 内存分配接口需要考虑对齐问题
#ifndef TT_MEMORY_POOL_H
#define TT_MEMORY_POOL_H

#include "prefix.h"

#pragma pack(1)
typedef struct tag_MemoryPool{
	tt_int32_t     magic;                            //magic num
	tt_byte_t      *pBlock;                          //point real buffer block
	tt_uint32_t    nSize;                            //unit's size 
	tt_uint32_t    nCnt;                             //unit count of all buffer block
	tt_uint32_t    nFree;                            //free unit count
	tt_uint32_t    nFirst;                           //point first empty unit

}tMemoryPool;
#pragma pack()
/*
*  分配固定大小内存块，没有内存控制块的消耗，内存与内存之间不用插入控制块
*/
extern unsigned char MemPoolInit(tMemoryPool *pMemPool, void *addr, unsigned int usize, unsigned int cnt);

extern void *MemAlloc(tMemoryPool *pMemPool);
extern unsigned char MemFree(tMemoryPool *pMemPool, void *addr);
//extern void MemPoolMain(void);

#endif // !_MEMORY_POOL_H_


