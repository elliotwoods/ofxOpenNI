//
//  ofxIRGenerator.h
//  UserTracking
//
//  Created by Elliot Woods on 18/06/2011.
//  Copyright 2011 Kimchi and Chips. All rights reserved.
//

#include "ofxOpenNIContext.h"

class ofxIRGenerator {
public:
	ofxIRGenerator(bool useTexture = true);
    ~ofxIRGenerator();
	
	bool setup(ofxOpenNIContext* pContext);
    
	void generateTexture();
	
    void update();
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	xn::IRGenerator& getXnIRGenerator();
    
    void setUseTexture(bool b);
	
private:
    void    allocate();
    void    deAllocate();
    
	xn::IRGenerator IR_generator;
	ofTexture IR_texture;
	UInt16* IR_pixels;
    unsigned char* IR_pixels_view;
    bool bUseTexture;
    
    XnUInt32 lastFrameUpdate;
};
