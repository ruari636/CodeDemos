class StateSpaceController
{
	static const bool Crane = true;

public:
	static float GetControlSignal(float xDesired, float x, float xdot, float theta, float thetaDot);
};
