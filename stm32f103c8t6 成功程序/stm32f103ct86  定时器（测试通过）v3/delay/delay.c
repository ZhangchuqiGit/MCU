#include "delay.h"



void Delay_Ms(unsigned int uiCount)  //�Ǿ�ȷ��ʱ����Լ1MS
{ 
  
  unsigned int i,j;

  for( j=0;j<uiCount;j++ )
  {  
    for( i=0;i<0x2AFF;i++ )
    {
      ;
    }
  }
} 

void Delay_Us(unsigned int uiCount)  //�Ǿ�ȷ��ʱ����Լ1US
{ 
  
  unsigned int i,j;

  for( j=0;j<uiCount;j++ )
  {  
    for( i=0;i<5;i++ )
    {
      ;
    }
  }
} 
