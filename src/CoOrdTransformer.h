#pragma once
#include "Vec2.h"
#include "ofMain.h"

class CoOrdTransformer {
public:
	static Vec2 GetTransformed(Vec2 cIn) {
		float width = (float)ofGetWidth();
		float height = (float)ofGetHeight();
		Vec2 offset = { width / 2.0f, height / 2.0f };
		return offset + cIn * std::min(width, height) / 10.0f;
	}
	static void DrawLine(Vec2 p1, Vec2 p2) {
		Vec2 t1 = GetTransformed(p1);
		Vec2 t2 = GetTransformed(p2);
		ofDrawLine(t1.x, t1.y, t2.x, t2.y);
	}
	static void DrawArrow(Vec2 p, Vec2 dir, float mag)
	{
		Vec2 start = GetTransformed(p);
		Vec2 end = GetTransformed(p + dir * mag);
		ofSetColor(( 0, 255, 0 ));
		ofDrawArrow({ start.x, start.y, 1 }, { end.x, end.y, 1 }, 10.0f);
		ofSetColor((255, 255, 255));
	}
};
