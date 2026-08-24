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
	forces["DampTangent"] = {};
	if (Prev != nullptr)
	{
		pegPos = Prev->GetEndPos();
		pos = Prev->GetEndPos();
	}
}

void Pendulum::Draw() const {
	const float xTop = pos.x;
	const float yTop = pos.y;
	const float xBottom = xTop + length * sinTheta;
	const float yBottom = length * cosTheta + yTop;

	CoOrdTransformer::DrawLine(pos, { xBottom, yBottom });
	CoOrdTransformer::DrawArrow(pos + (GetTangent() * temp.pos), temp.dir, temp.mag);
}

void Pendulum::Update(float dt) {
	temp = forces["DampTangent"];
	sinTheta = sin(theta);
	cosTheta = cos(theta);
	moment = 0.0f;
	Vec2 forceSum = { 0.0f, 0.0f };
	posMinusPegPosPrev = forces["Peg"].dir * forces["Peg"].mag / pegK;
	Vec2 posMinusPegPos = pos - pegPos;
	float velRelPeg = ((posMinusPegPos - posMinusPegPosPrev) * GetTangent()) / dt;
	forces["Peg"].pos = 0.0f;
	forces["Peg"].mag = pegK * (pos - pegPos).Len(); //std::max(pegK * (pos - pegPos).Len() - pegDamping * velRelPeg, 0.0f);

	if (pegPos - pos != Vec2 { 0.0f, 0.0f }) {
		forces["Peg"].dir = (pegPos - pos).GetNormalized();
	}
	for (const auto & [key, value] : forces)
	{
		forceSum += value.dir * value.mag;
		moment += (0.5f - value.pos) * value.mag * (value.dir * GetNormal());
	}
	acc = forceSum / mass;
	if (Prev != nullptr) {
		Prev->ApplyForce("NextPendulum", 1.0f, -forces["Peg"].dir, forces["Peg"].mag);
	}
	vel += acc * dt;
	pos += vel * dt;
	if (Prev != nullptr)
	{
		pegPos = Prev->GetEndPos();
	}
	float thetaDoubleDot = moment / Ig;
	thetaDot += thetaDoubleDot * dt;
	theta += thetaDot * dt;
	ApplyForce("DampTangent", 0.0f, GetTangent(), std::max(std::min(GetVelOfPoint(0.0f) * GetTangent() * mass * pegDamping, 5.0f), -5.0f));
	std::cout << std::max(std::min(GetVelOfPoint(0.0f) * GetTangent() * mass * pegDamping, 5.0f), -5.0f) << '\n';
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

void Pendulum::ApplyForceCOM(Vec2 dir, float mag) {
}

void Pendulum::ApplyForce(std::string name, float posAlongRod, Vec2 dir, float mag) {
	forces[name] = { posAlongRod, dir, mag };
}

void Pendulum::ApplyMoment(float M) {
	moment += M;
}

void Pendulum::SetMoment(float M) {
	moment = M;
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
