#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include <string.h>
#include <include/separator.h>
#include <include/connect.h>
#include <include/configuration.h>
config_t cfg;
int main(){
    FCGX_Init();
    cfg=configuration_init("config.cfg");
    FCGX_Request Request;
	FCGX_InitRequest(&Request, 0, 0);
    while (FCGX_Accept_r(&Request) >= 0)
	{

        char *queryString = FCGX_GetParam("QUERY_STRING", Request.envp);
        char **token=seperater(queryString);

        if(strlen(queryString)==0){
            FCGX_FPrintF(
				Request.out,
				"Content-type: text/html\r\n\r\n"
				"<h1>Hello.</h1>"
				"<p> Hello World </p>");
        }
        else{
            
            if(strlen(token[2])==0){
                char *res=search_db(getDBUsername(cfg),getDBPassword(cfg),token[0]);

                if(res==NULL){
                    FCGX_FPrintF(
							Request.out,
							"Content-type: text/html\r\n\r\n"
							"<h1>Hello.</h1>"
							"<p> Link Not Found </p>");
                }
                else{
                    FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", res);
                    
                }
                free(token);
                free(res);
            }
            else{
                insert_db(getDBUsername(cfg),getDBPassword(cfg),token[0],token[2]);
                FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", token[2]);
                free(token);
            }
        }



        FCGX_Finish_r(&Request);

    }
}