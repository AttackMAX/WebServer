/*
 * @Author: AttackMAX 2646479700@qq.com
 * @Date: 2024-12-04 13:47:50
 * @LastEditors: AttackMAX 2646479700@qq.com
 * @LastEditTime: 2024-12-06 14:53:34
 *
 * Copyright (c) 2024 by ※ AttackMAX ※, All Rights Reserved.
 */
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void errif(bool condition, const char *errmsg)
{
    if (condition)
    {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}