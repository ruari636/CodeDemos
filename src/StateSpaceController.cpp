#include "StateSpaceController.h"
#include <minmax.h>

const float p1Crane = 0.025f;
const float p2Crane = 0.025f;
const float p3Crane = 0.125f;
const float p4Crane = 0.025f;

float StateSpaceController::GetControlSignal(float xDesired, float x, float xdot, float theta, float thetaDot) {
	if (Crane)
	{
		float out = min(- 20 * p1Crane * (x - xDesired) - 30 * p2Crane * xdot + 4 * p3Crane * theta - 2 * p4Crane * thetaDot, 1.0f);
		return out;
	}
}
