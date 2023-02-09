#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <stdbool.h>
char g_uname[100];
char g_radn[100];
char g_url[300];


bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}


char** seperater(char *q)
{
    char **var =(char **)malloc(3 * sizeof(char *));
    
    int cnt=1;
    for (int i = 0; i < 3; i++)
    {
        var[i] = (char *)malloc(200 * sizeof(char));
    }
    while(*q != '\0')
    {
        if(*q == '='  && cnt ==1)
        {
            int k=0;
            q++;
            while(*q != '&')
            {
                g_uname[k++] = *q ;
                q++;
                 if(*q == '\0')
                    break;
            }
            g_uname[k]='\0';
            strcpy(var[0],g_uname);
            //  printf("%s\n",var[0]);
            cnt ++;
        }
         if(*q == '='  && cnt ==2)
        {
            int k=0;
            q++;
            while(*q != '&')
            {
                g_radn[k++]=*q ;
                q++;
                 if(*q == '\0')
                    break;
            }
              g_radn[k]='\0';
              strcpy(var[1],g_radn);
            //  printf("%s\n",var[1]);
            cnt ++;
        }
         if(*q == '='  && cnt ==3)
        {
            int k=0;
            q++;
            while(*q != '&')
            {
                g_url[k++]=*q ;
                q++;
                 if(*q == '\0')
            break;
            }
            g_url[k]='\0';

            strcpy(var[2],g_url);
            cnt ++;
        }
        if(*q == '\0')break;
         q++;
    }

    if(cnt==2){
        strcpy(var[1],"\0");
        strcpy(var[2],"\0");
    }
        return var;
}


