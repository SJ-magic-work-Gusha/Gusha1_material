/************************************************************
note
	if web cam on mac does not work, try
		> sudo killall VDCAssistant
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxSyphon.h"

#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class SJ_VIDEO_CAM : Noncopyable{
private:
	/****************************************
	****************************************/
	enum{
		VIDEO_WIDTH = 640,
		VIDEO_HEIGHT = 360,
		
	/*
		VIDEO_WIDTH = 960,
		VIDEO_HEIGHT = 540,
	*/
	};
	
	/****************************************
	****************************************/
	ofFbo fbo;
	int c_syphon;
	
	ofVideoGrabber *VideoCam;
	
	/****************************************
	****************************************/
	void Clear_fbo(ofFbo& fbo);
	void draw_to_fbo();

public:
	/****************************************
	****************************************/
	SJ_VIDEO_CAM();
	~SJ_VIDEO_CAM();
	
	void setup(int _Cam_id);
	void update();
	void draw(float _x, float _y, float _w, float _h);
	void publish_syphon(ofxSyphonServer& SyphonServer);
};
