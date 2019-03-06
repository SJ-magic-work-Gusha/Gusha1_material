/************************************************************
************************************************************/
#include "ofApp.h"


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _Cam_id, bool _MovSound_on)
: Cam_id(_Cam_id)
, MovSound_on(_MovSound_on)
, b_DispGui(true)
, b_ShowFrameRate(false)
, VisualizedSourceCode(NULL)
, b_mov_paused(false)
{
	VisualizedSourceCode = new SOURCE_CODE;
}

/******************************
******************************/
ofApp::~ofApp()
{
	if(VisualizedSourceCode) delete VisualizedSourceCode;
	delete Gui_Global;
}

/******************************
******************************/
void ofApp::exit()
{
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	font.load("font/RictyDiminishedDiscord-Bold.ttf", 15, true, true, true);
	
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("Gusha:material");
	ofSetVerticalSync(true);
	ofSetFrameRate(20);
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	/********************
	********************/
	setup_Gui();
	
	SyphonServer_CamOrMov.setName("Gusha_material_CamOrMov");
	
	/********************
	********************/
	VideoCam.setup(Cam_id);
	
	VisualizedSourceCode->setup();
	
	mov.setup(MovSound_on);
	Last_pos_mov = Gui_Global->pos_mov;
}

/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("Gusha:material", "gui.xml", 10, 10);
}

/******************************
******************************/
void ofApp::update(){
	float now = ofGetElapsedTimef();
	
	VideoCam.update();
	
	if(VisualizedSourceCode) VisualizedSourceCode->update(now);
	
	if(Last_pos_mov != Gui_Global->pos_mov) { mov.Seek(Gui_Global->pos_mov); }
	Last_pos_mov = Gui_Global->pos_mov;
	mov.update();
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ofClear(0, 0, 0, 0);
	ofSetColor(255);
	
	/********************
	********************/
	if(Gui_Global->Disp_SourceCode){
		if(VisualizedSourceCode) VisualizedSourceCode->draw();
	}else{
		VideoCam.draw(0, 0, DISP_CAM_WIDTH, DISP_CAM_HEIGHT);
		mov.draw(DISP_CAM_WIDTH, 0, DISP_CAM_WIDTH, DISP_CAM_HEIGHT);
	}
	
	/********************
	********************/
	if(VisualizedSourceCode) VisualizedSourceCode->publish_syphon();
	
	ofSetColor(255, 255, 0);
	ofSetLineWidth(2);
	
	if(Gui_Global->b_mov_to_syphon){
		mov.publish_syphon(SyphonServer_CamOrMov);
		ofDrawLine(800, 400, 1120, 400);
		
	}else{
		VideoCam.publish_syphon(SyphonServer_CamOrMov);
		ofDrawLine(160, 400, 480, 400);
	}
	
	/********************
	********************/
	if(b_DispGui) Gui_Global->gui.draw();
	
	/********************
	********************/
	if(b_ShowFrameRate){
		char buf[BUF_SIZE_S];
		sprintf(buf, "%7.2f", ofGetFrameRate());
		ofSetColor(255, 255, 0, 100);
		font.drawString(buf, ofGetWidth() - font.stringWidth("xxxxxxxxxx"), ofGetHeight() - font.stringHeight("xxxxxxxxxx"));
	}
	
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		/********************
		timing次第で、VisualizedSourceCode で、fbo描画が上手くいかない現象が発生。
		temporary対応として、Reset(deleteして新たにnew)を入れた。
		
		SOURCE_CODE::t_LastIntを初期化していないことに起因することが判明.
		Resetは不要なので、コメントアウトしておく。
		********************/
		/*
		case 'r':
			if(VisualizedSourceCode){
				delete VisualizedSourceCode;
				VisualizedSourceCode = new SOURCE_CODE;
				VisualizedSourceCode->setup();
			}
			break;
		*/
			
		case 'f':
			b_ShowFrameRate = !b_ShowFrameRate;
			break;
			
		case 't':
			mov.SeekToTop();
			break;
			
		case 'p':
			b_mov_paused = !b_mov_paused;
			mov.setPaused(b_mov_paused);
			break;
			
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
