#ifndef SAMPLER_COMMON
#define SAMPLER_COMMON

sampler LinearSampler	: register(s0);
sampler PointSampler	: register(s1);
SamplerComparisonState ShadowSampler	: register(s2);

#endif