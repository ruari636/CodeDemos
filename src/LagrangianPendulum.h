#pragma once
#include "Vec2.h"
#include "ofColor.h"

class LagrangianPendulum {
	const float l = 1.0f;
	float theta1 = 0.0f;
	float theta1dot = 0.0f;
	float theta2 = 0.0f;
	float theta2dot = 0.0f;
	float g = 10.0f;
	float m1 = 1.0f;
	float m2 = 1.0f;
	const float l1 = 1.0f;
	const float l2 = 1.0f;
	bool isBob = false;
	ofColor c = (0, 255, 0, 1.0f);

public:
	Vec2 drawOffset = { 0.0f, 0.0f };
	LagrangianPendulum(float l, float theta1, float theta1dot, float theta2, float theta2dot);
	void Update(float dt);
	void UpdateBob(float dt);
	void Draw();
};
