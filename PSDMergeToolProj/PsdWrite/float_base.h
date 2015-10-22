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

#ifndef FLOAT_BASH_H
#define FLOAT_BASH_H

class EFloat
{
public:
	float x;
public:
	EFloat()
		: x(0.0f)
	{}
	EFloat(float _x)
		: x(_x)
	{}
	inline EFloat operator *(const EFloat& f) const {
		return EFloat(x * f.x);
	}
	inline EFloat operator *(float f) const {
		return EFloat(x * f);
	}
	inline EFloat operator /(const EFloat& f) const {
		return EFloat(x / f.x);
	}
	inline EFloat operator +(const EFloat& f) const {
		return EFloat(x + f.x);
	}
	inline EFloat operator -(const EFloat& f) const {
		return EFloat(x - f.x);
	}
	inline float Get() const {
		return x;
	}
};

class EFloat2
{
public:
	float x;
	float y;
public:
	EFloat2()
		: x(0.0f)
	    , y(0.0f)
	{}
	EFloat2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}
	inline EFloat2 operator *(const EFloat2& f) const {
		return EFloat2(x * f.x, y * f.y);
	}
	inline EFloat2 operator *(float f) const {
		return EFloat2(x * f, y * f);
	}
	inline EFloat2 operator /(const EFloat2& f) const {
		return EFloat2(x / f.x, y / f.y);
	}
	inline EFloat2 operator +(const EFloat2& f) const {
		return EFloat2(x + f.x, y + f.y);
	}
	inline EFloat2 operator -(const EFloat2& f) const {
		return EFloat2(x - f.x, y - f.y);
	}
};

class EFloat3
{
public:
	float x;
	float y;
	float z;
public:
	EFloat3()
		: x(0.0f)
		, y(0.0f)
	    , z(0.0f)
	{}
	EFloat3(float _x, float _y, float _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{}
	inline EFloat3 operator *(const EFloat3& f) const {
		return EFloat3(x * f.x, y * f.y, z * f.z);
	}
	inline EFloat3 operator *(float f) const {
		return EFloat3(x * f, y * f, z * f);
	}
	inline EFloat3 operator /(const EFloat3& f) const {
		return EFloat3(x / f.x, y / f.y, z / f.z);
	}
	inline EFloat3 operator +(const EFloat3& f) const {
		return EFloat3(x + f.x, y + f.y, z + f.z);
	}
	inline EFloat3 operator -(const EFloat3& f) const {
		return EFloat3(x - f.x, y - f.y, z - f.z);
	}
};

class EFloat4
{
public:
	float x;
	float y;
	float z;
    float w;
public:
	EFloat4()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
	    , w(0.0f)
	{}
	EFloat4(const EFloat3& nor, float w) 
		: x(nor.x)
		, y(nor.y)
		, z(nor.z)
		, w(w)
	{}
	EFloat4(float _x, float _y, float _z, float _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{}
	inline EFloat4 operator *(const EFloat4& f) const {
        return EFloat4(x * f.x, y * f.y, z * f.z, w * f.w);
	}
	inline EFloat4 operator *(float f) const {
		return EFloat4(x * f, y * f, z * f, w * f);
	}
	inline EFloat4 operator /(const EFloat4& f) const {
		return EFloat4(x / f.x, y / f.y, z / f.z, w / f.w);
	}
	inline EFloat4 operator +(const EFloat4& f) const {
		return EFloat4(x + f.x, y + f.y, z + f.z, w + f.w);
	}
	inline EFloat4 operator -(const EFloat4& f) const {
		return EFloat4(x - f.x, y - f.y, z - f.z, w - f.w);
	}
};

class VRect
{
public:
    float left;
    float top;
    float right;
    float bottom;
public:
    VRect()
    : left(0.0f)
    , top(0.0f)
    , right(0.0f)
    , bottom(0.0f)
    {}
    VRect(const VRect& rect)
    : left(rect.left)
    , top(rect.top)
    , right(rect.right)
    , bottom(rect.bottom)
    {}
    VRect(float _left,
          float _top,
          float _right,
          float _bottom)
    : left(_left)
    , top(_top)
    , right(_right)
    , bottom(_bottom)
    {}
    inline const VRect& operator = (const VRect& rect) {
        left = rect.left;
        top = rect.top;
        right = rect.right;
        bottom = rect.bottom;
        return *this;
    }
};

class VSize
{
public:
    float width;
    float height;
public:
    VSize()
    : width(0.0f)
    , height(0.0f)
    {}
    VSize(const VSize& size)
    : width(size.width)
    , height(size.height)
    {}
    VSize(float _width,
          float _height)
    : width(_width)
    , height(_height)
    {}
    inline const VSize& operator = (const VSize& size) {
        width = size.width;
        height = size.height;
        return *this;
    }
};

typedef struct __m128_t_
{
    float x, y, z, w;
} __m128_t;

typedef struct __m64_t_
{
    int x, y;
} __m64_t;

typedef struct __sdouble4_t
{
    double x, y, z, w;
} sdouble4;

typedef __m128_t* v128;
typedef __m128_t sfloat2;
typedef __m128_t sfloat3;
typedef __m128_t sfloat4;

bool float_equal_float(float ft0, float ft1);

#endif
