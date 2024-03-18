#ifndef SAMPLER_COMMON
#define SAMPLER_COMMON

sampler LinearWrapSampler	: register(s0);
sampler LinearBorderSampler	: register(s1);
sampler PointClampSampler		: register(s2);
SamplerComparisonState ShadowSampler	: register(s3);

#endif