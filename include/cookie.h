#include <fcgi_stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdio.h>

#define COOKIE_PRESENT 1
#define COOKIE_ABSENT 0


int get_cookie(FCGX_Request *request ){
    char * cookies=FCGX_GetParam("HTTP_Cookie",request->envp);

    if(NULL!=(cookies)){
        return COOKIE_PRESENT;
    }
    else{
        return COOKIE_ABSENT;
    }
}



void set_cookie(FCGX_Request *request,char *key,char *value,int cookieTimeout){
    FCGX_FPrintF((*request).out, 
		"Content-type: text/html\r\n"
		"Set-Cookie: %s = %s;Max-Age = %d\r\n", key, value, cookieTimeout);
}