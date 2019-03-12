/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "VideoCam.h"
#include "SourceCode.h"
#include "mov.h"


/************************************************************
************************************************************/

/**************************************************
**************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		DISP_CAM_WIDTH	= 640,
		DISP_CAM_HEIGHT	= 360,
	};

	/****************************************
	****************************************/
	SJ_VIDEO_CAM VideoCam;
	int Cam_id;
	
	SOURCE_CODE* VisualizedSourceCode;
	
	MOV mov;
	string FileName;
	const bool MovSound_on;
	bool b_mov_paused;
	float Last_pos_mov;
	
	bool b_DispGui;
	bool b_ShowFrameRate;
	
	ofTrueTypeFont font;
	
	ofxSyphonServer SyphonServer_CamOrMov;
	
	/****************************************
	****************************************/
	void setup_Gui();
	
public:
	/****************************************
	****************************************/
	ofApp(int _Cam_id,  string _FileName, bool _MovSound_on);
	~ofApp();
	
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
