#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include <pthread.h>
#include <include/connect.h>
#include <include/separator.h>
#include <include/cache.h>
#include <include/cookie.h>

pthread_mutex_t lock;
void *requestHandler(void *iid)
{
	int *id = (int *)iid;

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

			// FCGX_FPrintF(
			// 	Request.out,
			// 	"Content-type: text/html\r\n\r\n"
			// 	"<h1>Hello.</h1>"
			// 	"<p> Query: %d %s %s</p>",
			// 	strlen(token[2]),
			// 	token[2],
			// 	queryString);

			// free(token);

			// free(queryString);

			if (strlen(token[2]) == 0)
			{

				pthread_mutex_lock(&lock);

				char *res_mem = search_mem(token[0]);
				char *res_db = search_db(token[0]);

				pthread_mutex_unlock(&lock);

				if (res_db != NULL && res_mem == NULL)
				{
					if (!startsWith("https", res_db))
					{
						FCGX_FPrintF(
							Request.out,
							"Content-type: text/html\r\n\r\n"
							"<h1>Hello.</h1>"
							"<p> Url is not stored properly in the database </p>");
					}
					else
					{
						pthread_mutex_lock(&lock);
						insert_mem(token[0], res_db);

						FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", res_db);

						free(res_db);

						free(token);
						pthread_mutex_unlock(&lock);
					}
				}
				else if (res_mem != NULL)
				{
					if (!startsWith("https", res_mem))
					{
						FCGX_FPrintF(
							Request.out,
							"Content-type: text/html\r\n\r\n"
							"<h1>Hello.</h1>"
							"<p> Url is not stored properly in the database </p>");
					}
					else
					{
						FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", res_mem);
						free(token);
					}
				}
				else
				{
					FCGX_FPrintF(
						Request.out,
						"Content-type: text/html\r\n\r\n"
						"<h1>Hello.</h1>"
						"<p> No Url Found </p>");
				}
			}
			else
			{
				if (!startsWith("https", token[2]))
				{
					FCGX_FPrintF(
						Request.out,
						"Content-type: text/html\r\n\r\n"
						"<h1>Hello.</h1>"
						"<p> Please Enter Proper Url</p>");
				}
				else
				{

					pthread_mutex_lock(&lock);

					insert_db(token[0], token[2]);
					insert_mem(token[0], token[2]);

					set_cookie(&Request,token[0],token[2],60*60*24);

					FCGX_FPrintF(Request.out, "HTTP/1.1 302\r\nLocation: %s\r\n\r\n", token[2]);

					free(token);

					pthread_mutex_unlock(&lock);
				}
			}
		}
	}

	FCGX_Finish_r(&Request);
}

int main()
{
	pthread_t th[8];
	pthread_mutex_init(&lock, NULL);
	for (int i = 0; i < 8; ++i)
	{
		pthread_create(&th[i], NULL, requestHandler, (void *)th);
		pthread_join(th[i], NULL);
	}

	pthread_mutex_destroy(&lock);

	pthread_exit(NULL);

	return 0;
}