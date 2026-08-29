#pragma once
#include "Vec2.h"
#include <string>
#include <map>
#include "ofColor.h"
#include "Force.h"

class PendulumBob {
	float m = 1.0f;
	float l = 1.0f;
	Vec2 pegPos = { 0.0f, 0.0f };
	Vec2 massPos;
	std::map<std::string, Force> forces;
	PendulumBob * next = nullptr;
	PendulumBob * prev = nullptr;
	Vec2 massVel = { 0.0f, 0.0f };
	float cosTheta;
	float sinTheta;
	float theta;
	float thetaDot = 0.0f;
	const float kString = 600.0f;
	ofColor c = (255, 255, 255);

public:
	PendulumBob(float thetaIn, float mIn, float lIn, PendulumBob * next);
	Vec2 drawOffset = { 0.0f, 0.0f };
	void ApplyForce(std ::string name, Force f);
	void Update(float dt);
	void Draw();
	Vec2 GetNormal() const;
	Vec2 GetTangent() const;
};
