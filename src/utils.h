#ifndef _UTILS_H_
#define _UTILS_H_

#include "../include/GL/gl3w.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define DEBUG 0

#define LOG_D(format, args...)                                        \
    if (DEBUG)                                                        \
    {                                                                 \
        printf("%s:%d " format "\n", __FUNCTION__, __LINE__, ##args); \
    }

#endif
