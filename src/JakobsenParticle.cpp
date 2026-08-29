#include "JakobsenParticle.h"

JakobsenParticle::JakobsenParticle(Vec2 pos, float mass)
	: pos(pos), mass(mass), prevPos(pos) {
}

Vec2 JakobsenParticle::GetPos() const {
	return pos;
}

void JakobsenParticle::MoveBy(Vec2 delta) {
	pos += delta;
}

void JakobsenParticle::SetForce(std::string name, Vec2 forceDir, float magnitude) {
	forces[name] = Force(magnitude, forceDir);
}

void JakobsenParticle::Update(float dt) {
	Vec2 forceSum = { 0.0f, 0.0f };
	for (const auto & [key, value] : forces) {
		forceSum += value.dir * value.mag;
	}
	Vec2 acc = forceSum / mass;
	vel += acc * dt;
	
	// Verlet integration
	auto ptemp = pos;
	pos = pos + (pos - prevPos) * 0.9 + acc * dt * dt;
	prevPos = ptemp;
}

Force JakobsenParticle::GetForce(std::string name) {
	if (forces.contains(name)) {
		return forces[name];
	}
	return {
		0.0f, { 0.0f, 0.0f }
	};
}
