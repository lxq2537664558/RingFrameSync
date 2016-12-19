#include "FrameSync.h"
#include "Ring.h"
#include "Stack.h"
#include <assert.h>



struct FrameSyncPara MainPara;
struct StatisticsPara StatPara;
int DataInLen = 100; //本次有多少数据输入
static struct Complex HistoryData[300]; //最后的256个数据需要返回去判断
struct RING SpathBuff;
struct RING *pBuff = &SpathBuff; //指向SpathBuff


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

	CreateRing(pBuff, 1664); //创建一个
}

char RunFrameSync(struct Complex *DataIn)
{
	int i;
	char Mode = MainPara.Status;
	int InLen = DataInLen; //获取前一级缓存中已经存在的数据量

	/*缓存值*/
	int pt0, pt1, pt2, pt3, pt4, pt5, pt6, pt7;
	struct Complex Tmp0, Tmp1, Tmp2, Tmp3, Tmp4, Tmp5, Tmp6, Tmp7;
	long long DelayValue; 
	long long PowerIn, PowerOut, DiffPower;
	struct Complex Tmp;


	while (DataInLen >= MainPara.DataRq) //只要数据够，则一直执行下去
	{
		DataInLen -= MainPara.DataRq;


		switch (Mode)
		{
		case DATA:
			//1.先把同步多余的数据发送 


			//2.再直接发送其余数据并计数 
			for (i = 0; i < MainPara.DataRq; i++)
			{

			}
			//3.最后256个数据同步记录,之后更新所有状态
			if (MainPara.DataCnt >= 308 * 128 - MainPara.BackWriteLen)
			{
				for (i = 0; i < MainPara.BackWriteLen; i++)
				{
					HistoryData[i] = DataIn[i];
					//输出到OuterBuff
				}
				MainPara.DataRq = 1664 - MainPara.BackWriteLen;
				MainPara.DataCnt = 0;
				MainPara.Status = INITIAL;
			}
			break;
		case INITIAL:
			//1. 先把最后HistoryData中256个数据存入
			for (i = 0; i < MainPara.BackWriteLen; i++)//需要回写的话
			{
				InQueue(pBuff, (RING_TYPE *)HistoryData);
			}
			MainPara.BackWriteLen = 0;			
			//2. 再存入1664-256个数，完成初始化过程
			for (i = 0; i < MainPara.DataRq; i++)
			{
				InQueue(pBuff, DataIn);
			}
			MainPara.Status = DELAYWORK;
			MainPara.DataRq = 256;
			break;
		case DELAYWORK:
			// 直接调用延迟相关，除了第一次，应该每次都是在差不多的地方同步上的
			for (i = 0; i < MainPara.DataRq; i++) //必然是256次查找求值
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

				Tmp.Re = 1; //把那个核心步骤写进去
				Tmp.Im = 1;
				DelayValue = Mod(Tmp);
				DelayValue *= (1664 / DiffPower); //1.得到延迟同步值

				if (DelayValue > CorrTh)
				{
					MainPara.PeakValue = DelayValue;
					MainPara.PeakRank = MainPara.RankBios;
				}

				//2. 连续三次在同一个地方同步上，则进入锁定环节
			}
			//3.所有值都求完后进行统计与判断
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
			//1. 直接调用中心对称相关，应该也是在差不多的地方同步上
			//2. 连续16次同步不上，则返回DELAYWORK
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