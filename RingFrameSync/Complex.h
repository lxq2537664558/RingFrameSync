/********************************************************************************/
/* Author:		YinChao
/* Date:		2016.11.2
/* Description: Library for complex's mathematical computation
/********************************************************************************/
#ifndef __COMPLEX_H
#define __COMPLEX_H

#include <math.h>

typedef int COMPLEX_TYPE;

struct Complex
{
	COMPLEX_TYPE Re;
	COMPLEX_TYPE Im;
};

struct Complex AddComplex(struct Complex a, struct Complex b);		//加法
struct Complex SubComplex(struct Complex a, struct Complex b);		//减法 a - b
struct Complex MultiComplex(struct Complex a, struct Complex b);	//乘法
struct Complex ConjComplex(struct Complex a);		//共轭
COMPLEX_TYPE ModComplex(struct Complex a);	//求模
COMPLEX_TYPE NormComplex(struct Complex a);  //模的平方
void AssignComplex(struct Complex *DataOut, COMPLEX_TYPE Re, COMPLEX_TYPE Im); //赋值
#endif