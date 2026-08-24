#include "LagrangianPendulum.h"
#include <ofMath.h>
#include "Vec2.h"
#include "CoOrdTransformer.h"

LagrangianPendulum::LagrangianPendulum(float m1, float m2, float l1, float l2, float theta1, float theta1dot, float theta2, float theta2dot)
	:
	m1(m1),
	m2(m2), l1(l1)
	, l2(l2)
	, theta1(theta1),
	theta1dot(theta1dot),
	theta2(theta2), theta2dot(theta2dot) {
}

void LagrangianPendulum::Update(float dt) {
	float sinTheta1mTheta2 = sin(theta1 - theta2);
	float cosTheta1mTheta2 = cos(theta1 - theta2);

	float theta1ddot = (-g * (2 * m1 + m2) * sin(theta1) - m2 * g * sin(theta1 - 2 * theta2) -
		m2 * 2 * sin(theta1 - theta2) * (theta1dot * theta1dot * l1 * cos(theta1) + theta2dot * theta2dot * l2)) /
		(l1 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));
	float theta2ddot = (2 * sin(theta1 - theta2) * (theta1dot * theta1dot * l1 * (m1 + m2) + g * (m1 + m2) * cos(theta1) +
		theta2dot * theta2dot * l2 * m2 * cos(theta1 - theta2))) /
		(l2 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));

	theta1dot += theta1ddot * dt;
	theta2dot += theta2ddot * dt;
	theta1 += theta1dot * dt;
	theta2 += theta2dot * dt;
}

void LagrangianPendulum::Draw() {
	Vec2 p1 = { 0.0f, 0.0f };
	Vec2 p2 = p1 + Vec2{ l1 * sin(theta1), l1 * cos(theta1) };
	Vec2 p3 = p2 + Vec2 { l2 * sin(theta2), l2 * cos(theta2) };
	CoOrdTransformer::DrawLine(p1, p2);
	CoOrdTransformer::DrawLine(p2, p3);
}
