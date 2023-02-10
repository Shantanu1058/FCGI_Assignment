#include <string.h>
#include <libconfig.h>


config_t configuration_init(const char *fileName){
    config_t cfg;
    config_init(&cfg);
    config_read_file(&cfg,fileName);
    return cfg;
}

char * getDBUsername(config_t cfg){
    const char *userName;
    config_lookup_string(&cfg,"username",&userName);
    return (char *) userName;
}

char * getDBPassword(config_t cfg){
    const char * password;
    config_lookup_string(&cfg,"password",&password);
    return (char *) password;
}

int getNumberOfThreads(config_t cfg){
    int threads;
    config_lookup_int(&cfg,"number_of_threads",&threads);

    return (int) threads;
}

int getCookiesTimeout(config_t cfg){
    int timeout;
    config_lookup_int(&cfg,"cookie_timeout",&timeout);

    return (int) timeout; 
}