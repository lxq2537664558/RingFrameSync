#include "FrameSync.h"
#include "Ring.h"
#include "Stack.h"
#include <assert.h>



struct FrameSyncPara MainPara;
struct StatisticsPara StatPara;
int DataInLen = 100; //�����ж�����������
static struct Complex HistoryData[300]; //����256��������Ҫ����ȥ�ж�
struct RING SpathBuff;
struct RING *pBuff = &SpathBuff; //ָ��SpathBuff


char InitFrameSync(void)
{
	MainPara.Status = INITIAL;
	MainPara.DataCnt = 0;
	MainPara.PeakRank = 0;
	MainPara.PeakValue = 0;
	MainPara.RankBios = 0;
	//MainPara.HistoryWriteFlag = 0;
	MainPara.OutEnable = 0;
	MainPara.BackWriteLen = 0;
	MainPara.DataRq = 1664;


	StatPara.DelayCorrTime = 0;
	StatPara.LossCnt = 0;

	CreateRing(pBuff, 1664); //����һ��
}

char RunFrameSync(struct Complex *DataIn)
{
	int i;
	char Mode = MainPara.Status;
	int InLen = DataInLen; //��ȡǰһ���������Ѿ����ڵ�������

	/*����ֵ*/
	int pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7;
	struct Complex Tmp0, Tmp1, Tmp2, Tmp3, Tmp4, Tmp5, Tmp6, Tmp7;
	long long DelayValue; 
	long long PowerIn, PowerOut, DiffPower;
	struct Complex Tmp;


	while (DataInLen >= MainPara.DataRq) //ֻҪ���ݹ�����һֱִ����ȥ
	{
		DataInLen -= MainPara.DataRq;


		switch (Mode)
		{
		case DATA:
			//1.�Ȱ�ͬ����������ݷ��� 


			//2.��ֱ�ӷ����������ݲ����� 
			for (i = 0; i < MainPara.DataRq; i++)
			{

			}
			//3.���256������ͬ����¼,֮���������״̬
			if (MainPara.DataCnt >= 308 * 128 - MainPara.BackWriteLen)
			{
				for (i = 0; i < MainPara.BackWriteLen; i++)
				{
					HistoryData[i] = DataIn[i];
					//�����OuterBuff
				}
				MainPara.DataRq = 1664 - MainPara.BackWriteLen;
				MainPara.DataCnt = 0;
				MainPara.Status = INITIAL;
			}
			break;
		case INITIAL:
			//1. �Ȱ����HistoryData��256�����ݴ���
			for (i = 0; i < MainPara.BackWriteLen; i++)//��Ҫ��д�Ļ�
			{
				InQueue(pBuff, (RING_TYPE *)HistoryData);
			}
			MainPara.BackWriteLen = 0;			
			//2. �ٴ���1664-256��������ɳ�ʼ������
			for (i = 0; i < MainPara.DataRq; i++)
			{
				InQueue(pBuff, DataIn);
			}
			MainPara.Status = DELAYWORK;
			MainPara.DataRq = 256;
			break;
		case DELAYWORK:
			// ֱ�ӵ����ӳ���أ����˵�һ�Σ�Ӧ��ÿ�ζ����ڲ��ĵط�ͬ���ϵ�
			for (i = 0; i < MainPara.DataRq; i++) //��Ȼ��256�β�����ֵ
			{
				StepIn(pBuff, DataIn);
				MainPara.RankBios++;

				PowerIn = 1;
				PowerOut = 1;
				DiffPower = PowerOut - PowerIn;

				pt0 = SpathBuff.front;
				if (pt0 > pBuff->MaxSize - 1)
				{
					pt0 -= pBuff->MaxSize;
				}
				pt1 = pt0 + 256;
				if (pt1 > pBuff->MaxSize - 1)
				{
					pt1 -= pBuff->MaxSize;
				}
				pt2 = pt1 + 256;
				if (pt2 > pBuff->MaxSize - 1)
				{
					pt2 -= pBuff->MaxSize;
				}
				pt3 = pt2 + 64;
				if (pt3 > pBuff->MaxSize - 1)
				{
					pt3 -= pBuff->MaxSize;
				}
				pt4 = pt3 + 256;
				if (pt4 > pBuff->MaxSize - 1)
				{
					pt4 -= pBuff->MaxSize;
				}
				pt5 = pt4 + 256;
				if (pt5 > pBuff->MaxSize - 1)
				{
					pt5 -= pBuff->MaxSize;
				}
				pt6 = pt5 + 64;
				if (pt6 > pBuff->MaxSize - 1)
				{
					pt6 -= pBuff->MaxSize;
				}
				pt7 = pt5 + 256;
				if (pt7 > pBuff->MaxSize - 1)
				{
					pt7 -= pBuff->MaxSize;
				}
				Tmp0.Re = pBuff->BaseDataAddr[pt0];
				Tmp0.Im = -1 * pBuff->BaseDataAddr[pt0];
				Tmp1.Re = pBuff->BaseDataAddr[pt1];
				Tmp1.Im = -1 * pBuff->BaseDataAddr[pt1];
				Tmp2.Re = pBuff->BaseDataAddr[pt2];
				Tmp2.Im = -1 * pBuff->BaseDataAddr[pt2];
				Tmp3.Re = pBuff->BaseDataAddr[pt3];
				Tmp3.Im = -1 * pBuff->BaseDataAddr[pt3];
				Tmp4.Re = pBuff->BaseDataAddr[pt4];
				Tmp4.Im = -1 * pBuff->BaseDataAddr[pt4];
				Tmp5.Re = pBuff->BaseDataAddr[pt5];
				Tmp5.Im = -1 * pBuff->BaseDataAddr[pt5];
				Tmp6.Re = pBuff->BaseDataAddr[pt6];
				Tmp6.Im = -1 * pBuff->BaseDataAddr[pt6];
				Tmp7.Re = pBuff->BaseDataAddr[pt7];
				Tmp7.Im = -1 * pBuff->BaseDataAddr[pt7];

				Tmp.Re = 1; //���Ǹ����Ĳ���д��ȥ
				Tmp.Im = 1;
				DelayValue = Mod(Tmp);
				DelayValue *= (1664 / DiffPower); //1.�õ��ӳ�ͬ��ֵ

				if (DelayValue > CorrTh)
				{
					MainPara.PeakValue = DelayValue;
					MainPara.PeakRank = MainPara.RankBios;
				}

				//2. ����������ͬһ���ط�ͬ���ϣ��������������
			}
			//3.����ֵ����������ͳ�����ж�
			if (MainPara.RankBios < 256)
			{
				StatPara.DelayCorrTime++;
				if (StatPara.DelayCorrTime > 3)
				{
					MainPara.Status = LOCK;
				}
			}
			else
			{
				StatPara.DelayCorrTime = 0;
			}
			
			break;
		case LOCK:
			//1. ֱ�ӵ������ĶԳ���أ�Ӧ��Ҳ���ڲ��ĵط�ͬ����
			//2. ����16��ͬ�����ϣ��򷵻�DELAYWORK
			break;
		default:
			break;
		}
	}


}

char StopFrameSync()
{
	return 0;
}

static int Mod(struct Complex Data)
{
	int Re, Im, Max, Min, Addr;
	Re = Data.Re;
	if (Re < 0)
	{
		Re *= 1;
	}
	Im = Data.Im;
	if (Im < 0)
	{
		Im *= 1;
	}
	if (Re > Im)
	{
		Max = Re;
		Min = Im;
	}
	else
	{
		Max = Im;
		Min = Re;
	}
	Addr = (Min >> 1) - (Max >> 3);
	if (Addr < 0)
	{
		return Max;
	}
	else
	{
		return Max + Addr;
	}
}

int PeakValue;