#pragma once
#include "Vec2.h"
#include "ofMain.h"

class CoOrdTransformer {
public:
	static const int factor = 8;
	static Vec2 GetTransformed(Vec2 cIn) {
		float width = (float)ofGetWidth();
		float height = (float)ofGetHeight();
		Vec2 offset = { width / 2.0f, height / 2.0f };
		return offset + cIn * std::min(width, height) / (float)factor;
	}
	static Vec2 InverseTransform(Vec2 p) {
		float width = (float)ofGetWidth();
		float height = (float)ofGetHeight();
		Vec2 offset = { width / 2.0f, height / 2.0f };
		return (p - offset) * (float)factor / std::min(width, height);
	}
	static void DrawLine(Vec2 p1, Vec2 p2, ofColor c) {
		ofSetColor(c);
		Vec2 t1 = GetTransformed(p1);
		Vec2 t2 = GetTransformed(p2);
		ofDrawLine(t1.x, t1.y, t2.x, t2.y);
	}
	static void DrawArrow(Vec2 p, Vec2 dir, float mag, ofColor c)
	{
		Vec2 start = GetTransformed(p);
		Vec2 end = GetTransformed(p + dir * mag);
		ofSetColor(c);
		ofDrawArrow({ start.x, start.y, 1 }, { end.x, end.y, 1 }, 10.0f);
	}
	static void DrawCircle(Vec2 p, float radius, ofColor c) {
		ofSetColor(c);
		ofDrawCircle({ GetTransformed(p).x, GetTransformed(p).y }, radius * std::min(ofGetWidth() / (float)factor, ofGetHeight() / (float)factor));
	}
	static void DrawString(ofTrueTypeFont& font, std::string text, Vec2 pos, ofColor c) {
		ofSetColor(c);
		font.drawString(text, GetTransformed(pos).x, GetTransformed(pos).y);
	}
};
