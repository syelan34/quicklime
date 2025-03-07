#pragma once

#include "defines.h"
#include <3ds.h>
#include <stdio.h>

#if DEBUG
#define ASSERT(condition, msg, args...)                                 \
	do {                                                                \
		if (!(condition)) {                                             \
		    char buf[255] = {};                                         \
			snprintf(buf, 255, msg, ##args);                            \
			printf("Assertion failed: %s\nIn function: %s\n%s:%u\n%s",  \
				   #condition, __func__, __FILE_NAME__, __LINE__, buf); \
			for (;;)                                                    \
				;                                                       \
		}                                                               \
	} while (0)
#else
#define ASSERT(condition, msg) (void)(condition)
#endif