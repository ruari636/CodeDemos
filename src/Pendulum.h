#pragma once
#include "Vec2.h"
#include <map>
#include <string>
#include <vector>

class Pendulum {
	struct Force {
		Vec2 dir;
		float pos;
		float mag;
		Force(float pos = 0.0f, Vec2 dir = { 0.0f, 1.0f }, float mag = 0.0f)
			: pos(pos)
			, dir(dir)
			, mag(mag) { };
	};
	Vec2 pegPos = { 0.0f, 0.0f };
	Vec2 pegVel = { 0.0f, 0.0f };
	float theta = 0.0f;
	float pegK = 1.0e3f;
	const float length = 1.0f;
	const float mass = 1.0f;
	const float Ig = mass * length * length / 12.0f;
	const float Ic = mass * length * length / 3.0f;
	float sinTheta = 0.0f;
	float cosTheta = 1.0f;
	float thetaDot = 0.0f;
	float pegVelX = 0.0f;
	float moment = 0.0f;
	Vec2 pos = { 0.0f, 0.0f };
	Vec2 vel = { 0.0f, 0.0f };
	Vec2 acc = { 0.0f, 0.0f };
	std::map<std::string, Force> forces = {};
	Pendulum * Prev = nullptr;

public:
	Pendulum(float mass = 1.0f, float theta = 0.0f, float thetaDot = 0.0f, Pendulum* Prev = nullptr);
	void Draw() const;
	void Update(float dt);
	void SetPegVelX(float vIn);
	Vec2 GetNormal() const;
	Vec2 GetTangent() const;
	void ApplyForceCOM(Vec2 dir, float mag);
	void ApplyForce(std::string name, float posAlongRod, Vec2 dir, float mag);
	void ApplyMoment(float M);
	void SetMoment(float M);
	void MovePeg(float ax, float dt);
	Vec2 GetEndPos() const;
	float GetPosX() const{
		return pegPos.x;
	}
	float GetVelX() const {
		return pegVel.x;
	}
	float getTheta() const {
		return theta;
	}
	float getThetaDot() const {
		return thetaDot;
	}
};
