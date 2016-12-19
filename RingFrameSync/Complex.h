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

struct Complex AddComplex(struct Complex a, struct Complex b);		//�ӷ�
struct Complex SubComplex(struct Complex a, struct Complex b);		//���� a - b
struct Complex MultiComplex(struct Complex a, struct Complex b);	//�˷�
struct Complex ConjComplex(struct Complex a);		//����
COMPLEX_TYPE ModComplex(struct Complex a);	//��ģ
COMPLEX_TYPE NormComplex(struct Complex a);  //ģ��ƽ��
void AssignComplex(struct Complex *DataOut, COMPLEX_TYPE Re, COMPLEX_TYPE Im); //��ֵ
#endif