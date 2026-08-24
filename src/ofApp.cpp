#include "ofApp.h"
#include "StateSpaceController.h"
#include "CoOrdTransformer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Cambridge Lab");
}

//--------------------------------------------------------------
void ofApp::update(){
	dt = ft.Mark();
	//if (SteppingForward)
	for (int i = 0; i < 32; i++)
	{
		lagrangianpendulum.Update(dt / 64.0f);
		pendulum1.Update(dt / 64.0f);
		pendulum2.Update(dt / 64.0f);
	}
	//pendulum.SetPegVelX(StateSpaceController::GetControlSignal(6.0f, pendulum.GetPosX(), pendulum.GetVelX(),
	//	pendulum.getTheta(), pendulum.getThetaDot()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	lagrangianpendulum.Draw();
	pendulum1.Draw();
	pendulum2.Draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'f')
	{
		ofToggleFullscreen();
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
