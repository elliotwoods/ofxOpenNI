#pragma once

#include "ofxOpenNIContext.h"

#define MAX_DEPTH 10000
class ofxDepthGenerator {
public:
	ofxDepthGenerator();
	
	bool setup(ofxOpenNIContext* pContext);
	
	void generateTexture();
	
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	unsigned char* getPixels();
	unsigned char* getGrayPixels();
	
	xn::DepthGenerator& getXnDepthGenerator();
    
    ofTexture& getTextureReference() { return depth_texture; }
	
private:
	xn::DepthGenerator depth_generator;
	ofTexture depth_texture;
	unsigned char* depth_pixels;
	unsigned char* gray_pixels; 
	int depth_coloring;
	float max_depth;
	float depth_hist[MAX_DEPTH];
};

