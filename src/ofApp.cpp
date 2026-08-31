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
		if (demoNum == 2) {
			timeCloth += dt;
			cloth1->SetTopLeftPos(Vec2 { 1.5f * sin(timeCloth), -3.5f + cos(timeCloth + 0.5f) });
			cloth1->SetTopRightPos(Vec2 { sin(timeCloth), -2.5f + cos(timeCloth) });
			cloth2->SetTopLeftPos(Vec2 { 1.5f * sin(timeCloth), -1.0f + cos(timeCloth + 0.5f) });
			cloth2->SetTopRightPos(Vec2 { sin(timeCloth), cos(timeCloth) });
			{
				cloth1->Update(dt, 16, true, true);
				cloth2->Update(dt, 0, true, false);
				SteppingForward = false;
			}
			if (time > 0.5f && transitionNum < 255.0f)
			{
				time -= 0.5f;
				transitionNum += 255.0f / 16;
				for (auto& c : constraintsSloMo)
				{
					Constraint::RelaxConstraint(c);
				}
			}
			for (int i = (int)rope.size() - 1; i > 1; i--)
			{
				auto& particle = *rope[i];
 				auto& nextParticle = *rope[i - 1];
				Vec2 delta = particle.GetPos() - nextParticle.GetPos();
				// delta pointing down initially
				Vec2 normalised = delta.GetNormalized();
				Force fPrev = particle.GetForce("PrevParticle");
				Vec2 newForce = fPrev.Vector() + particle.GetForce("Gravity").Vector();
				Force Next = { newForce.Len(), newForce.GetNormalized() };
				nextParticle.SetForce("PrevParticle", normalised, Next.Vector() * normalised);
				particle.SetForce("SpringK", -normalised, 1.0e4 * (delta.Len() - 0.1f));
				particle.Update(dt);
			}
			for (int i = 0; i < 16; i++) {
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
			CoOrdTransformer::DrawLine(i->get()->GetPos(), (i + 1)->get()->GetPos(), (255,255,255));
		}
		for (auto i = ropeSloMo.begin(); i != ropeSloMo.end() - 1; i++) {
			CoOrdTransformer::DrawLine(i->get()->GetPos(), (i + 1)->get()->GetPos(), ofColor(255.0f - transitionNum,transitionNum, 0.0f));
		}
		cloth1->Draw(ofColor(0, 0,127));
		cloth2->Draw(ofColor(127, 0, 0));
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
	if (key == 'a') {
		cloth1->SetTopLeftPos(cloth1->GetTopLeftPos() - Vec2{ dt * 0.01f, 0.0f });
	}
	if (key == '1') {
		demoNum = 1;
	}
	if (key == '2') {
		demoNum = 2;
		time = 0.0f;
		timeCloth = 0.0f;
		transitionNum = 0.0f;
		constraints.clear();
		constraintsSloMo.clear();
		ropeSloMo.clear();
		rope.clear();
		delete cloth1;
		cloth1 = new Cloth(10, 10, { 0.0f, 0.0f });
		Vec2 curPos = { -3.5f, 0.5f };
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
