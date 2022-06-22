
#ifndef __debug_H
#define __debug_H

#include "sys.h"
#include "usart.h"  
#include <stdio.h>
#include <stdlib.h>

#if 0

// _debug_log_info(" ")
#define _debug_log_info(fmt, ...)  { \
printf(">-> %d\t%s()\tINFO: ", __LINE__, __func__); \
printf(fmt, ##__VA_ARGS__); printf("\r\n"); }

#elif 0

// _debug_log_info(" ")
#define _debug_log_info(fmt, ...)  { \
printf(">-> %d\tINFO: ", __LINE__); \
printf(fmt, ##__VA_ARGS__); printf("\r\n");   }

#else

// _debug_log_info(" ")
#define _debug_log_info(fmt, ...)  ;

#define NO_Debug

#endif


#if 0
// _debug_log_info_c(" ")
#define _debug_log_info_c(fmt, ...)  { printf(fmt, ##__VA_ARGS__); }
#else
// _debug_log_info_c(" ")
#define _debug_log_info_c(fmt, ...)  ;
#endif


#endif

