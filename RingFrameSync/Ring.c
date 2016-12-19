#include "Ring.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Stack.h"

RING_TYPE ManualStack[Stack_SIZE]; //保存返回值时候覆盖的原始数据，实现倒退后又能前进到原来的情况
int HistoryNum = 0; //返回去多少个数
char CreateRing(pRing ppBuff, const int BuffSize)
{
#ifdef COMPLEX_TYPE

#else
	ppBuff->MaxSize = BuffSize;
	ppBuff->BaseDataAddr = (RING_TYPE *)malloc(sizeof(RING_TYPE) * BuffSize);
	if (ppBuff->BaseDataAddr == NULL)
	{
		return -1;
	}
	ppBuff->front = 0;
	ppBuff->rear = 0;
	ppBuff->Status = EMPTY;
#endif
	ClearStack();
	return 0;
}

void DestoryRing(pRing pBuff)
{
	if (pBuff != NULL)
	{
		free(pBuff->BaseDataAddr);
		pBuff->BaseDataAddr = NULL;
	}
}

void ResetRing(pRing pBuff)
{
	pBuff->front = 0;
	pBuff->rear = 0;
	pBuff->Status = EMPTY;
}



//InQueue有一个bug，如果填充的数据大于MaxSize，会把第一个数覆盖掉
char InQueue(pRing pBuff, const RING_TYPE *DataIn) //在数据没有存满之前，无法工作
{
#ifdef COMPLEX_TYPE
#else
	if (pBuff->Status > IDLE)
	{
		return -1; //已满，不需要再填充数据
	}
	pBuff->Status = IDLE;

	pBuff->BaseDataAddr[pBuff->rear] = DataIn[0];
	pBuff->rear = (pBuff->rear + 1) % pBuff->MaxSize; 
	if (pBuff->rear == pBuff->front) //如果已存满，则rear倒退1个，使之指向最后一个值，并更新状态
	{
		pBuff->rear = pBuff->rear - 1;
		if (pBuff->rear < 0)
		{
			pBuff->rear += pBuff->MaxSize;
		}
		pBuff->Status = FIND;
	}
#endif
	return 0;
}

char StepIn(pRing pBuff, const RING_TYPE *DataIn)
{
	if (pBuff->Status < FIND)
	{
		assert(NULL);//状态不对，数据不存满，无法工作
		return -1;
	}
	pBuff->rear = (pBuff->rear + 1) % pBuff->MaxSize;
	if (pBuff->Status == STORED)
	{
		push(pBuff->BaseDataAddr + pBuff->rear);	//把当前的数入栈保存
	}
	if (HistoryNum > 0)
	{
		pBuff->BaseDataAddr[pBuff->rear] = ManualStack[HistoryNum - 1];
		HistoryNum--;
	}
	else
	{
		pBuff->BaseDataAddr[pBuff->rear] = DataIn[0];
	}
	pBuff->front = (pBuff->front + 1) % pBuff->MaxSize;
	return 0;
}

char BackTo(pRing pBuff, int BackSize)
{
	int i;
	if (BackSize > Stack_SIZE || pBuff->Status < STORED || BackSize < 0) //stored状态必须在用户程序中得到设置
	{
		assert(NULL);
		return -1; //根本没有那么多数据可供返回
	}
	for (i = 0; i < BackSize; i++)
	{
		//ManualStack[BackSize - 1 - i] = pBuff->BaseDataAddr[pBuff->rear]; //先保存原始数据
		ManualStack[i] = pBuff->BaseDataAddr[pBuff->rear]; //先保存原始数据
		pop(pBuff->BaseDataAddr + pBuff->rear); //pop value;
		pBuff->rear = pBuff->rear - 1;
		pBuff->front = pBuff->front - 1;
		if (pBuff->rear < 0)
		{
			pBuff->rear += pBuff->MaxSize;
		}
		if (pBuff->front < 0)
		{
			pBuff->front += pBuff->MaxSize;
		}
	}
	HistoryNum += BackSize;
	return 0;
}

char ReadAll(pRing pBuff, RING_TYPE *DataOut)
{
	int i = 0;
	if (pBuff->Status == EMPTY)
	{
		return 0; //是空的，不用读了
	}
	do 
	{
		DataOut[i] = pBuff->BaseDataAddr[pBuff->front];
		pBuff->front = (pBuff->front + 1) % pBuff->MaxSize;
		i++;
	} while (pBuff->front != pBuff->rear);
	if (pBuff->Status > IDLE) //如果是满状态，则多读一个
	{
		DataOut[i] = pBuff->BaseDataAddr[pBuff->front];
		pBuff->front = (pBuff->front + 1) % pBuff->MaxSize;
	}
	pBuff->Status = EMPTY;
	//ReadAll之后front总是等于rear，且Status为Empty

	return 0;
}

char StatusJudge(pRing pBuff)
{
	//没啥用，暂时不实现
	//if (pBuff->front == pBuff->rear && pBuff->Status == EMPTY)
	//{
	//	return EMPTY;
	//}
	//else if (pBuff->rear == pBuff->front)
	//{
	//	if (is_empty() == 0)
	//	{
	//		return STORED;
	//	}
	//	return FIND;
	//}
	//else
	//{
	//	return IDLE;
	//}
	return 0;
}
