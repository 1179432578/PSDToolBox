/*
 * Copyright (c) 2014, 徐海宁
 * All rights reserved.
 *
 * 文件名称：
 * 文件描述：
 *
 * 当前版本：0.2
 * 作    者：徐海宁
 * 完成日期：2014年8月8日
 * 修改日期：2014年8月8日
 */
#ifndef INT_BASE_H
#define INT_BASE_H

typedef struct __m128i_t_
{
	int x, y, z, w;
} __m128i_t;

typedef __m128i_t* v128i;

typedef __m128i_t sint2;
typedef __m128i_t sint3;
typedef __m128i_t sint4;

#endif
