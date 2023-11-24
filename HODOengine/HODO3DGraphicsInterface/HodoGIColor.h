#pragma once

namespace hodoGI
{
	union Color
	{
		float rgba[4] = { 0, };
		struct  
		{
			float r;
			float g;
			float b;
			float a;
		};
	};
}