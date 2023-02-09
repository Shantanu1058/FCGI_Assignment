#!/bin/bash
gcc phase4.c $(mysql_config --libs) -o output $(mysql_config --cflags) -lfcgi -ggdb -O3 -lmemcached -lpthread
kill -9 `pidof output`
spawn-fcgi -a 127.0.0.1 -p 8080 output