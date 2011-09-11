/*
 *  Turtle.h
 *  Lsystem
 *
 *  Created by Daan on 02-04-11.
 *  Copyright 2011 Daan van Hasselt. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include <vector>

#define MIN_L_LENGTH (1)
#define MAX_L_LENGTH (20)

class Turtle {

public:
	Turtle();
	Turtle(string forward, string left, string right);
	void draw(string input, float x, float y, float angle);
	void moveForward();
	void turnLeft();
	void turnRight();
	void setMyColor(ofColor newColor) {myColor = newColor;};
	void setFadeFactor(float newFactor) {fadeFactor = newFactor;};
	void setMaxLeafDepth(float newLeafDepth) {maxLeafDepth = newLeafDepth;};

    void setFont(ofTrueTypeFont* _pMyFont) {pMyFont = _pMyFont;};

    void setLabelString(const std::string& _labelString) {labelString = _labelString;};

	string forward;
	string left;
	string right;

	float angle;
	float curAngle;
	float length;
	float x;
	float y;
	ofColor myColor;
	float fadeFactor;

	vector<float> xHis;
	vector<float> yHis;
	vector<float> aHis;

	void pushValues();
	void popValues();

    ofTrueTypeFont* pMyFont;
    std::string labelString;
    unsigned char buildNumber;
    std::string labelBuildString;
    int maxLeafDepth;
    void drawVerString();
};
