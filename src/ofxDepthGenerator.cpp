#include "ofxDepthGenerator.h"
#include "ofxOpenNIMacros.h"

XnUInt8 PalletIntsR [256] = {0};
XnUInt8 PalletIntsG [256] = {0};
XnUInt8 PalletIntsB [256] = {0};

////////////////////
//constructor, setup
ofxDepthGenerator::ofxDepthGenerator() :
width(0), 
height(0),
isFrameNew(false),
bUseTexture(true),
previewMode(ONI_DEPTH_PIXELMODE_YELLOW_NORMALISED),
bClearPreview(true)
{
    
}

bool ofxDepthGenerator::setup(ofxOpenNIContext* pContext) {
	if(!pContext->isInitialized()) {
		return false;
	}
	
	//context = rContext;
	
	// When the context is using a recording we need to fetch the depth generator.
	// --------------------------------------------------------------------------
	/*
     if(!pContext->isUsingRecording()) {
     XnStatus result = depth_generator.Create(pContext->getXnContext());
     CHECK_RC(result, "Creating depth generator using recording");
     }
     else {
     pContext->getDepthGenerator(this);
     }
	 */
	XnStatus result = XN_STATUS_OK;	
	
	// check if the USER generator exists.
	result = pContext->getXnContext()
    .FindExistingNode(XN_NODE_TYPE_DEPTH, depth_generator);
	SHOW_RC(result, "Find depth generator");
	if(result != XN_STATUS_OK) {
		result = depth_generator.Create(pContext->getXnContext());
		SHOW_RC(result, "Create depth generator");
		if(result != XN_STATUS_OK) {			
			return false;
		}
	}	
	
	
	ofLog(OF_LOG_VERBOSE, "Depth camera inited");
	
	
	//Set the input to VGA (standard is QVGA wich is not supported on the Kinect)
	XnMapOutputMode map_mode; 
	map_mode.nXRes = XN_VGA_X_RES; 
	map_mode.nYRes = XN_VGA_Y_RES;
	map_mode.nFPS = 30;
	//
	result = depth_generator.SetMapOutputMode(map_mode);
	max_depth = depth_generator.GetDeviceMaxDepth();		
    
    
    // start her up!
	depth_generator.StartGenerating();	
    
    
    // allocate memory
    width = map_mode.nXRes;
    height = map_mode.nYRes;
    
    depthPixels = new XnDepthPixel[width * height];
	depthPreview.allocate(width, height, GL_RGB);		
	depthPreviewPixels = new unsigned char[width * height * 4];
    
	return true;
	
}

////////////////////
//get functions
unsigned char* ofxDepthGenerator::getPixels() {
	return depthPreviewPixels;
}

const XnDepthPixel* ofxDepthGenerator::getDepthPixels(){
	return depthPixels;
}

xn::DepthGenerator& ofxDepthGenerator::getXnDepthGenerator() {
	return depth_generator;
}

////////////////////
//update,draw


void ofxDepthGenerator::update()
{    
    updateData();
    
    if (!isFrameNew)
        return;
    
    if (bUseTexture)
        updateTexture();
    
}

void ofxDepthGenerator::updateData()
{
    //we presume already checked if new frame (e.g. in update)
    
    // get meta-data
    xn::DepthMetaData metaDepthFrame;
	depth_generator.GetMetaData(metaDepthFrame);	
    XnUInt32 FrameID = metaDepthFrame.FrameID();

    //if we're still loading. if so, then ignore
	if (FrameID == 0)
		return;
    
    // check if frame is new
    isFrameNew = (lastFrameUpdate != FrameID);
    if (!isFrameNew)
        return;
    lastFrameUpdate = FrameID;

    // get the pixels pointer
	const XnDepthPixel* pixelsIn = metaDepthFrame.Data();
	XN_ASSERT(pixelsIn);
    
    // make a local copy
    // since we have no cropping supported,
    // we can presume XOffset() == YOffset() == 0
    memcpy(depthPixels, pixelsIn, sizeof(XnDepthPixel) * width * height);
    
}

