#pragma once
#include "JakobsenParticle.h"

struct Constraint {
	JakobsenParticle * p1;
	JakobsenParticle * p2;
	float spacingDist = 0.1f;
	static void RelaxConstraint(Constraint& c)
	{
		auto p1 = c.p1;
		auto p2 = c.p2;
		Vec2 diff = (p1->GetPos() - p2->GetPos());
		float realDist = diff.Len();
		Vec2 dir = diff.GetNormalized();
		Vec2 thisMovement = dir * (realDist - c.spacingDist) / 2;
		p1->MoveBy(-thisMovement);
		p2->MoveBy(thisMovement);
	}
};
