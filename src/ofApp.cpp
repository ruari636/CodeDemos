#include "ofApp.h"
#include "StateSpaceController.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Cambridge Lab");
}

//--------------------------------------------------------------
void ofApp::update(){
	dt = ft.Mark();
	//if (SteppingForward)
	for (int i = 0; i < 10; i++)
	{
		//pendulum2.Update(dt / 16.0f);
		pendulum1.Update(dt / 10.0f);
		SteppingForward = false;
	}
	//pendulum.SetPegVelX(StateSpaceController::GetControlSignal(6.0f, pendulum.GetPosX(), pendulum.GetVelX(),
	//	pendulum.getTheta(), pendulum.getThetaDot()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	pendulum1.Draw();
	//pendulum2.Draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'f')
	{
		ofToggleFullscreen();
	}
	else if (key == 'a') {
		pendulum1.MovePeg(-30.0f, dt);
	}
	else if (key == 'd') {
		//pendulum1.MovePeg(30.0f, dt);
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
