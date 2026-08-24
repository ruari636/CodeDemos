#pragma once
class LagrangianPendulum {
	const float l = 1.0f;
	float theta1 = 0.0f;
	float theta1dot = 0.0f;
	float theta2 = 0.0f;
	float theta2dot = 0.0f;
	float g = 10.0f;

public:
	LagrangianPendulum(float l, float theta1, float theta1dot, float theta2, float theta2dot);
	void Update(float dt);
	void Draw();
};
