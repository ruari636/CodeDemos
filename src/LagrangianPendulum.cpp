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

void LagrangianPendulum::UpdateBob(float dt) {
	isBob = true;
	float theta1ddot = (-g * (2 * m1 + m2) * sin(theta1) - m2 * g * sin(theta1 - 2 * theta2) - m2 * 2 * sin(theta1 - theta2) * (theta1dot * theta1dot * l1 * cos(theta1) + theta2dot * theta2dot * l2)) / (l1 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));
	float theta2ddot = (2 * sin(theta1 - theta2) * (theta1dot * theta1dot * l1 * (m1 + m2) + g * (m1 + m2) * cos(theta1) + theta2dot * theta2dot * l2 * m2 * cos(theta1 - theta2))) / (l2 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));

	theta1dot += theta1ddot * dt;
	theta2dot += theta2ddot * dt;
	theta1 += theta1dot * dt;
	theta2 += theta2dot * dt;
}

void LagrangianPendulum::Draw() {
	Vec2 p1 = drawOffset;
	Vec2 p2;
	Vec2 p3;
	if (isBob) {
		p2 = p1 + Vec2 { l1 * sin(theta1), l1 * cos(theta1) };
		p3 = p2 + Vec2 { l2 * sin(theta2), l2 * cos(theta2) };
		CoOrdTransformer::DrawCircle(p2, 0.05f, c);
		CoOrdTransformer::DrawCircle(p3, 0.05f, c);
	} else {
		p2 = p1 + Vec2 { l * sin(theta1), l * cos(theta1) };
		p3 = p2 + Vec2 { l * sin(theta2), l * cos(theta2) };
	}
	CoOrdTransformer::DrawLine(p1, p2, c);
	CoOrdTransformer::DrawLine(p2, p3, c);
}
