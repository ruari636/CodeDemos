#include "PendulumBob.h"
#include "CoOrdTransformer.h"

PendulumBob::PendulumBob(float thetaIn, float mIn, float lIn, PendulumBob * next)
	: m(mIn), l(lIn), next(next), theta(thetaIn) {
	forces["Gravity"] = { 10.0f * m, { 0.0f, 1.0f } };
	cosTheta = cos(theta);
	sinTheta = sin(theta);
	if (next != nullptr) {
		pegPos = next->pegPos + next->GetTangent() * next->l;
	}
	massPos = pegPos + Vec2{ sinTheta, cosTheta } * l;
}

void PendulumBob::ApplyForce(std::string name, Force f) {
	forces[name] = f;
}

void PendulumBob::Update(float dt) {
	if (massPos.y != pegPos.y) {
		theta = atan((massPos.x - pegPos.x) / (massPos.y - pegPos.y));
	} else {
		theta = massPos.x > pegPos.x ? PI / 2.0f : -PI / 2.0f;
	}
	cosTheta = cos(theta);
	sinTheta = sin(theta);
	if (next != nullptr) {
		pegPos = next->massPos;
		next->ApplyForce("NextString", forces["StringExtension"] * -1.0f);
	}
	Vec2 forceSum = {0.0f, 0.0f};
	forces["StringExtension"] = (massPos - pegPos).Len() > l ?
		Force { ((massPos - pegPos).Len() - l) * kString, -GetTangent() } : Force { 0, GetTangent() };
	for (const auto & [key, value] : forces) {
		forceSum += value.dir * value.mag;
	}
	Vec2 acc = forceSum / m;
	massVel += acc * dt;
	massPos += massVel * dt;
}

void PendulumBob::Draw() {
	const float xTop = pegPos.x + drawOffset.x;
	const float yTop = pegPos.y + drawOffset.y;
	const float xBottom = xTop + l * sinTheta;
	const float yBottom = l * cosTheta + yTop;

	CoOrdTransformer::DrawLine({xTop, yTop}, { xBottom, yBottom }, c);
	CoOrdTransformer::DrawCircle({ xBottom, yBottom }, 0.05f, c);
}

Vec2 PendulumBob::GetNormal() const {
	return -Vec2(cosTheta, -sinTheta);
}

Vec2 PendulumBob::GetTangent() const {
	return Vec2(sinTheta, cosTheta);
}
