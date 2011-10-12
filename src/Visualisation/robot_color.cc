/*
 * robot_color.cc
 *
 *  Created on: 12.10.2011
 *      Author: MeisterYeti
 */

#include <cmath>
#include <cfloat>

#include "robot_color.h"

#define INV_GOLDEN_RATIO 0.61803398874989484820458683436564
#define ONE_OVER_THREE   0.3333333333333333333333333333333
#define TWO_OVER_THREE   0.6666666666666666666666666666667
#define ONE_OVER_SIX     0.1666666666666666666666666666667

void hsl2rgb_sub(const float& a, const float& b, float& rgb) {
	if(rgb < 0) {
		++rgb;
	} else if(rgb > 1) {
		--rgb;
	}

	if(rgb < ONE_OVER_SIX) {
		rgb = a+(b-a)*6.0*rgb;
	} else if(rgb < 0.5) {
		rgb = b;
	} else if(rgb < TWO_OVER_THREE) {
		rgb = a+(b-a)*(TWO_OVER_THREE-rgb)*6.0;
	} else {
		rgb = a;
	}
}

void hsl2rgb(const float& h, const float& s, const float& l, float* rgb) {
	if(std::abs(s)<FLT_EPSILON) {
		rgb[0] = l;
		rgb[1] = l;
		rgb[2] = l;
	} else {
		float tmp2 = l<0.5 ? l*(1.0+s) : l+s-l*s;
		float tmp1 = 2.0*l-tmp2;
		// R
		rgb[0] = h+ONE_OVER_THREE;
		hsl2rgb_sub(tmp1, tmp2, rgb[0]);
		// G
		rgb[1] = h;
		hsl2rgb_sub(tmp1, tmp2, rgb[1]);
		// B
		rgb[2] = h-ONE_OVER_THREE;
		hsl2rgb_sub(tmp1, tmp2, rgb[2]);
	}
}

void generateColor(const unsigned int& color_number, float* color) {
	float hue = (float)color_number*INV_GOLDEN_RATIO+ONE_OVER_THREE;;
	hue = std::modf(hue, color);
	hsl2rgb(hue, 1.0f, 0.5f, color);
}
