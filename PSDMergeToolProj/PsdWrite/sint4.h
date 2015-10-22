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

#ifndef SINT4_H
#define SINT4_H

#include "int_base.h"
#include "float_base.h"

inline sint4 SInt4(int _x, int _y, int _z, int _w)
{ 
    sint4 ret =
    {
        _x,
        _y,
        _z,
        _w
    };
    return ret;
}

inline int SInt4_get_x(sint4* i4)
{
    return i4->x;
}

inline int SInt4_get_y(sint4* i4)
{
    return i4->y;
}

inline int SInt4_get_z(sint4* i4)
{
    return i4->z;
}

inline int SInt4_get_w(sint4* i4)
{
    return i4->w;
}

/// 这个函数用来计算的应当都是正整数
inline sint4 SInt4_mul(sint4 i4_0, sint4 i4_1)
{
    sint4 ret =
    {
        i4_0.x * i4_1.x,
        i4_0.y * i4_1.y,
        i4_0.z * i4_1.z,
        i4_0.w * i4_1.w
    };
    return ret;
}

inline sint4 SInt4_add(sint4 i4_0, sint4 i4_1)
{
    sint4 ret =
    {
        i4_0.x + i4_1.x,
        i4_0.y + i4_1.y,
        i4_0.z + i4_1.z,
        i4_0.w + i4_1.w
    };
    return ret;
}

inline void SInt4_export(sint4 i4, int* result)
{
    result[0] = i4.x;
    result[1] = i4.y;
    result[2] = i4.z;
    result[3] = i4.w;
}

inline sfloat4 SInt4_convert_ft4(sint4 i4)
{
    sfloat4 ret =
    {
        (float)i4.x,
        (float)i4.y,
        (float)i4.z,
        (float)i4.w
    };
    return ret;
}

#endif