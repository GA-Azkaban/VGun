#pragma once
#include "Singleton.h"
#include <random>

namespace HDEngine
{
	class RandomGenerator : public Singleton<RandomGenerator>
	{
	public:
		friend Singleton;

		RandomGenerator()
			: gen(std::random_device()())
		{

		}

		std::mt19937& MersenneTwiste()
		{
			return gen;
		}

	private:
		std::mt19937 gen;
	};
}
