#pragma once
#include "JakobsenParticle.h"

struct Constraint {
	JakobsenParticle& p1;
	JakobsenParticle& p2;
	float spacingDist = 0.1f;
	Constraint(JakobsenParticle & p1, JakobsenParticle & p2, float spacing)
		: p1(p1)
		, p2(p2)
		, spacingDist(spacing) { }
	static void RelaxConstraint(Constraint& c, bool oneSided = false)
	{
		auto& p1 = c.p1;
		auto& p2 = c.p2;
		Vec2 diff = (p1.GetPos() - p2.GetPos());
		float realDist = diff.Len();
		Vec2 dir = diff.GetNormalized();
		Vec2 thisMovement = dir * (realDist - c.spacingDist) / 2;
		if (oneSided)
		{
			p2.MoveBy(thisMovement * 2);
		} else {
			p1.MoveBy(-thisMovement);
			p2.MoveBy(thisMovement);
		}
	}
};
