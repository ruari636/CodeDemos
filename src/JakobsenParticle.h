#pragma once
#include "Vec2.h"
#include "Force.h"
#include <map>
#include <string>

class JakobsenParticle {
	
	Vec2 vel = { 0.0f, 0.0f };
	Vec2 pos;
	Vec2 prevPos;
	std::map<std::string, Force> forces;
	float mass = 1.0f;
public:
	JakobsenParticle(Vec2 pos, float mass);
	Vec2 GetPos() const;
	void MoveBy(Vec2 delta);
	void SetForce(std::string name, Vec2 forceDir, float magnitude);
	void SetForce(std::string name, Force f);
	void Update(float dt);
	Force GetForce(std::string name) const;
};
