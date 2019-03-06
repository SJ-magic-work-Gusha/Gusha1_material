/************************************************************
************************************************************/
#include "mov.h"

/************************************************************
************************************************************/

/******************************
******************************/
MOV::MOV()
{
}

/******************************
******************************/
MOV::~MOV()
{
}

/******************************
******************************/
void MOV::Clear_fbo(ofFbo& fbo)
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
void MOV::setup(bool MovSound_on)
{
	/********************
	********************/
	fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT, GL_RGBA, 0);
	Clear_fbo(fbo);
	
	/********************
	********************/
	// if(video.load("mov/LIVEAID Hap 640.mov")){
	if(video.load("mov/mov.mp4")){
		setup_video(video, MovSound_on);
	}else{
		printf("> No mov file.\n");
		fflush(stdout);
	}
}

/******************************
******************************/
#ifdef HAP
void MOV::setup_video(ofxHapPlayer& video, bool MovSound_on)
#else
void MOV::setup_video(ofVideoPlayer& video, bool MovSound_on)
#endif
{	
	if(video.isLoaded()){
		/*
		printf("loaded movie : (%f, %f)\n", video.getWidth(), video.getHeight());
		fflush(stdout);
		*/
		
		video.setLoopState(OF_LOOP_NORMAL);
		// video.setLoopState(OF_LOOP_PALINDROME);
		
		video.setSpeed(1);
		
		if(MovSound_on)	video.setVolume(1.0);
		else			video.setVolume(0.0);
		
		video.play();
	}
}

/******************************
******************************/
void MOV::SeekToTop()
{
	if(video.isLoaded()){
		video.setPosition(0); // percent.
	}
}

/******************************
******************************/
void MOV::Seek(float pos)
{
	if(video.isLoaded()){
		if(video.isPaused()) video.setPosition(pos); // percent.
	}
}

/******************************
******************************/
void MOV::setPaused(bool b_pause)
{
	if(video.isLoaded()){
		video.setPaused(b_pause);
	}
}

/******************************
******************************/
void MOV::update()
{
	if(video.isLoaded()){
		video.update();
		if(video.isFrameNew()){
			draw_to_fbo();
		}
	}
}

/******************************
******************************/
void MOV::publish_syphon(ofxSyphonServer& SyphonServer)
{
	ofTexture tex = fbo.getTextureReference();
	SyphonServer.publishTexture(&tex);
}

/******************************
******************************/
void MOV::draw_to_fbo()
{
	if(!video.isLoaded()) return;
	
	fbo.begin();
		/********************
		********************/
		// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ofClear(0, 0, 0, 0);
		
		/********************
		********************/
		ofColor BaseColor = Gui_Global->gui_MovColor;
		ofSetColor(BaseColor);
		// ofSetColor(255, 255, 255, 255);
		
		video.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();
}

/******************************
******************************/
void MOV::draw(float _x, float _y, float _w, float _h)
{
	ofSetColor(255, 255, 255, 255);
	fbo.draw(_x, _y, _w, _h);
}

