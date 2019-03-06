/************************************************************
************************************************************/
#include "SourceCode.h"

/************************************************************
************************************************************/

/******************************
******************************/
SOURCE_CODE::SOURCE_CODE()
: c_syphon(0)
, now(0)
, t_LastInt(0)
{
	/********************
	********************/
	srand((unsigned) time(NULL));
	
	/********************
	********************/
	for(int i = 0; i < NUM_CONTENTS; i++)	fp_Contents[i] = NULL;
	
	fp_Contents[0] = fopen("../../../data/sample.cpp", "r");
	fp_Contents[1] = fopen("../../../data/sample.asm", "r");

	for(int i = 0; i < NUM_CONTENTS; i++){
		if(fp_Contents[i] == NULL) { ERROR_MSG(); std::exit(1); }
	}
	
	/********************
	********************/
	N_Lines_PerSec[0] = 1.1;
	// N_Lines_PerSec[1] = 8.2;
	N_Lines_PerSec[1] = 6.2;
	
	for(int i = 0; i < NUM_CONTENTS; i++) { NumLines_Proceed[i] = 0; }
	
	/********************
	********************/
	CursorPos[0] = 0;
	CursorPos[1] = 30;
	
	CursorSpeed[0] = 50;
	CursorSpeed[1] = 35;

	
	/********************
	********************/
	for(int i = 0; i < NUM_CONTENTS; i++)	TopOfContents[i] = NULL;
	
	for(int i = 0; i < NUM_CONTENTS; i++){
		for(int j = 0; j < NUM_DISPLINES_CONTENTS; j++){
			string str_Line;
			
			Read_1Line_FromFile(i, str_Line);
			Add_ContentsChain(i, str_Line);
		}
	}
}

/******************************
******************************/
SOURCE_CODE::~SOURCE_CODE()
{
	/********************
	********************/
	for(int i = 0; i < NUM_CONTENTS; i++){
		if(fp_Contents[i]) fclose(fp_Contents[i]);
	}
	
	/********************
	********************/
	for(int i = 0; i < NUM_CONTENTS; i++){
		CHAIN_CONTENTS* temp = TopOfContents[i];
		CHAIN_CONTENTS* temp_to_del;
		
		while(temp){
			temp_to_del = temp;
			temp = temp->Next;
			delete temp_to_del;
		}
	}
}

/******************************
******************************/
void SOURCE_CODE::Add_ContentsChain(int ContentsId, string& s)
{
	CHAIN_CONTENTS** temp = &TopOfContents[ContentsId];
	while(*temp){
		temp = &((*temp)->Next);
	}
	(*temp) = new CHAIN_CONTENTS(s);
}

/******************************
******************************/
int SOURCE_CODE::Count_NumContent_inChain(CHAIN_CONTENTS* top)
{
	int counter = 0;
	CHAIN_CONTENTS** temp = &top;
	while(*temp){
		temp = &((*temp)->Next);
		counter++;
	}
	return counter;
}

/******************************
******************************/
void SOURCE_CODE::update_ContentsChain(int ContentsId, string& s)
{
	/********************
	********************/
	if(Count_NumContent_inChain(TopOfContents[ContentsId]) < 2) return;
	
	/********************
	********************/
	CHAIN_CONTENTS* temp = TopOfContents[ContentsId];
	TopOfContents[ContentsId] = temp->Next;
	temp->Next = NULL;
	temp->str_line = s;
	
	CHAIN_CONTENTS** Last = &TopOfContents[ContentsId];
	while(*Last){
		Last = &((*Last)->Next);
	}
	(*Last) = temp;
}

/******************************
******************************/
void SOURCE_CODE::Align_StringOfData(string& s)
{
	size_t pos;
	// while((pos = s.find_first_of(" 　\t\n\r")) != string::npos){ // 半角・全角space, \t 改行 削除
	while((pos = s.find_first_of(" 　\n\r")) != string::npos){ // 半角・全角space, \t 改行 削除
		s.erase(pos, 1);
	}
}

/******************************
description
	font.drawStringAsShapes(str_line.c_str(), 0, 0);
	では、str_line.c_str()に"\t"が入っていても無視される.
	そこで、space x 4にreplaceしておく.
******************************/
void SOURCE_CODE::TabToSpacex4(string& s)
{
	string Tab = "\t";
	string Space   = "    ";
	
	// size_t pos = s.find_first_of(YenN, 0); // NG. don't use find_first_of here.
	size_t pos = s.find(Tab, 0);

	while(pos != string::npos){
		s.replace(pos, Tab.length(), Space);
		pos = s.find(Tab, pos);
	}
}

/******************************
description
	外部fileから文字列を読み込んだ時、
	string文字列中の"\n"は実は"\\n"(\\ + n の２文字)、改行文字のリテラルではない.
	本methodは、これを変換するためのmethod.
	
参考
	http://d.hatena.ne.jp/p95095yy/20050222/1109119001
	注意)
		本URLでは、find_first_of を使っているが、間違い.
		find_first_of()は、「列挙された1文字ずつを」探しに行く.
		正解は、「文字列」を探しに行く find().
******************************/
void SOURCE_CODE::YennToCR(string& s)
{
	string YenN = "\\n";
	string CR   = "\n";
	
	// size_t pos = s.find_first_of(YenN, 0); // NG. don't use find_first_of here.
	size_t pos = s.find(YenN, 0);

	while(pos != string::npos){
		s.replace(pos, YenN.length(), CR);
		pos = s.find(YenN, pos);
	}
}

