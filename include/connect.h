#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <include/configuration.h>




static char *host = "localhost";
static char *db   = "sample";
static char *sk = NULL;
unsigned int port = 3306;
unsigned int flags = 0;
FILE *filePointer;
char *ans;


void insert_db( char *user,char* pass,char *username,char *link){
    filePointer=fopen("/etc/nginx/error.log","w");
    MYSQL *con;
    con = mysql_init(NULL);
    if (!mysql_real_connect(con, host, user, pass, db, port, sk, flags)) {
        fprintf(filePointer, "Error %s (%d)", mysql_error(con), mysql_errno(con));
        exit(1);
    }

    char query[1024];

    sprintf(query,"INSERT INTO db (username,url) VALUES('%s','%s');",username,link);


    printf("%s",query);
    if(mysql_query(con,query)){
        fprintf(filePointer, "Error %s (%d)", mysql_error(con), mysql_errno(con));
    }

    mysql_close(con);

    fclose(filePointer);
}

char *search_db(char*user,char* pass,char *username){
    filePointer=fopen("/etc/nginx/error.log","w");
    MYSQL *con;
    con = mysql_init(NULL);
    if (!mysql_real_connect(con, host, user, pass, db, port, sk, flags)) {
        fprintf(filePointer, "Error %s (%d)", mysql_error(con), mysql_errno(con));
        exit(1);
    }


    char query[1024];

    sprintf(query,"SELECT url FROM db WHERE username='%s';",username);


    if(mysql_query(con,query)){
        fprintf(filePointer, "Error %s (%d)", mysql_error(con), mysql_errno(con));
    }

    MYSQL_RES *res;
    MYSQL_ROW row;

    ans=(char *) malloc(20*sizeof(char *));

    res=mysql_use_result(con);

    // printf("%s\n",query);

    int cnt=0;
    while((row=mysql_fetch_row(res))!=NULL){
        strcpy(ans,row[0]);
        cnt++;
    }

    mysql_free_result(res);

    mysql_close(con);

    fclose(filePointer);

    if(cnt==0){
        return NULL;
    }


    return ans;

}
