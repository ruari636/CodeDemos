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
	static const int springK = 10000;
	Vec2 topLeftPosition;
	Vec2 topRightPosition;

public:
	Cloth(int nCols, int nRows, Vec2 startPos);
	void Update(float dt, int nPasses, bool gravity = true);
	void Draw(ofColor c);
	Vec2 GetTopLeftPos();
	void SetTopLeftPos(Vec2 posIn);
	void SetTopRightPos(Vec2 posIn);
};
