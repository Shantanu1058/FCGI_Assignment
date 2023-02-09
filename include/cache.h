#include <libmemcached/memcached.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

memcached_server_st *servers = NULL;
memcached_st *memc;
memcached_return rc;

FILE *filePointer;
void init_mem(){

    filePointer=fopen("/etc/nginx/error.log","w");
    
    memc=memcached_create(NULL);
    servers = memcached_server_list_append(servers, "localhost", 11211, &rc);
    rc = memcached_server_push(memc, servers);

    if (rc == MEMCACHED_SUCCESS)
        fprintf(filePointer, "ERROR: Added server successfully\n");
    else
        fprintf(filePointer, "ERROR: %s\n", memcached_strerror(memc, rc));


    fclose(filePointer);    
}


void insert_mem(char *key,char *value){

    filePointer=fopen("/etc/nginx/error.log","w");
    
    rc = memcached_set(memc, key, strlen(key), value, strlen(value), (time_t)0, (uint32_t)0);

    if (rc == MEMCACHED_SUCCESS)
        fprintf(filePointer, "Key-Value Pair Created\n");
    else
        fprintf(filePointer, "ERROR: %s\n", memcached_strerror(memc, rc));

    fclose(filePointer); 

}

char* search_mem(char*key){

    char *res;
    size_t value_length;
    uint32_t flags;
    res = memcached_get(memc, key, strlen(key), &value_length, &flags, &rc);
    if(rc!=MEMCACHED_SUCCESS){
        return NULL;
    }
    else{
        return res;
    }

    
}
