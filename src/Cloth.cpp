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
	Force fPrev = p1.GetForce("PrevParticle");
	Vec2 newForce = fPrev.Vector() + p1.GetForce("Gravity").Vector();
	Force Next = { newForce.Len(), newForce.GetNormalized() };
	switch (r) {
	case Relation::Left:
		p2.SetForce("RightParticle", normalised, springK * (delta.Len() - 0.1f));
		p1.SetForce("LeftParticle", -normalised, springK * (delta.Len() - 0.1f));
		break;
	case Relation::Right:
		p2.SetForce("LeftParticle", normalised, springK * (delta.Len() - 0.1f));
		p1.SetForce("RightParticle", -normalised, springK * (delta.Len() - 0.1f));
		break;
	case Relation::Above:
		p2.SetForce("BelowParticle", normalised, springK * (delta.Len() - 0.1f));
		p1.SetForce("AboveParticle", -normalised, springK * (delta.Len() - 0.1f));
		break;
	case Relation::Below:
		p2.SetForce("AboveParticle", normalised, springK * (delta.Len() - 0.1f));
		p1.SetForce("BelowParticle", -normalised, springK * (delta.Len() - 0.1f));
		break;
	}
}

void Cloth::Update(float dt, int nPasses, bool gravity) {
	for (int y = nRows - 1; y > -1; y--) {
		for (int x = 0; x < nCols; x++) {
			bool isOnLHS = x == 0;
			bool isOnRHS = x == (nCols - 1);
			bool isOnTop = y == 0;
			bool isOnBottom = y == (nRows - 1);
			int curPos = y * nCols + x;
			Vec2 forceSum = { 0.0f, 0.0f };
			auto & particle = *cloth[curPos];
			float verticalComponentSum = 0.0f;
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
			if (isOnTop && isOnLHS)
			{
				Vec2 delta = topLeftPosition - particle.GetPos();
				if (delta.LenSq() != 0.0f) {
					particle.SetForce("ElasticClamp", delta.GetNormalized(), springK * delta.Len());
				}
			}
			if (isOnTop && isOnRHS) {
				Vec2 delta = topRightPosition - particle.GetPos();
				if (delta.LenSq() != 0.0f) {
					particle.SetForce("ElasticClamp", delta.GetNormalized(), springK * delta.Len());
				}
			}
		}
	}
	for (int y = 0; y < nRows; y++) {
		for (int x = 0; x < nCols; x++) {
			int curPos = y * nCols + x;
			auto & particle = *cloth[curPos];
			particle.Update(dt);
		}
	}
	for (int i = 0; i < nPasses; i++) {
		for (auto & c : constraintsCloth) {
			Constraint::RelaxConstraint(c);
		}
	}
}

Cloth::Cloth(int nCols, int nRows, Vec2 startPos)
	: nCols(nCols)
	, nRows(nRows)
	, topLeftPosition(startPos)
	, topRightPosition(startPos + Vec2{0.1f * nCols, 0.0f}) {
	float tempX = startPos.x;
	for (int y = 0; y < nRows; y++) {
		for (int x = 0; x < nCols; x++) {
			cloth.emplace_back(std::make_unique<JakobsenParticle>(startPos, 1.0f));
			startPos.x += 0.1f;
		}
		startPos.x = tempX;
		startPos.y += 0.1f;
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
				constraintsCloth.emplace_back(*cloth[curPos], *cloth[curPos + nCols], 0.1f);
			}
		}
	}
}

void Cloth::Draw(ofColor c) {
	for (auto & constraint : constraintsCloth) {
		CoOrdTransformer::DrawLine(constraint.p1.GetPos(), constraint.p2.GetPos(), (255, 255, 255, 255));
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
