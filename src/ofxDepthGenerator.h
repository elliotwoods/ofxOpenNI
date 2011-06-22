#pragma once

#include "ofxOpenNIContext.h"

#define MAX_DEPTH 10000

typedef enum {
    ONI_DEPTH_PIXELMODE_YELLOW_NORMALISED = 0,
    ONI_DEPTH_PIXELMODE_YELLOW_RAW,
    ONI_DEPTH_PIXELMODE_GRAY_RAW
} ofxOpenNIDepthPixelMode;

class ofxDepthGenerator {
public:
	ofxDepthGenerator();
	
	bool setup(ofxOpenNIContext* pContext);
	
    void update();
    void generateTexture() { update(); }
	
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	unsigned char*      getPixels();
    const XnDepthPixel* getDepthPixels(); // returns depths in mm's
	
	xn::DepthGenerator& getXnDepthGenerator();
    
    ofTexture& getTextureReference() { return depthPreview; }
    bool getIsFrameNew() const {return isFrameNew; }
    
    void setPreviewMode(ofxOpenNIDepthPixelMode mode);
    void setClearPreview(bool b);
    
    //get position in meters of pixel (i,j)
    void    getWorldXYZ(int i, int j, float* xyz);
    ofVec3f getWorldXYZ(int i, int j);
    ofVec3f getWorldXYZ(ofVec2f xy);
	
private:
    
    void    updateData();
	void    updateTexture();
    
    
	xn::DepthGenerator      depth_generator;
    
    XnDepthPixel*           depthPixels;
    
	int     width, height;
    
	float   max_depth;
	float   depth_hist[MAX_DEPTH];
    
    XnUInt32                lastFrameUpdate;
    bool                    isFrameNew;
    
    //preview
	ofTexture               depthPreview;
	unsigned char*          depthPreviewPixels;
    
    bool                    bUseTexture;
    ofxOpenNIDepthPixelMode previewMode;
    bool                    bClearPreview;
};

