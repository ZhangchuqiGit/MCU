
#ifndef __debug_H
#define __debug_H

#include "sys.h"
#include "usart.h"  
#include <stdio.h>
#include <stdlib.h>

#if 1
// _debug_log_info(" ")
#define _debug_log_info(fmt, ...)  { \
printf(">！>！> %d  %s()  INFO: ", __LINE__, __func__); \
printf(fmt, ##__VA_ARGS__); printf("\r\n");   }
#elif 1
// _debug_log_info(" ")
#define _debug_log_info(fmt, ...)  { \
printf(">！>！> %d\tINFO: ", __LINE__); \
printf(fmt, ##__VA_ARGS__); printf("\n");   }
#else
// _debug_log_info(" ")
#define _debug_log_info(fmt, ...)  {}
#endif

#endif

