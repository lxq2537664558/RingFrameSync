#ifndef __Stack_H
#define __Stack_H
#define Stack_SIZE 256
typedef int StackType;/* ��ջ���洢��ֵ���������� */

//void create_Stack(int size);
//void destroy_Stack(void);
void push(StackType *DataIn);
void pop(StackType *DataOut);
StackType top(void);
char is_empty(void);
char is_full(void);
void ClearStack();
#endif
