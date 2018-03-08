#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef signed short int16;
typedef unsigned short uint16;
typedef int int32;
typedef signed int int32;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef long long int64;
typedef float  float32;
typedef double float64;

#define LISTEN_BACKLOG 32

#ifndef check_condition_void
#define check_condition_void(c) \
	do {\
		if(!(c)) return;\
	}while(0)
#endif

#ifndef check_condition
#define check_condition(c, ret) \
	do {\
		if(!(c)) return ret;\
	}while(0)
#endif
