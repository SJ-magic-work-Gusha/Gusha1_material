/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <time.h>

#include "ofMain.h"
#include "ofxSyphon.h"

#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class SOURCE_CODE : Noncopyable{
private:
	/****************************************
	****************************************/
	enum{
		NUM_CONTENTS = 2,
		NUM_DISPLINES_CONTENTS = 50,
	};
	
	enum{
		SOURCE_WIDTH = 1000,
		SOURCE_HEIGHT = 1000,
	};
	
	
	/****************************************
	****************************************/
	class CHAIN_CONTENTS : Noncopyable{
	private:
		
	public:
		string str_line;
		ofColor TextColor;
		CHAIN_CONTENTS* Next;
		
		CHAIN_CONTENTS(string& s)
		: Next(NULL)
		, str_line(s)
		{
			// TextColor = ofColor(255, 255, 255, 255);
			TextColor = ofColor(100);
		}
		
		void draw(ofTrueTypeFont& font)
		{
			ofSetColor(TextColor);
			font.drawString(str_line.c_str(), 0, 0);
		}
	};
	
	/****************************************
	****************************************/
	/********************
	********************/
	int c_syphon;
	
	ofTrueTypeFont font;
	
	ofFbo fbo;
	ofxSyphonServer fbo_SyphonServer;
	
	FILE* fp_Contents[NUM_CONTENTS];
	
	double N_Lines_PerSec[NUM_CONTENTS];
	double NumLines_Proceed[NUM_CONTENTS];
	
	float now;
	float t_LastInt;
	
	CHAIN_CONTENTS* TopOfContents[NUM_CONTENTS];
	
	/********************
	********************/
	float CursorPos[NUM_CONTENTS];
	float CursorSpeed[NUM_CONTENTS];
	
	
	/****************************************
	****************************************/
	void TabToSpacex4(string& s);
	void YennToCR(string& s);
	string CR_To_Yenn(string s);

	int Count_NumContent_inChain(CHAIN_CONTENTS* top);
	void Add_ContentsChain(int ContentsId, string& s);
	void Align_StringOfData(string& s);
	void Read_1Line_FromFile(int ContentsId, string& str_Line);
	void update_ContentsChain(int ContentsId, string& s);
	void Contents_LineFeed(int Contents_id, double dt);
	double get_FontHeight();
	void Clear_fbo();
	void draw_to_fbo();
	
public:
	/****************************************
	****************************************/
	SOURCE_CODE();
	~SOURCE_CODE();
	
	void setup();
	void update(float _now);
	void draw();
	void publish_syphon();
};
