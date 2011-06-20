//
//  ofxIRGenerator.cpp
//  UserTracking
//
//  Created by Elliot Woods on 18/06/2011.
//  Copyright 2011 Kimchi and Chips. All rights reserved.
//

#include "ofxIRGenerator.h"

ofxIRGenerator::ofxIRGenerator(bool useTexture) : 
bUseTexture(useTexture),
lastFrameUpdate(0)
{
}

ofxIRGenerator::~ofxIRGenerator()
{    
    delete[] IR_pixels;
}

void ofxIRGenerator::update(){
    
    lastFrameUpdate = IR_generator.GetFrameID();
    
	xn::IRMetaData imd;
	IR_generator.GetMetaData(imd);	
    
	const XnIRPixel* pImage = imd.Data();
	memcpy(IR_pixels, pImage, sizeof(unsigned char) * imd.XRes() * imd.YRes()*2);
    
    if (bUseTexture)
    {
        int nPixels = imd.XRes() * imd.YRes();
        
        for (int i=0; i<nPixels; i++)
            IR_pixels_view[i] = IR_pixels[i] >> 2; // 10bit->8bit
        
        IR_texture.loadData(getPixels(),imd.XRes(), imd.YRes(), GL_LUMINANCE8);
    }
}

void ofxIRGenerator::draw(float x, float y, float w, float h){

    //if we haven't manually updated
    if (lastFrameUpdate != IR_generator.GetFrameID())
        update();
	
    glColor3f(1,1,1);
	IR_texture.draw(x, y, w, h);		
}

bool ofxIRGenerator::setup(ofxOpenNIContext* pContext) {
	if(!pContext->isInitialized()) {
		return false;
	}
	
	//Create image generator
	XnStatus result = IR_generator.Create(pContext->getXnContext());
	
	if (result != XN_STATUS_OK){
		printf("Setup IR Image failed: %s\n", xnGetStatusString(result));
		return false;
	} else {
		ofLog(OF_LOG_VERBOSE, "Create IR Generator inited");
		
		XnMapOutputMode map_mode; 
		map_mode.nXRes = XN_VGA_X_RES; 
		map_mode.nYRes = XN_VGA_Y_RES; 
		map_mode.nFPS = 30;
		
		result = IR_generator.SetMapOutputMode(map_mode);
				
		IR_pixels = new UInt16[map_mode.nXRes * map_mode.nYRes * 2];
		
		IR_generator.StartGenerating();		
        
        if (bUseTexture)
            allocate();
        
		return true;
	}		
}

xn::IRGenerator& ofxIRGenerator::getXnIRGenerator(){
	return IR_generator;
}

void ofxIRGenerator::setUseTexture(bool b)
{
    //check if we need to allocate texture
    if (!bUseTexture)
        allocate();
        
    bUseTexture = b;
}

void ofxIRGenerator::allocate()
{
    XnMapOutputMode map_mode;
    IR_generator.GetMapOutputMode(map_mode);
    
    //allocate grey pixels
    IR_pixels_view.allocate(map_mode.nXRes, map_mode.nYRes, OF_PIXELS_MONO);
    
    //allocate texture
    if (!IR_texture.bAllocated())
        IR_texture.allocate(map_mode.nXRes, map_mode.nYRes, GL_LUMINANCE8);
    
}