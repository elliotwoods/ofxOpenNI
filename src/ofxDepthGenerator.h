#pragma once

#include "ofxOpenNIContext.h"

#define MAX_DEPTH 10000

//slower but more accurate?
//also versatile if new devices emerge
#define ONI_DEPTH_USE_OPENNI_XYZ_METHOD

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
    
    //get position in mm of pixel (x,y)
	//x, y are pixel coords from top-left of image
    ofVec3f getWorldXYZ(float const x, float const y) const;
    ofVec3f getWorldXYZ(ofVec2f const xy) const; 
	void	getWorldXYZ(vector<ofVec2f> const &xy, vector<ofVec3f> &xyz) const;
	
	void	getAllWorldXYZ(float* allWorldXYZ);
	void	getAllWorldXYZ(vector<ofVec3f> &WorldXYZ);
	
	float	getWidth() { return width; }
	float	getHeight() { return width; }
	
	
protected:
    
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
	
	//xyz calculation
	void    getWorldXYZ(ofVec2f const * const xy, ofVec3f * xyz, const int count) const;
};

