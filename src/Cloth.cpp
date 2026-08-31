#include "Cloth.h"
#include "CoOrdTransformer.h"

enum class Relation {
	Left,
	Right,
	Above,
	Below
};

static void SetSpringForce(JakobsenParticle& p1, JakobsenParticle& p2, float springK, Relation r) {
	Vec2 delta = p1.GetPos() - p2.GetPos();
	// delta pointing down initially
	Vec2 normalised = delta.GetNormalized();
	switch (r) {
	case Relation::Left:
		p2.SetForce("RightParticle", normalised, springK * (delta.Len() - 0.1f));
		p1.SetForce("LeftParticle", -normalised, springK * (delta.Len() - 0.1f));
		break;
	case Relation::Above:
		p2.SetForce("BelowParticle", normalised, springK * (delta.Len() - 0.1f));
		p1.SetForce("AboveParticle", -normalised, springK * (delta.Len() - 0.1f));
		break;
	}
}

static void ApplyForceTwoParticles(std::string name, JakobsenParticle& p1, JakobsenParticle& p2, Force f)
{
	p1.SetForce(name, f.dir, f.mag);
	name += "FromOther";
	p2.SetForce(name, -f.dir, f.mag);
}

Vec2 GetDelta(const JakobsenParticle& p1, const JakobsenParticle& p2)
{
	return p1.GetPos() - p2.GetPos();
} 

void Cloth::Update(float dt, int nPasses, bool gravity, bool strictConstraints) {
	for (int y = nRows - 1; y > -1; y--) {
		for (int x = 0; x < nCols; x++) {
			bool isOnLHS = x == 0;
			bool isOnRHS = x == (nCols - 1);
			bool isOnTop = y == 0;
			bool isOnBottom = y == (nRows - 1);
			int curPos = y * nCols + x;
			Vec2 forceSum = { 0.0f, 0.0f };
			auto & particle = *cloth[curPos];
			if (!gravity) {
				particle.SetForce("Gravity", { 0.0f, 1.0f }, 0.0f);
			} else {
				particle.SetForce("Gravity", { 0.0f, 1.0f }, 10.0f);
			}
			if (!isOnLHS) {
				auto & leftP = *cloth[curPos - 1];
				SetSpringForce(particle, leftP, springK, Relation::Left);
			}
			if (!isOnTop) {
				auto & topP = *cloth[curPos - nCols];
				SetSpringForce(particle, topP, springK, Relation::Above);
			}
			if ((isOnTop && isOnLHS) || (isOnTop && isOnRHS)) {
				Vec2 delta = GetPosBetweenTLandTR(x) - particle.GetPos();
				if (delta.LenSq() != 0.0f) {
					particle.SetForce("ElasticClamp", delta.GetNormalized(), springK * delta.Len());
				}
			}
		}
	}
	for (int y = nRows - 1; y > -1; y--) {
		for (int x = 0; x < nCols; x++) {
			int curPos = y * nCols + x;
			auto & particle = *cloth[curPos];
			particle.Update(dt);
			//auto fV = particle.GetForce("Gravity").Vector();
			//fV += particle.GetForce("BelowParticle").Vector();
			//fV += particle.GetForce("AboveParticle").Vector();
			//fV += particle.GetForce("LeftParticle").Vector() + particle.GetForce("RightParticle").Vector();
			//if (y == 0 && x == 0)
			//{
			//	std::cout << "Vertical force on particle: " << fV.Len() * (fV.GetNormalized() * Vec2 { 0.0f, 1.0f }) << '\n';
			//}	
		}
	}
	if (strictConstraints) {
		for (int i = 0; i < nPasses; i++) {
			for (auto & c : constraintsCloth) {
				Constraint::RelaxConstraint(c);
			}
		}
	}
}

Cloth::Cloth(int nCols, int nRows, Vec2 TL, Vec2 TR)
	:
	nCols(nCols),
	nRows(nRows), topLeftPosition(TL)
	, topRightPosition(TR)
{
	float tempX = TL.x;
	for (int y = 0; y < nRows; y++) {
		for (int x = 0; x < nCols; x++) {
			cloth.emplace_back(std::make_unique<JakobsenParticle>(TL, 1.0f));
			TL.x += 0.1f;
		}
		TL.x = tempX;
		TL.y += 0.1f;
	}
	for (int y = 0; y < nRows; y++) {
		for (int x = 0; x < nCols; x++) {
			bool isOnLHS = x == 0;
			bool isOnRHS = x == (nCols - 1);
			bool isOnTop = y == 0;
			bool isOnBottom = y == (nRows - 1);
			int curPos = y * nCols + x;
			cloth[curPos]->SetForce("Gravity", { 0.0f, 1.0f }, 10.0f);
			if (!isOnRHS) {
				constraintsCloth.emplace_back(*cloth[curPos], *cloth[curPos + 1], 0.1f);
			}
			if (!isOnBottom) {
				if (isOnTop)
				{
				constraintsCloth.emplace_back(*cloth[curPos], *cloth[curPos + nCols], 0.1f, true);
				} else {
					constraintsCloth.emplace_back(*cloth[curPos], *cloth[curPos + nCols], 0.1f);
				}
			}
		}
	}
	for (int i = 0; i < 32; i++) {
		for (auto & c : constraintsCloth) {
			Constraint::RelaxConstraint(c);
		}
	}
}

void Cloth::Draw(ofColor c) {
	for (auto & constraint : constraintsCloth) {
		CoOrdTransformer::DrawLine(constraint.p1.GetPos(), constraint.p2.GetPos(), c);
	}
}

Vec2 Cloth::GetTopLeftPos() {
	return topLeftPosition;
}

void Cloth::SetTopLeftPos(Vec2 posIn) {
	topLeftPosition = posIn;
}

void Cloth::SetTopRightPos(Vec2 posIn) {
	topRightPosition = posIn;
}

Vec2 Cloth::GetPosBetweenTLandTR(int colN) {
	return topLeftPosition + (topRightPosition - topLeftPosition) * (float(colN) / float(nCols));
}