/******************************
description
	外部fileに戻す時は、
	"\n" -> "\\n"
******************************/
string SOURCE_CODE::CR_To_Yenn(string s)
{
	string YenN = "\\n";
	string CR   = "\n";
	
	size_t pos = s.find(CR, 0);

	while(pos != string::npos){
		s.replace(pos, CR.length(), YenN);
		pos = s.find(CR, pos);
	}
	
	return s;
}

/******************************
******************************/
void SOURCE_CODE::Read_1Line_FromFile(int ContentsId, string& str_Line)
{
	/********************
	********************/
	char buf[BUF_SIZE_S];
	
	if(fgets(buf, BUF_SIZE_S, fp_Contents[ContentsId]) == NULL){
		fseek(fp_Contents[ContentsId], 0, SEEK_SET);
		
		if(fgets(buf, BUF_SIZE_S, fp_Contents[ContentsId]) == NULL){
			printf("No Contents in fp_Contents[%d]\n", ContentsId);
			std::exit(1);
		}else{
			/*
			printf("File_%d Loop\n", ContentsId);
			fflush(stdout);
			*/
		}
	}
	
	/********************
	********************/
	str_Line = buf;
	TabToSpacex4(str_Line);
	// Align_StringOfData(str_Line);
}

/******************************
******************************/
void SOURCE_CODE::setup()
{
	/********************
	********************/
	font.load("font/RictyDiminishedDiscord-Bold.ttf", 11, true, true, true);
	// font.load("font/RictyDiminished-Regular.ttf", 12, true, true, true);
	
	/********************
	********************/
	fbo.allocate(SOURCE_WIDTH, SOURCE_HEIGHT, GL_RGBA, 0);
	Clear_fbo();
	
	fbo_SyphonServer.setName("Gusha_material_SourceCode");
	
	/********************
	********************/
	t_LastInt = ofGetElapsedTimef();
}

/******************************
******************************/
void SOURCE_CODE::Clear_fbo()
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
void SOURCE_CODE::update(float _now)
{
	/********************
	********************/
	now = _now;
	
	// if(Gui_Global->b_SourceCode){
	if(1){
		/********************
		********************/
		Contents_LineFeed(0, now - t_LastInt);
		Contents_LineFeed(1, now - t_LastInt);
		
		for(int i = 0; i < NUM_CONTENTS; i++){
			CursorPos[i] += CursorSpeed[i] * (now - t_LastInt);
			if(fbo.getHeight() + 30 < CursorPos[i]) CursorPos[i] = 0;
		}
		
		/********************
		********************/
		draw_to_fbo();
	}else{
		Clear_fbo();
	}
	
	/********************
	********************/
	t_LastInt = now;
}

/******************************
******************************/
void SOURCE_CODE::Contents_LineFeed(int Contents_id, double dt)
{
	/********************
	********************/
	NumLines_Proceed[Contents_id] += N_Lines_PerSec[Contents_id] * dt;
	int int_NumLines_Proceed = int(NumLines_Proceed[Contents_id]);
	NumLines_Proceed[Contents_id] = NumLines_Proceed[Contents_id] - int_NumLines_Proceed;
	
	for(int i = 0; i < int_NumLines_Proceed; i++){
		string str_Line;
		Read_1Line_FromFile(Contents_id, str_Line);
		
		update_ContentsChain(Contents_id, str_Line);
	}
}

/******************************
******************************/
void SOURCE_CODE::draw()
{
	ofSetColor(255, 255, 255, 255);
	fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
}

/******************************
******************************/
void SOURCE_CODE::publish_syphon()
{
	/*
	c_syphon++;
	if(c_syphon % 2 == 0){
		c_syphon = 0;
		return;
	}
	*/
	
	ofTexture tex = fbo.getTextureReference();
	fbo_SyphonServer.publishTexture(&tex);
}

/******************************
******************************/
double SOURCE_CODE::get_FontHeight()
{
	return font.stringHeight("0123456789abcdefghijklmnopqrstuvwxyz") * 1.5;
}

/******************************
******************************/
void SOURCE_CODE::draw_to_fbo()
{
	fbo.begin();
		/********************
		********************/
		// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ofClear(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		
		/********************
		********************/
		ofDisableAlphaBlending();
		ofEnableSmoothing();
		
		/********************
		********************/
		float stringHeight = get_FontHeight();;
		
		/********************
		contentd[0]
		********************/
		ofPushMatrix();
		{
			ofSetColor(255, 0, 0, 100);
			ofDrawRectangle(0, CursorPos[0], fbo.getWidth()*2/3, get_FontHeight()/2);
			
			double WrittenHeight = 0;
			CHAIN_CONTENTS* temp = TopOfContents[0];
			
			while(temp){
				ofTranslate(0, stringHeight);
				temp->draw(font);
				temp = temp->Next;
				
				WrittenHeight += stringHeight;
				if(fbo.getHeight() < WrittenHeight) break;
			}
		}
		ofPopMatrix();
		
		/********************
		contentd[1]
		********************/
		ofPushMatrix();
		{
			ofTranslate(fbo.getWidth(), fbo.getHeight());
			ofScale(-1, -1, 0);
			
			ofSetColor(0, 100, 255, 100);
			ofDrawRectangle(0, CursorPos[1], fbo.getWidth()*2/3, get_FontHeight()/2);
			
			double WrittenHeight = 0;
			CHAIN_CONTENTS* temp = TopOfContents[1];
			
			while(temp){
				ofTranslate(0, stringHeight);
				temp->draw(font);
				temp = temp->Next;
				
				WrittenHeight += stringHeight;
				if(fbo.getHeight() < WrittenHeight) break;
			}
		}
		ofPopMatrix();
	fbo.end();
}
