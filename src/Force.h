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
	Vec2 Vector() const {
		return dir * mag;
	}
	Force operator+(const Force& rhs)
	{
		Vec2 total = this->Vector() + rhs.Vector();
		return Force(total.Len(), total.GetNormalized());
	}
};