void ofxDepthGenerator::updateTexture()
{
    if (bClearPreview)
        memset(depthPreviewPixels, width*height*3, 0);
    
    switch(previewMode)
    {
        case ONI_DEPTH_PIXELMODE_YELLOW_NORMALISED:
        {
            XnDepthPixel *depthPixelsMover = depthPixels;
            
            // count histogram bins
            memset(depth_hist, 0, MAX_DEPTH * sizeof(float));
            unsigned int num_of_points = 0;
            for (XnUInt y = 0; y < height; ++y)
                for (XnUInt x = 0; x < width; ++x, ++depthPixelsMover)
                    if (*depthPixelsMover != 0) {
                        depth_hist[*depthPixelsMover]++;
                        num_of_points++;
                    }

            // make histogram cumulative
            for (int i=1; i < MAX_DEPTH; i++)
                depth_hist[i] += depth_hist[i-1];
            
            // normalize histogram
            if(num_of_points)
                for(int i = 0; i < MAX_DEPTH; ++i) {
                    depth_hist[i] = (unsigned int)(256 * (1.0f - (depth_hist[i] / num_of_points)));
                }
            
            // assign colour values
            depthPixelsMover = depthPixels;
            unsigned char * depthPreviewPixelsMover = depthPreviewPixels;
            int hist_value;
            //
            for(int i=0; i<width*height; ++i){
                hist_value = depth_hist[*depthPixelsMover];
                
                depthPreviewPixelsMover[0] = hist_value;
                depthPreviewPixelsMover[1] = hist_value;
                depthPreviewPixelsMover[2] = 0;
                
                depthPixelsMover++;
                depthPreviewPixelsMover+=3;
            }
            
            break;
        }
            
        case ONI_DEPTH_PIXELMODE_YELLOW_RAW:
        {
            XnDepthPixel *depthPixelsMover = depthPixels;
            unsigned char * depthPreviewPixelsMover = depthPreviewPixels;
            //
            unsigned char value;
            for (int i=0; i<width*height; i++)
            {
                //to 8bit (optimised automatically to >> 6)
                value = *depthPixelsMover >> XnDepthPixel(ceil(log(MAX_DEPTH)/log(2) - 8));
                
                depthPreviewPixelsMover[0] = value;
                depthPreviewPixelsMover[1] = value;
                depthPreviewPixelsMover[2] = 0;
                
                depthPixelsMover++;
                depthPreviewPixelsMover+=3;
            }
            
            break;
            
        }
            
            
        case ONI_DEPTH_PIXELMODE_GRAY_RAW:
        {
            XnDepthPixel *depthPixelsMover = depthPixels;
            unsigned char * depthPreviewPixelsMover = depthPreviewPixels;
            //
            unsigned char value;
            for (int i=0; i<width*height; i++)
            {
                //to 8bit (optimised automatically to >> 6)
                value = *depthPixelsMover >> XnDepthPixel(ceil(log(MAX_DEPTH)/log(2) - 8));
                
                depthPreviewPixelsMover[0] = value;
                depthPreviewPixelsMover[1] = value;
                depthPreviewPixelsMover[2] = value;
                
                depthPixelsMover++;
                depthPreviewPixelsMover+=3;
            }
            
            break;
            
        }
    }
	
    
	
	depthPreview.loadData(depthPreviewPixels, width, height, GL_RGB);	
    
    // other draw modes removed since already depreciated / probably unnecessary
}

void ofxDepthGenerator::draw(float x, float y, float w, float h){
    
    update();
    
	depthPreview.draw(x, y, w, h);	
}

////////////////////
// set preview mode

void ofxDepthGenerator::setPreviewMode(ofxOpenNIDepthPixelMode mode)
{
    previewMode = mode;
}

void ofxDepthGenerator::setClearPreview(bool b)
{
    bClearPreview = b;
}


////////////////////
// depth maths
const float fx_d = 1.0f / 5.9421434211923247e+02f;
const float fy_d = 1.0f / 5.9104053696870778e+02f;
const float cx_d = 3.3930780975300314e+02f;
const float cy_d = 2.4273913761751615e+02f;

void ofxDepthGenerator::getWorldXYZ(int i, int j, float* xyz)
{
    // get the pixel  
    float depth = depthPixels[i + j * width];
    
    if (depth < 0.1)
    {
        xyz[0] = 0;
        xyz[1] = 0;
        xyz[2] = 0;
    } else {
        xyz[2] = depth / 1000.0f;
        xyz[0] = (float(i) - cx_d) * fx_d * xyz[2];
        xyz[1] = -(float(j) - cy_d) * fy_d * xyz[2];
    }
}

ofVec3f ofxDepthGenerator::getWorldXYZ(int i, int j)
{
    ofVec3f xyz;
    getWorldXYZ(i, j, &xyz.x);
    return xyz;
}

ofVec3f ofxDepthGenerator::getWorldXYZ(ofVec2f xy)
{
    ofVec3f xyz;
    getWorldXYZ(xy.x, xy.y, &xyz.x);
    return xyz;
}

