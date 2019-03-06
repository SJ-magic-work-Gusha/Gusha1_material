/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

FILE* fp_Log = NULL;


/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/* */
	gui.add(Disp_SourceCode.setup("Disp_SourceCode", false));
	
	gui.add(b_mov_to_syphon.setup("syphon:mov(or cam)", false));
	gui.add(pos_mov.setup("pos_mov", 0, 0, 1.0));
	
	{
		ofColor initColor = ofColor(255, 255, 255, 255);
		ofColor minColor = ofColor(0, 0, 0, 0);
		ofColor maxColor = ofColor(255, 255, 255, 255);
		
		gui.add(gui_VideoColor.setup("VideoColor", initColor, minColor, maxColor));
		gui.add(gui_MovColor.setup("MovColor", initColor, minColor, maxColor));
	}

	/********************
	********************/
	gui.minimizeAll();
}

