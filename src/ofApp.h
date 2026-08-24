#pragma once

#include "ofMain.h"
#include "Pendulum.h"
#include "FrameTimer.h"
#include "LagrangianPendulum.h"
#include "PendulumBob.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		//Pendulum pendulum1 = Pendulum(1.0f, PI / 8.0f, 0.0f, nullptr);
		//Pendulum pendulum2 = Pendulum(1.0f, 0.0f, 0.0f, &pendulum1);
		//Pendulum pendulum3 = Pendulum(1.0f, PI / 3.0f, 0.0f, nullptr);
		LagrangianPendulum lagrangianpendulumBob = LagrangianPendulum { 1.0f, PI / 8.0f, 0.0f, 0.0f, 0.0f };
		//LagrangianPendulum lagrangianpendulum = LagrangianPendulum { 1.0f, PI / 8.0f, 0.0f, 0.0f, 0.0f };
		FrameTimer ft;
		PendulumBob pendulumBob = PendulumBob(PI / 8.0f, 1.0f, 1.0f, nullptr);
		PendulumBob pendulumBob2 = PendulumBob(0.0f, 1.0f, 1.0f, &pendulumBob);
		bool SteppingForward = false;
		float dt = 0.0f;
};
