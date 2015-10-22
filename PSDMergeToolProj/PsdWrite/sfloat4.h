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

#ifndef SFLOAT4_H
#define SFLOAT4_H

#include "float_base.h"
#include "int_base.h"
#include <math.h>

///==========================================================================///
///  non test function                                                       ///
///==========================================================================///
inline sfloat4 SFloat4(float _x, float _y, float _z, float _w)
{
    sfloat4 ret = {_x, _y, _z, _w};
    return ret;
}

inline sfloat4 SFloat4(float _x)
{
    sfloat4 ret = {_x, _x, _x, _x};
    return ret;
}

inline void SFloat4_set_x(sfloat4* f4, float x)
{
    ((float*)f4)[0] = x;
}

inline void SFloat4_set_y(sfloat4* f4, float y)
{
    ((float*)f4)[1] = y;
}

inline void SFloat4_set_z(sfloat4* f4, float z)
{
    ((float*)f4)[2] = z;
}

inline void SFloat4_set_w(sfloat4* f4, float w)
{
    ((float*)f4)[3] = w;
}

inline float SFloat4_get_x(const sfloat4* f4)
{
    return ((float*)f4)[0];
}

inline float SFloat4_get_y(const sfloat4* f4)
{
    return ((float*)f4)[1];
}

inline float SFloat4_get_z(const sfloat4* f4)
{
    return ((float*)f4)[2];
}

inline float SFloat4_get_w(const sfloat4* f4)
{
    return ((float*)f4)[3];
}

inline void SFloat4_export(sfloat4 f4, float* fout)
{
    fout[0] = f4.x;
    fout[1] = f4.y;
    fout[2] = f4.z;
    fout[3] = f4.w;
}

inline EFloat4 SFloat4_convert(sfloat4 f4)
{
    EFloat4 ret;
    SFloat4_export(f4, (float*)&ret);
    return ret;
}

///==========================================================================///
///  SFloat4_convert_int4                                                    ///
///==========================================================================///

inline sint4 SFloat4_convert_int4(sfloat4 f4)
{
    sint4 ret = {(int)f4.x, (int)f4.y, (int)f4.z, (int)f4.w};
    return ret;
}

inline bool test_SFloat4_convert_int4()
{
    sfloat4 f = SFloat4(1.0f, 2.0f, 3.0f, 4.0f);
    sint4 i = SFloat4_convert_int4(f);
    int* ip = (int*)&i;
    if (ip[0] == 1 &&
        ip[1] == 2 &&
        ip[2] == 3 &&
        ip[3] == 4)
        return true;
    else
        return false;
}

///==========================================================================///
///  SFloat4_convert_db4                                                     ///
///==========================================================================///

inline sdouble4 SFloat4_convert_db4(sfloat4 f4)
{
    sdouble4 ret = {(double)f4.x, (double)f4.y, (double)f4.z, (double)f4.w};
    return ret;
}

///==========================================================================///
///  SFloat4_max                                                             ///
///==========================================================================///

inline sfloat4 SFloat4_max(sfloat4 f4_0, sfloat4 f4_1)
{
    sfloat4 ret =
    {
        f4_0.x > f4_1.x ? f4_0.x : f4_1.x,
        f4_0.y > f4_1.y ? f4_0.y : f4_1.y,
        f4_0.z > f4_1.z ? f4_0.z : f4_1.z,
        f4_0.w > f4_1.w ? f4_0.w : f4_1.w
    };
    return ret;
}


///==========================================================================///
///  SFloat4_min                                                             ///
///==========================================================================///

inline sfloat4 SFloat4_min(sfloat4 f4_0, sfloat4 f4_1)
{
    sfloat4 ret =
    {
        f4_0.x < f4_1.x ? f4_0.x : f4_1.x,
        f4_0.y < f4_1.y ? f4_0.y : f4_1.y,
        f4_0.z < f4_1.z ? f4_0.z : f4_1.z,
        f4_0.w < f4_1.w ? f4_0.w : f4_1.w
    };
    return ret;
}

inline bool test_SFloat4_min()
{
    sfloat4 ft0 = SFloat4(1.0f, 2.0f, 3.0f, 4.0f);
    sfloat4 ft1 = SFloat4(4.0f, 3.0f, 2.0f, 1.0f);
    sfloat4 t = SFloat4_min(ft0, ft1);
    float* fp = (float*)&t;
    if (float_equal_float(fp[0], 1.0f) &&
        float_equal_float(fp[1], 2.0f) &&
        float_equal_float(fp[2], 2.0f) &&
        float_equal_float(fp[3], 1.0f))
        return true;
    else
        return false;
}

///==========================================================================///
///  SFloat4_assign                                                          ///
///==========================================================================///

inline sfloat4 SFloat4_assign(sfloat3 f3)
{
    sfloat4 ret = {f3.x, f3.y, f3.z, 1.0f};
    return ret;
}

