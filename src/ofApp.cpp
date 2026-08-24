#include "ofApp.h"
#include "StateSpaceController.h"
#include "CoOrdTransformer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Cambridge Lab");
	lagrangianpendulumBob.drawOffset = { 1.0f, -1.0f };
	pendulumBob.drawOffset = { -1.0f, -1.0f };
	pendulumBob2.drawOffset = { -1.0f, -1.0f };
}

//--------------------------------------------------------------
void ofApp::update(){
	dt = ft.Mark();
	//if (SteppingForward)
	{
		for (int i = 0; i < 16; i++) {
			//lagrangianpendulum.Update(dt / 16.0f);
			lagrangianpendulumBob.UpdateBob(dt / 16.0f);
			pendulumBob.Update(dt / 16.0f);
			pendulumBob2.Update(dt / 16.0f);
		}
		SteppingForward = false;
	}
	//pendulum.SetPegVelX(StateSpaceController::GetControlSignal(6.0f, pendulum.GetPosX(), pendulum.GetVelX(),
	//	pendulum.getTheta(), pendulum.getThetaDot()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	lagrangianpendulumBob.Draw();
	pendulumBob.Draw();
	pendulumBob2.Draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'f')
	{
		ofToggleFullscreen();
	}
	if (key == 'd')
	{
		SteppingForward = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
