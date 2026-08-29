#include "ofApp.h"
#include "StateSpaceController.h"
#include "CoOrdTransformer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Cambridge Lab");
	lagrangianpendulumBob.drawOffset = { -1.0f, -1.0f };
	pendulumBob.drawOffset = { -3.0f, -1.0f };
	pendulumBob2.drawOffset = { -3.0f, -1.0f };
	pendulum1.drawOffset = { 1.0f, -1.0f };
	pendulum2.drawOffset = { 1.0f, -1.0f };
	pendulum3.drawOffset = { 3.0f, -1.0f };

	ofTrueTypeFont::setGlobalDpi(96);
	font.load("monospace", 11);
}

//--------------------------------------------------------------
void ofApp::update(){
	dt = ft.Mark();
	time += dt;
	//if (SteppingForward)
	{
		if (demoNum == 1) {
			for (int i = 0; i < 16; i++) {
				//lagrangianpendulum.Update(dt / 16.0f);
				lagrangianpendulumBob.UpdateBob(dt / 16.0f);
				pendulumBob.Update(dt / 16.0f);
				pendulumBob2.Update(dt / 16.0f);
				pendulum1.Update(dt / 16.0f);
				pendulum2.Update(dt / 16.0f);
				pendulum3.Update(dt / 16.0f);
			}
		}
		if (demoNum == 2)
		{
			if (time > 0.2f)
			{
				time -= 0.2f;
				for (auto& c : constraintsSloMo)
				{
					Constraint::RelaxConstraint(c);
				}
			}
			for (int i = 1; i < rope.size(); i++)
			{
				auto particle = *rope[i];
				auto prevParticle = *rope[i - 1];
				Vec2 delta = prevParticle.GetPos() - particle.GetPos();
				//particle.SetForce("PrevParticle", delta.GetNormalized(), (delta.Len() - 0.1f) * 1000.0f);
				particle.Update(dt);
			}
			for (int i = 0; i < 8; i++) {
				for (int i = 0; i < constraints.size(); i++) {
					if (i == 0) {
						Constraint::RelaxConstraint(constraints[i], true);
					} else {
						Constraint::RelaxConstraint(constraints[i]);
					}
				}
			}
		}
		// SteppingForward = false;
	}
	//pendulum.SetPegVelX(StateSpaceController::GetControlSignal(6.0f, pendulum.GetPosX(), pendulum.GetVelX(),
	//	pendulum.getTheta(), pendulum.getThetaDot()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (demoNum == 1) {
		lagrangianpendulumBob.Draw();
		pendulumBob.Draw();
		pendulumBob2.Draw();
		pendulum3.Draw();
		pendulum1.Draw();
		pendulum2.Draw();
		CoOrdTransformer::DrawString(font, "Numerical Solution (bobs): ", { -4.0f, -2.5f }, (255, 255, 255));
		CoOrdTransformer::DrawString(font, "Lagrangian Solution (bobs): ", { -2.0f, -2.5f }, (255, 255, 255));
		CoOrdTransformer::DrawString(font, "Numerical Solution\n(Two rods): ", { 1.0f, -2.5f }, (255, 255, 255));
		CoOrdTransformer::DrawString(font, "Numerical Solution\n(One rod): ", { 3.0f, -2.5f }, (255, 255, 255));
	}
	else if (demoNum == 2)
	{
		for (auto i = rope.begin(); i != rope.end() - 1; i++)
		{
			CoOrdTransformer::DrawLine(i->get()->GetPos(), (i + 1)->get()->GetPos(), (255,255,255,255));
		}
		for (auto i = ropeSloMo.begin(); i != ropeSloMo.end() - 1; i++) {
			CoOrdTransformer::DrawLine(i->get()->GetPos(), (i + 1)->get()->GetPos(), (255, 255, 255, 255));
		}
	}
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
	if (key == '1') {
		demoNum = 1;
	}
	if (key == '2') {
		demoNum = 2;
		time = 0.0f;
		ropeSloMo.clear();
		Vec2 curPos = { -1.5f, 0.5f };
		for (int i = 0; i < 10; i++)
		{
			curPos.x += 0.1f;
			std::unique_ptr<JakobsenParticle> p = std::make_unique<JakobsenParticle>(curPos, 1.0f);
			ropeSloMo.emplace_back(std::move(p));
		}
		for (int i = 0; i < 10; i++)
		{
			curPos.y -= 0.1f;
			std::unique_ptr<JakobsenParticle> p = std::make_unique<JakobsenParticle>(curPos, 1.0f);
			ropeSloMo.emplace_back(std::move(p));
		}
		for (int i = 0; i < 10; i++) {
			curPos.x += 0.1f;
			std::unique_ptr<JakobsenParticle> p = std::make_unique<JakobsenParticle>(curPos, 1.0f);
			ropeSloMo.emplace_back(std::move(p));
		}
		for (int i = 0; i < 10; i++) {
			curPos.y += 0.1f;
			std::unique_ptr<JakobsenParticle> p = std::make_unique<JakobsenParticle>(curPos, 1.0f);
			ropeSloMo.emplace_back(std::move(p));
		}
		for (int i = 0; i < 10; i++) {
			curPos.x += 0.1f;
			std::unique_ptr<JakobsenParticle> p = std::make_unique<JakobsenParticle>(curPos, 1.0f);
			ropeSloMo.emplace_back(std::move(p));
		}
		for (int i = 0; i < ropeSloMo.size() - 1; i++)
		{
			constraintsSloMo.push_back({ *ropeSloMo[i], *ropeSloMo[i + 1], 0.05f });
		}
		curPos = { 1.0f, -1.0f };
		for (int i = 0; i < 20; i++) {
			std::unique_ptr<JakobsenParticle> p = std::make_unique<JakobsenParticle>(curPos, 1.0f);
			rope.emplace_back(std::move(p));
			if (i != 0) {
				rope[i]->SetForce("Gravity", { 0.0f, 1.0f }, 10.0f);
			}
			curPos.y += 0.1f;
		}
		for (int i = 0; i < rope.size() - 1; i++) {
			constraints.push_back({ *rope[i], *rope[i + 1], 0.1f });
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	if (demoNum == 2) {
		rope[0]->MoveBy(CoOrdTransformer::InverseTransform(Vec2 { (float)x, (float)y }) - rope[0]->GetPos());
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
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
