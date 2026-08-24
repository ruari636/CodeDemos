#include "LagrangianPendulum.h"
#include <ofMath.h>
#include "Vec2.h"
#include "CoOrdTransformer.h"

LagrangianPendulum::LagrangianPendulum(float l, float theta1, float theta1dot, float theta2, float theta2dot)
	:
	l(l),
	theta1(theta1),
	theta1dot(theta1dot),
	theta2(theta2), theta2dot(theta2dot) {
}

void LagrangianPendulum::Update(float dt) {

	float theta1ddot = 4.5 * g * sin(theta2) * cos(theta1 - theta2) - 9 * g * sin(theta1) -
		4.5 * l * theta1dot * theta1dot * sin(theta1 - theta2) * cos(theta1 - theta2) - 3 * l * theta2dot * theta2dot * sin(theta1 - theta2);
	theta1ddot /= l * (8 - 4.5 * cos(theta1 - theta2) * cos(theta1 - theta2));
	
	float theta2ddot = 3 * l * theta1dot * theta1dot * sin(theta1 - theta2) - 3 * g * sin(theta2) - 3 * l * theta1ddot * cos(theta1 - theta2);
	theta2ddot /= 2 * l;

	theta1dot += theta1ddot * dt;
	theta2dot += theta2ddot * dt;
	theta1 += theta1dot * dt;
	theta2 += theta2dot * dt;
}

void LagrangianPendulum::Draw() {
	Vec2 p1 = { 0.0f, 0.0f };
	Vec2 p2 = p1 + Vec2{ l * sin(theta1), l * cos(theta1) };
	Vec2 p3 = p2 + Vec2 { l * sin(theta2), l * cos(theta2) };
	ofSetColor((255, 0, 0));
	CoOrdTransformer::DrawLine(p1, p2);
	CoOrdTransformer::DrawLine(p2, p3);
	ofSetColor((255, 255, 255));
}
