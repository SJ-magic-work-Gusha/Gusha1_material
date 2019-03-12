/************************************************************
************************************************************/
#include "VideoCam.h"

/************************************************************
************************************************************/

/******************************
******************************/
SJ_VIDEO_CAM::SJ_VIDEO_CAM()
: VideoCam(NULL)
, c_syphon(0)
{
}

/******************************
******************************/
SJ_VIDEO_CAM::~SJ_VIDEO_CAM()
{
	if(VideoCam) delete VideoCam;
}

/******************************
******************************/
bool SJ_VIDEO_CAM::setup(int _Cam_id)
{
	/********************
	********************/
	VideoCam = new ofVideoGrabber;
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    VideoCam->setVerbose(true);
	
	vector< ofVideoDevice > Devices = VideoCam->listDevices();// 上 2行がないと、List表示されない.
	
	if(_Cam_id == -1){
		// std::exit(1);
		return false;
	}else{
		if(Devices.size() <= _Cam_id) { ERROR_MSG(); std::exit(1); }
		
		VideoCam->setDeviceID(_Cam_id);
		VideoCam->initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);
		
		printf("> Cam size asked = (%d, %d), actual = (%d, %d)\n", int(VIDEO_WIDTH), int(VIDEO_HEIGHT), int(VideoCam->getWidth()), int(VideoCam->getHeight()));
		fflush(stdout);
	}

	/********************
	********************/
	fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT, GL_RGBA, 0);
	Clear_fbo(fbo);
	
	return true;
}

/******************************
******************************/
void SJ_VIDEO_CAM::Clear_fbo(ofFbo& fbo)
{
	fbo.begin();
	
	// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	ofClear(0, 0, 0, 0);
	
	fbo.end();
}

/******************************
******************************/
void SJ_VIDEO_CAM::update()
{
	VideoCam->update();
	if(VideoCam->isFrameNew()){
		draw_to_fbo();
	}
}

/******************************
******************************/
void SJ_VIDEO_CAM::publish_syphon(ofxSyphonServer& SyphonServer)
{
	ofTexture tex = fbo.getTextureReference();
	SyphonServer.publishTexture(&tex);
}

/******************************
******************************/
void SJ_VIDEO_CAM::draw_to_fbo()
{
	fbo.begin();
		/********************
		********************/
		// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ofClear(0, 0, 0, 0);
		
		/********************
		********************/
		ofColor BaseColor = Gui_Global->gui_VideoColor;
		ofSetColor(BaseColor);
		// ofSetColor(255, 255, 255, 255);
		
		VideoCam->draw(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();
}

/******************************
******************************/
void SJ_VIDEO_CAM::draw(float _x, float _y, float _w, float _h)
{
	ofSetColor(255, 255, 255, 255);
	fbo.draw(_x, _y, _w, _h);
}

