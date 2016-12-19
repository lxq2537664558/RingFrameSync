#ifndef __RING_H
#define __RING_H
/************************************************************************/
/* ���ƣ�Ring
** ���ߣ�����
** ʱ�䣺2016-12-16
** ������һ���Զ�������ݽṹ���ýṹ��Ҫ�������֡ͬ��������������
** ������FIFO����Ring����״̬�¿�ʼ���������Ҹ���Ȥ����Ϣ��
** �������ܣ�StepInһ�����ݻ�ˢ�µ����������ݣ����ڲ��Ҹ���Ȥ������
** ��ǿ���ܣ�����Stored״̬��StepInʱ���ˢ�������ݱ���������ͨ��BackTo��
** �Է��ص�֮ǰ��ĳ��״̬��������StepIn�ֻ����ǰˢ���������Զ�������
** ʵ��ʱ�⵹����Ч����
** ��ע��ֻ��Ring�������ݺ����������ʼ������							*/
/************************************************************************/



//#define COMPLEX_TYPE //����˴���Ϊ����ģʽ

#ifdef COMPLEX_TYPE
	typedef struct Complex RING_TYPE;
#else
	typedef int RING_TYPE;
#endif // COMPLEX_TYPE


static enum MyEnum
{
	EMPTY, //û������
	IDLE,  //���ݲ������޷�����
	FIND,  //���ݴ�������������
	STORED //���ݴ������߲�ߴ�����
};


struct RING
{
	RING_TYPE *BaseDataAddr;
	int front;
	int rear;
	int MaxSize;
	int WorkMode;
	int Status; //0->�� 1->������������״̬�� other->���ݲ��������ܹ���
};
typedef struct RING *pRing;

char CreateRing(pRing pBuff, const int BuffSize);  //����һ��Ring������ΪBuffSize
void DestoryRing(pRing pBuff);	//����Ring��������Դ
void ResetRing(pRing pBuff);		//����Ring������Ϊ�գ���ջΪ��
char InQueue(pRing pBuff, const RING_TYPE *DataIn);	//�����ݴ���Ring����
char StepIn(pRing pBuff, const RING_TYPE *DataIn);	//����Ѱ��ģʽ��һ���жϵõ�һ����
char BackTo(pRing pBuff, int BackSize);	//����Stack���ص�֮ǰ��ĳ��״̬(����������Ҳͬ����Stack����)


char ReadOne(pRing pBuff, const RING_TYPE *DataIn, RING_TYPE *DataOut); //һ�ζ�ȡһ����
char ReadAll(pRing pBuff, RING_TYPE *DataOut); //��Ring�вд������һ�ζ���
char StatusJudge(pRing pBuff);


// �û����ú���
char DelayCorrWorking(pRing pBuff);
char CenterCorrWorking(pRing pBuff);



#endif
