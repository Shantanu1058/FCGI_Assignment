#include <libmemcached/memcached.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include "include/separator.h"

memcached_server_st *servers = NULL;
memcached_st *memc;
memcached_return rc;

static char *host = "localhost";
static char *user = "root";
static char *pass = "Godfather_1058";
static char *db   = "sample";
static char *sk = NULL;
unsigned int port = 3306;
unsigned int flags = 0;


char *ans;
void insert_db( char *username,char *link){
    MYSQL *con;
    con = mysql_init(NULL);
    if (!mysql_real_connect(con, host, user, pass, db, port, sk, flags)) {
        fprintf(stderr, "Error %s (%d)", mysql_error(con), mysql_errno(con));
        exit(1);
    }

    char query[1024];

    sprintf(query,"INSERT INTO db (username,url) VALUES('%s','%s');",username,link);


    printf("%s",query);
    if(mysql_query(con,query)){
        fprintf(stderr, "Error %s (%d)", mysql_error(con), mysql_errno(con));
    }

    mysql_close(con);
}

char *search_db(char *username){
    MYSQL *con;
    con = mysql_init(NULL);
    if (!mysql_real_connect(con, host, user, pass, db, port, sk, flags)) {
        fprintf(stderr, "Error %s (%d)", mysql_error(con), mysql_errno(con));
        exit(1);
    }


    char query[1024];

    sprintf(query,"SELECT url FROM db WHERE username='%s';",username);


    if(mysql_query(con,query)){
        fprintf(stderr, "Error %s (%d)", mysql_error(con), mysql_errno(con));
    }

    MYSQL_RES *res;
    MYSQL_ROW row;

    

    res=mysql_use_result(con);

    printf("%s\n",query);

    ans=(char *) malloc(10*sizeof(char *));
    // int rows=mysql_num_rows(res);

    // printf("%d\n",rows);

    // if(rows==0){
    //     return NULL;
    // }
    int cnt=0;
    while((row=mysql_fetch_row(res))!=NULL){
        strcpy(ans,row[0]);
        // printf("%s",ans);
        cnt++;
    }

    mysql_free_result(res);

    mysql_close(con);

    if(cnt==0){
        return NULL;
    }


    return ans;

}


void init(){
    
    memc=memcached_create(NULL);
    servers = memcached_server_list_append(servers, "localhost", 11211, &rc);
    rc = memcached_server_push(memc, servers);

    if (rc == MEMCACHED_SUCCESS)
        fprintf(stderr, "Added server successfully\n");
    else
        fprintf(stderr, "ERROR: %s\n", memcached_strerror(memc, rc));
}


void insert_mem(char *key,char *value){
    rc = memcached_set(memc, key, strlen(key), value, strlen(value), (time_t)0, (uint32_t)0);

    if (rc == MEMCACHED_SUCCESS)
        fprintf(stderr, "Key-Value Pair Created\n");
    else
        fprintf(stderr, "ERROR: %s\n", memcached_strerror(memc, rc));
}

void * search_mem(void *key){

    init();

    char *res;
    size_t value_length;
    uint32_t flags;
    res = memcached_get(memc, (char *) key, strlen((char *) key), &value_length, &flags, &rc);
    if(rc!=MEMCACHED_SUCCESS){
        return NULL;
    }
    else{
        return (void *) res;
    }

    
}

int main(){
    FILE *filePointer;

    filePointer=fopen("/etc/nginx/error.log","w");

    if(filePointer==NULL){
        printf("Cannot Open File\n");
    }
    else{
        printf("File Opened\n");
        char dataToBeWritten[50]= "Error Log file";
        fputs(dataToBeWritten, filePointer) ;
        fputs("\n", filePointer) ;
        fclose(filePointer);

        printf("File Closed\n");
    }


    return 0;
}