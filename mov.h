/************************************************************
************************************************************/
#pragma once

/********************
 ofxHapPlayerを使うと、時間と共にCPU占有率が高くなってしまう現象が見られた。
色々とtestしたみたが、解決せず。
ofVideoPlayer で問題なく動作するので、こちらで対応。
********************/
// #define HAP

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxSyphon.h"
// #include "ofxHapPlayer.h"

#include "sj_common.h"

/************************************************************
************************************************************/
class MOV : Noncopyable{
private:
	/****************************************
	****************************************/
	enum{
		/*
		VIDEO_WIDTH = 640,
		VIDEO_HEIGHT = 360,
		*/
		
		VIDEO_WIDTH = 1280,
		VIDEO_HEIGHT = 720,
	};
	
	/****************************************
	****************************************/
	ofFbo fbo;
	
#ifdef HAP
	ofxHapPlayer video;
#else
	ofVideoPlayer video;
#endif
	
	/****************************************
	****************************************/
	void Clear_fbo(ofFbo& fbo);
	void draw_to_fbo();
	
#ifdef HAP
	void setup_video(ofxHapPlayer& video, bool MovSound_on);
#else
	void setup_video(ofVideoPlayer& video, bool MovSound_on);
#endif
	
public:
	/****************************************
	****************************************/
	MOV();
	~MOV();
	
	void setup(bool MovSound_on);
	void update();
	void draw(float _x, float _y, float _w, float _h);
	void publish_syphon(ofxSyphonServer& SyphonServer);
	void SeekToTop();
	void Seek(float pos);
	void setPaused(bool b_pause);
};

