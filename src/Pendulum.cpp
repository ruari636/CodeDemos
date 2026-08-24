#include "Pendulum.h"
#include <cmath>
#include "ofMain.h"
#include "Vec2.h"
#include "CoOrdTransformer.h"

Pendulum::Pendulum(float mass, float theta, float thetaDot, Pendulum * Prev)
	: mass(mass)
	, theta(theta)
	, thetaDot(thetaDot)
	, Prev(Prev) {
	cosTheta = cos(theta);
	sinTheta = sin(theta);
	forces["Peg"] = {};
	forces["Gravity"] = { 0.5f, { 0.0f, 1.0f }, 10.0f * mass };
	//forces["DampTangent"] = {};
	if (Prev != nullptr)
	{
		pegPos = Prev->GetEndPos();
		pos = Prev->GetEndPos();
	}
}

void Pendulum::Draw() const {
	const float xTop = pos.x + DrawOffset.x;
	const float yTop = pos.y + DrawOffset.y;
	const float xBottom = xTop + length * sinTheta;
	const float yBottom = length * cosTheta + yTop;

	CoOrdTransformer::DrawLine(pos + DrawOffset, { xBottom, yBottom }, c);
	// CoOrdTransformer::DrawArrow(pos + (GetTangent() * temp.pos), temp.dir, temp.mag);
}

void Pendulum::Update(float dt) {
	sinTheta = sin(theta);
	cosTheta = cos(theta);
	float moment = 0.0f;
	Vec2 forceSum = { 0.0f, 0.0f };
	Vec2 forceSumPeg = forceSum;
	for (const auto & [key, value] : forces) {
		forceSum += value.dir * value.mag;
		if (key == "Peg") {
			forceSumPeg += value.dir * value.mag;
		}
		moment += (0.5f - value.pos) * value.mag * (value.dir * GetNormal());
	}
	acc = forceSum / mass;
	if (Prev != nullptr && forceSumPeg != Vec2 { 0.0f, 0.0f }) {
		Prev->ApplyForce("NextPendulum", 1.0f, -forceSumPeg.GetNormalized(), forceSumPeg.Len());
	}
	vel += acc * dt;
	pos += vel * dt;
	forces["Peg"].pos = 0.0f;
	forces["Peg"].mag = pegK * ((pos - pegPos) * (pos - pegPos));
	if (pegPos - pos != Vec2 { 0.0f, 0.0f }) {
		forces["Peg"].dir = (pegPos - pos).GetNormalized();
	}
	//pegPos += pegVel * dt;
	if (Prev != nullptr) {
		pegPos = Prev->GetEndPos();
	}
	float thetaDoubleDot = moment / Ig;
	thetaDot += thetaDoubleDot * dt;
	theta += thetaDot * dt;

	// add drag
	forces["Drag"] = { 0.001f * mass * thetaDot * thetaDot * thetaDot, GetNormal(), mass };
}

void Pendulum::SetPegVelX(float vIn) {
	pegVel.x = vIn;
}

Vec2 Pendulum::GetNormal() const {
	return -Vec2(cosTheta, -sinTheta);
}

Vec2 Pendulum::GetTangent() const {
	return Vec2(sinTheta, cosTheta);
}

void Pendulum::ApplyForce(std::string name, float posAlongRod, Vec2 dir, float mag) {
	forces[name] = { posAlongRod, dir, mag };
}

void Pendulum::ApplyImpulse(float distFromEnd, Vec2 dir, float mag) {
	float deltaW = mag * (dir * GetNormal()) * (0.5f - distFromEnd) / Ig;
	Vec2 deltaV = dir * mag / mass;
	thetaDot += deltaW;
	vel += deltaV;
}

void Pendulum::MovePeg(float ax, float dt) {
	pegVel.x += ax * dt;
}

Vec2 Pendulum::GetEndPos() const {
	return pegPos + GetTangent() * length;
}

Vec2 Pendulum::GetVelOfPoint(float distFromEnd) {
	return vel - GetNormal() * thetaDot * distFromEnd;
}
