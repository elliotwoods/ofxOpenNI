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
    const XnDepthPixel* getDepthPixels();
	
	xn::DepthGenerator& getXnDepthGenerator();
    
    ofTexture& getTextureReference() { return depth_texture; }
    bool getIsFrameNew() const {return isFrameNew; }
    
    //get position in meters of pixel (i,j)
    ofVec3f getWorldXYZ(int i, int j);
    void    getWorldXYZ(int i, int j, float* xyz);
	
private:
	xn::DepthGenerator depth_generator;
	ofTexture depth_texture;
    
	unsigned char* depth_preview_pixels;
	unsigned char* gray_pixels; 
    const XnDepthPixel* depth_pixels;
    
	int width, height;
    
    int depth_coloring;
	float max_depth;
	float depth_hist[MAX_DEPTH];
    
    XnUInt32 lastFrameUpdate;
    bool isFrameNew;
};

