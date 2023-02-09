#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include <string.h>
#include <include/cache.h>
#include <include/separator.h>

int main()
{
    FCGX_Init();
    init_mem();
    FCGX_Request Request;
    FCGX_InitRequest(&Request, 0, 0);
    while (FCGX_Accept_r(&Request) >= 0)
    {

        char *queryString = FCGX_GetParam("QUERY_STRING", Request.envp);

        if (strlen(queryString) == 0)
        {
            FCGX_FPrintF(
                Request.out,
                "Content-type: text/html\r\n\r\n"
                "<h1>Hello.</h1>"
                "<p> Hello World </p>");
        }
        else
        {

            char **token = seperater(queryString);

            if (strlen(token[2]) == 0)
            {
                char *res = search_mem(token[0]);

                if (res == NULL)
                {
                    FCGX_FPrintF(
                        Request.out,
                        "Content-type: text/html\r\n\r\n"
                        "<h1>Hello.</h1>"
                        "<p> Link Not Found </p>");
                }
                else
                {
                    FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", res);
                }
                free(token);
            }
            else
            {
                insert_mem(token[0], token[2]);
                FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", token[2]);
                free(token);
            }
        }

        FCGX_Finish_r(&Request);
    }
}