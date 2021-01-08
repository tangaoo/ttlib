/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       memoryPool.c
 * @ingroup    memorypool
 * @author     tango
 * @date       2020-11-19 
 * @brief      memoryPool.c file
 */
#if 0

#include "memoryPool.h"

tMemoryPool g_MemPool;

void GateIn(void){}

void GateOut(void){}

unsigned char MemPoolInit(tMemoryPool *pMemPool, void *addr, unsigned int usize, unsigned int cnt)
{
	unsigned int i;

	pMemPool->pBlock = (tt_byte_t *)addr;
	pMemPool->nSize = usize;
	pMemPool->nCnt = cnt;
	pMemPool->nFirst = 0;
	pMemPool->nFree = pMemPool->nCnt;
	
	for (i = 0; i < cnt; i++)
	{
		*(unsigned short *)(pMemPool->pBlock + i*pMemPool->nSize) = i+1;
	}

	return 0;
}

void *MemAlloc(tMemoryPool *pMemPool)
{
	unsigned char *p;
	GateIn();
	if (pMemPool->nFree == 0)
	{	
		tt_trace_i("have no buffer ,error");
		return NULL;
	}

	p = pMemPool->pBlock + pMemPool->nFirst * pMemPool->nSize;
	pMemPool->nFirst = *(unsigned short *)p;
	pMemPool->nFree -= 1;

	GateOut();

	return p;
}

unsigned char MemFree(tMemoryPool *pMemPool, void *addr)
{
	int n = 0;

	GateIn();
	if ((unsigned char*)addr < pMemPool->pBlock || (unsigned char*)addr > (pMemPool->pBlock + pMemPool->nSize * pMemPool->nCnt))
	{
		tt_trace_i("addr is not valid");
		return 1;
	}

	n = ((unsigned int)addr - (unsigned int)pMemPool->pBlock) / pMemPool->nSize;
	*(unsigned short *)addr = pMemPool->nFirst;
	pMemPool->nFirst = n;
	pMemPool->nFree += 1;

	GateOut();

	return 0;
}

#endif

