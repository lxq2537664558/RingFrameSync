#ifndef __FRAMESYNC_H
#define __FRAMESYNC_H

typedef int FrameType;



struct LComplex
{
	long long Re;
	long long Im;
};




enum	//帧同步算法的四种状态
{
	INITIAL,	//Ring不满，填充状态
	DELAYWORK,		//Ring正常工作，延迟相关算法没有找到同步头
	LOCK,		//连续4次延迟相关都对了，以后直接开启中心对称同步（连续10次中心对称失败，返回FIND）
	DATA		//同步完成，直接输出数据
};

struct FrameSyncPara //帧同步算法的所有全局变量参数
{
	char Status; //算法处于什么状态？
	long RankBios; //当前点的偏移量（0:新一帧起始点， <0上一帧的位置)
	int PeakValue; //极大值
	int PeakRank; //极大值出现的位置
	int DataCnt; //已发送数据量统计(0 ~ 308*128)
	//int HistoryWriteFlag; //是否需要回写数据(可根据BackWriteLen是否为零来判断)
	int BackWriteLen; //每次回写的个数
	int DataRq; //本次运行需要的数据量，如不够直接返回。
	int OutEnable; //输出使能，延迟相关成功后不能输出数据，数据直接丢弃。
};

struct StatisticsPara
{
	int TransTime; //成功传输多少帧数据
	int DelayCorrTime; //延迟对称连续成功次数（4次以后切换模式并自动清0，失败自动清0）
	int LossCnt; //中心对称连续丢失次数
};


char InitFrameSync(void);
char RunFrameSync(FrameType *DataIn);
char StopFrameSync(void);
#endif
