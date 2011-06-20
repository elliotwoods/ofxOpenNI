//
//  ofxIRGenerator.h
//  UserTracking
//
//  Created by Elliot Woods on 18/06/2011.
//  Copyright 2011 Kimchi and Chips. All rights reserved.
//

#pragma once

#include "ofxOpenNIContext.h"
#include "ofBaseTypes.h"

class ofxIRGenerator : public ofBaseHasPixels {
public:
	ofxIRGenerator(bool useTexture = true);
    ~ofxIRGenerator();
	
	bool setup(ofxOpenNIContext* pContext);
    
	void update();
	
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	xn::IRGenerator& getXnIRGenerator();
    
    void setUseTexture(bool b);
    
    ofTexture& getTextureReference() { return IR_texture; }
    
    ofPixels& getPixelsRef() { return IR_pixels_view; }
    unsigned char * getPixels() { return IR_pixels_view.getPixels(); }
	
private:
    void    allocate();
    
	xn::IRGenerator IR_generator;
	ofTexture IR_texture;
	UInt16* IR_pixels;
    ofPixels IR_pixels_view;
    bool bUseTexture;
    
    XnUInt32 lastFrameUpdate;
};
