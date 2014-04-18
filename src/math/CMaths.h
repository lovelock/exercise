#ifndef CMathsH
#define CMathsH

#include "math/CTransform.h"
#include <math.h>

inline bool cZero(const double& a_value)
{
    return ((a_value < C_TINY) && (a_value > -C_TINY));
}

inline bool cPositiveBound(const double& a_value,
                           const double& a_boundMax)
{
    return ((a_value > C_TINY) && ((a_boundMax < 0) || (a_value < a_boundMax)));
}

template<class T> inline T cAbs(const T& a_value)
{
    return (a_value >= 0 ? a_value : -a_value);
}

template<class T> inline double cSign(const T& a_value)
{
    if (a_value < 0)
    {
        return (-1);
    }
    else
    {
        return (1);
    }
}

template<class T> inline T cMax(const T& a_value1,
                                const T& a_value2)
{
    return (a_value1 >= a_value2 ? a_value1 : a_value2);
}


template<class T> inline T cMin(const T& a_value1,
                                const T& a_value2)
{
    return (a_value1 <= a_value2 ? a_value1 : a_value2);
}


template<class T> inline T cMax3(const T& a_value1,
                                 const T& a_value2,
                                 const T& a_value3)
{
    return (cMax(a_value1, cMax(a_value2, a_value3)));
}


template<class T> inline T cMin3(const T& a_value1,
                                 const T& a_value2,
                                 const T& a_value3)
{
    return (cMin(a_value1, cMin(a_value2, a_value3)));
}


template<class T> inline T cMaxAbs(const T& a_value1,
                                   const T& a_value2)
{
    return (cAbs(a_value1) >= cAbs(a_value2) ? a_value1 : a_valu2);
}


template<class T> inline T cMinAbs(const T& a_value1,
                                   const T& a_value2)
{
    return (cAbs(a_value1) <= cAbs(a_value2) ? a_value1 : a_valu2);
}

template<class T> inline T cMax3Abs(const T& a_value1,
                                    const T& a_value2,
                                    const T& a_value3)
{
    return (cMaxAbs(a_valu1, cMaxAbs(a_value2, a_value3)));
}


template<class T> inline T cMin3Abs(const T& a_value1,
                                    const T& a_value2,
                                    const T& a_value3)
{
    return (cMinAbs(a_valu1, cMinAbs(a_value2, a_value3)));
}


template<class T> inline void cSwap(T& a_value1,
                                    T& a_value2)
{
    T value = a_value1;
    a_value1 = a_value2;
    a_value2 = value;
}

template<class T> inline T cLerp(const double& a_leve1,
                                 const T& a_value1,
                                 const T& a_value2)
{
    return (a_value2 * a_leve1 + a_value1 * (1 - a_leve1));
}


template<class T> inline T cClamp(const T& a_value,
                                  const T& a_low,
                                  const T& a_high)
{
    return (a_value < a_low ? a_low : a_value > a_high ? a_high : a_value);
}


template<class T> inline T cClamp0(const T& a_value)
{
    return cMax<T>(0, a_value);
}


inline double cClamp01(const double& a_value)
{
    return (cClamp(a_value, 0.0, 1.0));
}

