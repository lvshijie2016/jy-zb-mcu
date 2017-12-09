#include "log.h"


static char output[256]={0};
void dbg_print_log(const char *msg,
            const char *pccFunc,   
            const int   ciLine,
            const char *pccFmt,...)
{
	
	va_list arglst;
	va_start(arglst,pccFmt);
	vsnprintf(output,sizeof(output),pccFmt,arglst);
	printf("[%s][%s():%04d] %s\n",msg, pccFunc, ciLine, output);
	va_end(arglst);
	
}




void dbg_print_buf(const char *msg, ...)
{
	uint16_t i;
	uint8_t* buf;
	uint32_t cnt;
	va_list arglst;
	va_start(arglst, msg);
	buf = va_arg( arglst, uint8_t*);        
	cnt = va_arg( arglst, uint32_t);

	printf("[%s][L:%d] ", msg,cnt);
	for(i=0;i<cnt;i++)
	{
			printf("0x%02X,",buf[i]);
	}
	printf("\n");
	va_end(arglst);
}


