#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char** argv ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context
	
	/********************
	********************/
	int Cam_id = -1;
	bool MovSound_on = false;
	string FileName = "NotSet";
	
	/********************
	********************/
	printf("> parameters\n");
	printf("\t-c:Cam id(-1)\n");
	printf("\t-v:Mov sound on : 0=off,1=on (0)\n");
	printf("\t-f:movie file name\n");
	
	for(int i = 1; i < argc; i++){
		if( strcmp(argv[i], "-c") == 0 ){
			if(i+1 < argc){
				Cam_id = atoi(argv[i+1]);
				if(Cam_id < 0) Cam_id = 0;
			}
		}else if( strcmp(argv[i], "-v") == 0 ){
			if(i+1 < argc){
				if(atoi(argv[i+1]) == 1) MovSound_on = true;
			}
		}else if( strcmp(argv[i], "-f") == 0 ){
			if(i+1 < argc){
				FileName = argv[i+1];
			}
		}
	}
	
	ofRunApp(new ofApp(Cam_id, FileName, MovSound_on));
}
