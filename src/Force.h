#pragma once
#include "Vec2.h"

struct Force {
	Force() = default;
	Force(float mag, Vec2 dir)
		: mag(mag)
		, dir(dir)
		{}
	float mag;
	Vec2 dir;
	Force operator*(const float & rhs) {
		return { mag * rhs, dir };
	}
};
