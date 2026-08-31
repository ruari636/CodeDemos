#pragma once
#include "Vec2.h"
#include <vector>
#include <memory>
#include "Constraint.h"
#include "ofMain.h"

class Cloth
{
private:
	std::vector<std::unique_ptr<JakobsenParticle>> cloth;
	std::vector<Constraint> constraintsCloth;
	const int nCols;
	const int nRows;
	static const int springK = 1000;
	Vec2 topLeftPosition;
	Vec2 topRightPosition;
	Vec2 GetTopClampPos(int x) {
		return topLeftPosition + Vec2 { 0.1f, 0.0f } * (float)x;
	};

public:
	Cloth(int nCols, int nRows, Vec2 startPos);
	void Update(float dt, int nPasses, bool gravity = true, bool strictConstraints = true);
	void Draw(ofColor c);
	Vec2 GetTopLeftPos();
	void SetTopLeftPos(Vec2 posIn);
	void SetTopRightPos(Vec2 posIn);
	Vec2 GetPosBetweenTLandTR(int colN);
	~Cloth() = default;
};
