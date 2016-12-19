#ifndef __RING_H
#define __RING_H
/************************************************************************/
/* 名称：Ring
** 作者：尹超
** 时间：2016-12-16
** 描述：一种自定义的数据结构。该结构主要用来解决帧同步的数据流问题
** 类似于FIFO，在Ring存满状态下开始工作，查找感兴趣的信息。
** 基本功能：StepIn一个数据会刷新掉最后面的数据，用于查找感兴趣的内容
** 增强功能：开启Stored状态，StepIn时会把刷掉的数据保存起来。通过BackTo可
** 以返回到之前的某个状态，而继续StepIn又会把先前刷掉的数据自动补起来
** 实现时光倒流的效果。
** 备注：只有Ring存满数据后才能正常开始工作。							*/
/************************************************************************/



//#define COMPLEX_TYPE //定义此处，为复数模式

#ifdef COMPLEX_TYPE
	typedef struct Complex RING_TYPE;
#else
	typedef int RING_TYPE;
#endif // COMPLEX_TYPE


static enum MyEnum
{
	EMPTY, //没有数据
	IDLE,  //数据不满，无法工作
	FIND,  //数据存满，开启查找
	STORED //数据存满，边查边存数据
};


struct RING
{
	RING_TYPE *BaseDataAddr;
	int front;
	int rear;
	int MaxSize;
	int WorkMode;
	int Status; //0->空 1->满（正常工作状态） other->数据不满，不能工作
};
typedef struct RING *pRing;

char CreateRing(pRing pBuff, const int BuffSize);  //创建一个Ring，长度为BuffSize
void DestoryRing(pRing pBuff);	//销毁Ring，回收资源
void ResetRing(pRing pBuff);		//重置Ring，内容为空，堆栈为空
char InQueue(pRing pBuff, const RING_TYPE *DataIn);	//把数据存入Ring（）
char StepIn(pRing pBuff, const RING_TYPE *DataIn);	//开启寻找模式，一次判断得到一个数
char BackTo(pRing pBuff, int BackSize);	//利用Stack返回到之前的某个状态(被挤出的数也同样被Stack保存)


char ReadOne(pRing pBuff, const RING_TYPE *DataIn, RING_TYPE *DataOut); //一次读取一个数
char ReadAll(pRing pBuff, RING_TYPE *DataOut); //把Ring中残存的数据一次读空
char StatusJudge(pRing pBuff);


// 用户调用函数
char DelayCorrWorking(pRing pBuff);
char CenterCorrWorking(pRing pBuff);



#endif
