#pragma once

#include "Colors.h"
#include "Utility.h"

class VectorColor
{
public:
    float r, g, b;
public:
    VectorColor()
        :
        r(0.0f),
        g(0.0f),
        b(0.0f)
    {}
    VectorColor(const VectorColor& col)
        :
        r(col.r),
        g(col.g),
        b(col.b)
    {}
    VectorColor(float value)
        :
        r(value),
        g(value),
        b(value)
    {}
    VectorColor(float a_r, float a_g, float a_b)
        :
        r(a_r),
        g(a_g),
        b(a_b)
    {}
    VectorColor(unsigned char a_r, unsigned char a_g, unsigned char a_b)
        :
        r((float)a_r / 255.0f),
        g((float)a_g / 255.0f),
        b((float)a_b / 255.0f)
    {}
    VectorColor(VectorColor col, unsigned char x)
        :
        r(col.r),
        g(col.g),
        b(col.b)
    {}

    // multiplication
    VectorColor& operator =(const VectorColor& VectorColor)
    {
        r = VectorColor.r;
        g = VectorColor.g;
        b = VectorColor.b;
        return *this;
    }
    VectorColor operator*(const float& value) const
    {
        VectorColor res;
        res.r = r * value;
        res.g = g * value;
        res.b = b * value;
        return res;
    }
    VectorColor& operator*=(const float& value)
    {
        r *= value;
        g *= value;
        b *= value;
        return *this;
    }
    VectorColor operator*(const VectorColor& rhs) const
    {
        VectorColor res;
        res.r = r * rhs.r;
        res.g = g * rhs.g;
        res.b = b * rhs.b;
        return res;
    }
    VectorColor& operator*=(const VectorColor& rhs)
    {
        r *= rhs.r;
        g *= rhs.g;
        b *= rhs.b;
        return *this;
    }
    
    // addition
    VectorColor operator+(const float& value) const
    {
        VectorColor res;
        res.r = r + value;
        res.g = g + value;
        res.b = b + value;
        return res;
    }
    VectorColor& operator+=(const float& value)
    {
        r += value;
        g += value;
        b += value;
        return *this;
    }
    VectorColor operator+(const VectorColor& rhs) const
    {
        VectorColor res;
        res.r = r + rhs.r;
        res.g = g + rhs.g;
        res.b = b + rhs.b;
        return res;
    }
    VectorColor& operator+=(const VectorColor& rhs)
    {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this;
    }

    // subtraction
    VectorColor operator-(const float& value) const
    {
        VectorColor res;
        res.r = r - value;
        res.g = g - value;
        res.b = b - value;
        return res;
    }
    VectorColor& operator-=(const float& value)
    {
        r -= value;
        g -= value;
        b -= value;
        return *this;
    }
    VectorColor operator-(const VectorColor& rhs) const
    {
        VectorColor res;
        res.r = r - rhs.r;
        res.g = g - rhs.g;
        res.b = b - rhs.b;
        return res;
    }
    VectorColor& operator-=(const VectorColor& rhs)
    {
        r -= rhs.r;
        g -= rhs.g;
        b -= rhs.b;
        return *this;
    }

    // division
    VectorColor operator/(const float& value) const
    {
        VectorColor res;
        float invValue = 1.0f / value;
        res.r = r * invValue;
        res.g = g * invValue;
        res.b = b * invValue;
        return res;
    }
    VectorColor& operator/=(const float& value)
    {
        float invValue = 1.0f / value;
        r *= invValue;
        g *= invValue;
        b *= invValue;
        return *this;
    }
    VectorColor operator/(const VectorColor& rhs) const
    {
        VectorColor res;
        res.r = r / rhs.r;
        res.g = g / rhs.g;
        res.b = b / rhs.b;
        return res;
    }
    VectorColor& operator/=(const VectorColor& rhs)
    {
        r /= rhs.r;
        g /= rhs.g;
        b /= rhs.b;
        return *this;
    }

    bool operator<(const float& rhs) const
    {
        return (r < rhs && g < rhs && b < rhs);
    }

    void Limit(float value)
    {
        r = Min(r, value);
        b = Min(b, value);
        g = Min(g, value);
    }

    Color To32Bit() const
    {
        Color res;
        res.SetA(255);
        res.SetR((unsigned char)(r * 255.0f));
        res.SetG((unsigned char)(g * 255.0f));
        res.SetB((unsigned char)(b * 255.0f));
        return res;
    }
};
