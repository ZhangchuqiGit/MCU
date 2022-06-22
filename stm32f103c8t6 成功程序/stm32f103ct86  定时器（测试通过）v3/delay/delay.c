#include "delay.h"



void Delay_Ms(unsigned int uiCount)  //非精确延时，大约1MS
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

void Delay_Us(unsigned int uiCount)  //非精确延时，大约1US
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
