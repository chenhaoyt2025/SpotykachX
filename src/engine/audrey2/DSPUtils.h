#pragma once
#ifndef INFS_DSPUTILS_H
#define INFS_DSPUTILS_H

#include <cmath>
#include <Utility/dsp.h>
#ifdef __arm__
#include <arm_math.h>
#endif

namespace infrasonic {

inline float audrey_dbfs2lin(float dbfs) {
    return daisysp::pow10f(dbfs * 0.05f);
}

inline float audrey_onepole_coef(float time_s, float sample_rate) {
    if (time_s <= 0.0f || sample_rate <= 0.0f) { return 1.0f; }
    return daisysp::fmin(1.0f / (time_s * sample_rate), 1.0f);
}

inline float onepole_coef_t60(float time_s, float sample_rate)
{
	return audrey_onepole_coef(time_s * 0.1447597f, sample_rate);
}

inline float ftension(const float in, const float factor)
{
    if (factor == 0.0f) return in;
    const float denom = expm1f(factor);
    return expm1f(in * factor) / denom;
}

inline float tanf(const float x)
{
#ifdef __arm__
    return std::tan(x);
    // float s, c;
    // arm_sin_cos_f32(x, &s, &c);
    // return s / c;
#else
    return std::tan(x);
#endif
}

}

#endif
