#include <include/separator.h>
#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include <string.h>
struct Person{
    char *userName;
    double randn;
    char *link;
};

struct Person* g_database[100];
int g_size=0;

 void insert(char *userName,char *link){
    struct Person *p= (struct Person*) malloc(sizeof(struct Person));
    p->userName=userName;
    p->link=link;

    g_database[g_size]=p;
    g_size++;
}

char* search(char * userName){
    for(int i=0;i<g_size;++i){
        if(!strcmp(g_database[i]->userName,userName)){
            return g_database[i]->link;
        }
    }
    return NULL;
}
int main(){
    FCGX_Init();
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
                char *res=search(token[0]);

                if(res==NULL){
                    FCGX_FPrintF(
							Request.out,
							"Content-type: text/html\r\n\r\n"
							"<h1>Hello.</h1>"
							"<p> Link Not Found </p>");
                }
                else{
                    FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", res);
                    free(token);
                }
            }
            else{
                insert(token[0],token[2]);
                FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", token[2]);
                free(token);
            }
        }



        FCGX_Finish_r(&Request);

    }
}