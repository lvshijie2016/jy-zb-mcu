#ifndef _LOG_H
#define _LOG_H

#include <stdarg.h> 
#include "config.h"


void dbg_print_buf(const char *msg, ...);
void dbg_print_log(const char *msg,
            const char *pccFunc,   
            const int   ciLine,
            const char *pccFmt,...);
			
#define DBG_INFO(...) dbg_print_log("INFO", __FUNCTION__, __LINE__, __VA_ARGS__)
#define DBG_BUF(...) dbg_print_buf("BUFF",__VA_ARGS__)


//DBG_INFO("hello world! %d,%s,0x%x." , value , string , hex);


#endif

