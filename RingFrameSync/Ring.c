#include "Ring.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Stack.h"

RING_TYPE ManualStack[Stack_SIZE]; //���淵��ֵʱ�򸲸ǵ�ԭʼ���ݣ�ʵ�ֵ��˺�����ǰ����ԭ�������
int HistoryNum = 0; //����ȥ���ٸ���
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



//InQueue��һ��bug������������ݴ���MaxSize����ѵ�һ�������ǵ�
char InQueue(pRing pBuff, const RING_TYPE *DataIn) //������û�д���֮ǰ���޷�����
{
#ifdef COMPLEX_TYPE
#else
	if (pBuff->Status > IDLE)
	{
		return -1; //����������Ҫ���������
	}
	pBuff->Status = IDLE;

	pBuff->BaseDataAddr[pBuff->rear] = DataIn[0];
	pBuff->rear = (pBuff->rear + 1) % pBuff->MaxSize; 
	if (pBuff->rear == pBuff->front) //����Ѵ�������rear����1����ʹָ֮�����һ��ֵ��������״̬
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
		assert(NULL);//״̬���ԣ����ݲ��������޷�����
		return -1;
	}
	pBuff->rear = (pBuff->rear + 1) % pBuff->MaxSize;
	if (pBuff->Status == STORED)
	{
		push(pBuff->BaseDataAddr + pBuff->rear);	//�ѵ�ǰ������ջ����
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
	if (BackSize > Stack_SIZE || pBuff->Status < STORED || BackSize < 0) //stored״̬�������û������еõ�����
	{
		assert(NULL);
		return -1; //����û����ô�����ݿɹ�����
	}
	for (i = 0; i < BackSize; i++)
	{
		//ManualStack[BackSize - 1 - i] = pBuff->BaseDataAddr[pBuff->rear]; //�ȱ���ԭʼ����
		ManualStack[i] = pBuff->BaseDataAddr[pBuff->rear]; //�ȱ���ԭʼ����
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
		return 0; //�ǿյģ����ö���
	}
	do 
	{
		DataOut[i] = pBuff->BaseDataAddr[pBuff->front];
		pBuff->front = (pBuff->front + 1) % pBuff->MaxSize;
		i++;
	} while (pBuff->front != pBuff->rear);
	if (pBuff->Status > IDLE) //�������״̬������һ��
	{
		DataOut[i] = pBuff->BaseDataAddr[pBuff->front];
		pBuff->front = (pBuff->front + 1) % pBuff->MaxSize;
	}
	pBuff->Status = EMPTY;
	//ReadAll֮��front���ǵ���rear����StatusΪEmpty

	return 0;
}

char StatusJudge(pRing pBuff)
{
	//ûɶ�ã���ʱ��ʵ��
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
