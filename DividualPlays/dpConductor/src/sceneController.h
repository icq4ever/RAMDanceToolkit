//
//  sceneController.h
//  dpConductor
//
//  Created by Ovis aries on 2016/01/26.
//
//

#ifndef sceneController_h
#define sceneController_h

#include "ofMain.h"
#include "baseOscSender.h"
#include "dpConductorConstants.h"
#include "ofxUI.h"

class sceneController : public baseOscSender{
public:
	
	void setup(string RDTK1_addr, string RDTK2_addr);
	
	string	getSceneName (int index);
	
	void	setScene_both(string name, bool viewA, bool viewB);
	void	setScene(string name, bool RDTK_isA, bool viewA, bool viewB);
	void	disableScene(string name, bool RDTK_isA);
	
	void	clearExtractor(string scene);
	void	setExtractor(string scene, string name, Joint node);
	void	loadExtractor(string scene);
	void	setFloatTune(string scene, string type, float value);
	void	setToggleTune(string scene, string type, bool value);
	void	setButtonTune(string scene, string type);
	
	void	clearScene();
	
	string addr_rdtk1, addr_rdtk2;
	
	string scene_rdtk1_a;
	string scene_rdtk1_b;
	string scene_rdtk2_a;
	string scene_rdtk2_b;
};

#endif /* sceneController_h */
