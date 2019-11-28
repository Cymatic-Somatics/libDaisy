#pragma once
#include <stdint.h>
#include <math.h>
#include "hid_ctrl.h"
// TODO: Move init and process to .cpp file
// - i was cool with them being in the h file until math.h got involved for the log stuff.
// TODO: add some sort of check to see if class T has a process() function
namespace daisy
{
	//template<class T>
	class parameter
	{
	  public:
		enum
		{
			CURVE_LINEAR,
			CURVE_EXP,
			CURVE_LOG,
			CURVE_CUBE,
			CURVE_LAST,
		};
		parameter() {}
		~parameter() {}

		inline void init(hid_ctrl input, float min, float max, float curve) 
		{ 
			pmin = min;
			pmax = max;
			pcurve = curve;
			in	 = input;
			// prevent NaN/-Infinity
			lmin   = logf(min < 0.0000001f ? 0.0000001f : min);
			lmax   = logf(max);
		}

		inline float process()
		{ 
			switch(pcurve)
			{
				case CURVE_LINEAR: 
					val = (in.process() * (pmax - pmin)) + pmin; 
					break;
				case CURVE_EXP:
					val = in.process();
					val = ((val * val) * (pmax - pmin)) + pmin;
					break;
				case CURVE_LOG: 
					val = expf((in.process() * (lmax - lmin)) + lmin);
					break;
				case CURVE_CUBE:
					val = in.process();
					val = ((val *(val * val)) * (pmax - pmin)) + pmin;
					break;
				default: break;
			}
			return val;
		}

		inline float value() { return val; }

	  private:
		hid_ctrl in;
		float   pmin, pmax;
		float	lmin, lmax; // for log range
		float	val;
		uint8_t pcurve;
	};
} // namespace daisy
