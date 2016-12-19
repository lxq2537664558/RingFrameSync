#ifndef __FRAMESYNC_H
#define __FRAMESYNC_H

typedef int FrameType;



struct LComplex
{
	long long Re;
	long long Im;
};




enum	//֡ͬ���㷨������״̬
{
	INITIAL,	//Ring���������״̬
	DELAYWORK,		//Ring�����������ӳ�����㷨û���ҵ�ͬ��ͷ
	LOCK,		//����4���ӳ���ض����ˣ��Ժ�ֱ�ӿ������ĶԳ�ͬ��������10�����ĶԳ�ʧ�ܣ�����FIND��
	DATA		//ͬ����ɣ�ֱ���������
};

struct FrameSyncPara //֡ͬ���㷨������ȫ�ֱ�������
{
	char Status; //�㷨����ʲô״̬��
	long RankBios; //��ǰ���ƫ������0:��һ֡��ʼ�㣬 <0��һ֡��λ��)
	int PeakValue; //����ֵ
	int PeakRank; //����ֵ���ֵ�λ��
	int DataCnt; //�ѷ���������ͳ��(0 ~ 308*128)
	//int HistoryWriteFlag; //�Ƿ���Ҫ��д����(�ɸ���BackWriteLen�Ƿ�Ϊ�����ж�)
	int BackWriteLen; //ÿ�λ�д�ĸ���
	int DataRq; //����������Ҫ�����������粻��ֱ�ӷ��ء�
	int OutEnable; //���ʹ�ܣ��ӳ���سɹ�����������ݣ�����ֱ�Ӷ�����
};

struct StatisticsPara
{
	int TransTime; //�ɹ��������֡����
	int DelayCorrTime; //�ӳٶԳ������ɹ�������4���Ժ��л�ģʽ���Զ���0��ʧ���Զ���0��
	int LossCnt; //���ĶԳ�������ʧ����
};


char InitFrameSync(void);
char RunFrameSync(FrameType *DataIn);
char StopFrameSync(void);
#endif