inline bool test_SFloat4_assign()
{
    sfloat3 ft3;
    float* fp = (float*)&ft3;
    fp[0] = 1.0f;
    fp[1] = 2.0f;
    fp[2] = 3.0f;
    sfloat4 ft4 = SFloat4_assign(ft3);
    fp = (float*)&ft4;
    if (float_equal_float(fp[0], 1.0f) &&
        float_equal_float(fp[1], 2.0f) &&
        float_equal_float(fp[2], 3.0f) &&
        float_equal_float(fp[3], 1.0f))
        return true;
    else
        return false;
}

///==========================================================================///
///  SFloat4_assign                                                          ///
///==========================================================================///

inline sfloat4 SFloat4_assign(sfloat3 f3, float w)
{
    sfloat4 ret = {f3.x, f3.y, f3.z, w};
    return ret;
}

inline bool test_SFloat4_assign2()
{
    sfloat3 ft3;
    float* fp = (float*)&ft3;
    fp[0] = 1.0f;
    fp[1] = 2.0f;
    fp[2] = 3.0f;
    sfloat4 ft4 = SFloat4_assign(ft3, 4.0f);
    fp = (float*)&ft4;
    if (float_equal_float(fp[0], 1.0f) &&
        float_equal_float(fp[1], 2.0f) &&
        float_equal_float(fp[2], 3.0f) &&
        float_equal_float(fp[3], 4.0f))
        return true;
    else
        return false;
}

///==========================================================================///
///  SFloat4_assign                                                          ///
///==========================================================================///

inline sfloat4 SFloat4_assign(sfloat2 f2, float z, float w)
{
    sfloat4 ret = {f2.x, f2.y, z, w};
    return ret;
}

inline bool test_SFloat4_assign3()
{
    sfloat2 ft2;
    float* fp = (float*)&ft2;
    fp[0] = 1.0f;
    fp[1] = 2.0f;
    sfloat4 ft4 = SFloat4_assign(ft2, 3.0f, 4.0f);
    fp = (float*)&ft4;
    if (float_equal_float(fp[0], 1.0f) &&
        float_equal_float(fp[1], 2.0f) &&
        float_equal_float(fp[2], 3.0f) &&
        float_equal_float(fp[3], 4.0f))
        return true;
    else
        return false;
}


///==========================================================================///
///  SFloat4_add                                                             ///
///==========================================================================///

inline sfloat4 SFloat4_add(sfloat4 f4_0, sfloat4 f4_1)
{
    sfloat4 ret =
    {
        f4_0.x + f4_1.x,
        f4_0.y + f4_1.y,
        f4_0.z + f4_1.z,
        f4_0.w + f4_1.w
    };
    return ret;
}

///==========================================================================///
///  SFloat4_sub                                                             ///
///==========================================================================///

inline sfloat4 SFloat4_sub(sfloat4 f4_0, sfloat4 f4_1)
{
    sfloat4 ret =
    {
        f4_0.x - f4_1.x,
        f4_0.y - f4_1.y,
        f4_0.z - f4_1.z,
        f4_0.w - f4_1.w
    };
    return ret;
}

///==========================================================================///
///  SFloat4_mul                                                             ///
///==========================================================================///

inline sfloat4 SFloat4_mul(sfloat4 f4_0, sfloat4 f4_1)
{
    sfloat4 ret =
    {
        f4_0.x * f4_1.x,
        f4_0.y * f4_1.y,
        f4_0.z * f4_1.z,
        f4_0.w * f4_1.w
    };
    return ret;
}

///==========================================================================///
///  SFloat4_div                                                             ///
///==========================================================================///

inline sfloat4 SFloat4_div(sfloat4 f4_0, sfloat4 f4_1)
{
    sfloat4 ret =
    {
        f4_0.x / f4_1.x,
        f4_0.y / f4_1.y,
        f4_0.z / f4_1.z,
        f4_0.w / f4_1.w
    };
    return ret;
}

///==========================================================================///
///  SFloat4_xyz                                                             ///
///==========================================================================///

inline sfloat3 SFloat4_xyz(sfloat4 f4)
{
    sfloat3 ret =
    {
        f4.x,
        f4.y,
        f4.z,
        0.0f
    };
    return ret;
}

///==========================================================================///
///  SFloat4_xy                                                              ///
///==========================================================================///

inline sfloat2 SFloat4_xy(sfloat4 f4)
{
    sfloat2 ret =
    {
        f4.x,
        f4.y,
    };
    return ret;
}

///==========================================================================///
///  SFloat4_yxzw                                                            ///
///==========================================================================///

inline sfloat4 SFloat4_yxzw(const sfloat4 f4)
{
    sfloat4 ret =
    {
        f4.y,
        f4.x,
        f4.z,
        f4.w
    };
    return ret;
}

///==========================================================================///
///  SFloat4_wwww                                                            ///
///==========================================================================///

inline sfloat4 SFloat4_wwww(sfloat4 f4)
{
    sfloat3 ret =
    {
        f4.w,
        f4.w,
        f4.w,
        f4.w
    };
    return ret;
}

#endif