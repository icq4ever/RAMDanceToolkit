#pragma once

#include "ramBaseScene.h"
#include "ControlSegment.h"
#include "PlaybackSegment.h"

/*
 almost all things of this class depends on ofxUI
*/
class ramActorsScene : public ramBaseScene
{
public:
	
	ramActorsScene();
	~ramActorsScene();
	
	string getName() const;
	void setupControlPanel();
	void setup();
	void update();
	void draw();
	void drawHUD();
	
	void onActorSetup(const ramActor &actor);
	void onRigidSetup(const ramRigidBody &rigid);
	void onActorExit(const ramActor &actor);
	void onRigidExit(const ramRigidBody &rigid);
	
	void onKeyPressed(ofKeyEventArgs &e);
	void onValueChanged(ofxUIEventArgs &e);
	void onFileDrop(ofDragInfo &e);
	
	void showAll(bool showAll);
	void resetPosAll(bool showAll);
    void pauseAll(bool bPause);
    void recAll(bool bStartRec);
	bool getShowAll();
    void loadFile(const string filePath);
    
	void setNeedsUpdatePanel(const bool needsUpdate);
    bool needsUpdatePanel();
    
	void drawNodes(const ramNodeArray &NA); // experimental
    
private:
    
    /// playback
	ramTSVCoder coder;
    
	
	/// internal use
    void addSegment(BaseSegment *newSegment);
	void removeControlSegment(const string name);
	void rebuildControlPanel();
	void createPanelHeader();
    
    
	/// instances which are controlled programatically
	ofxUILabelToggle *btnShowAll;
	ofxUILabelToggle *btnPause;
	ofxUILabelToggle *btnRecAll;

	
	/// ActorsPanel
	ofxUICanvasPlus *mLocalPanel;
	
	
	/// ControlSegment map
	map<string, BaseSegment*> mSegmentsMap;
	typedef map<string, BaseSegment*>::iterator SegmentsIter;
	
	
	/// fonts
	ofTrueTypeFont font;
	float fontSize;
	
	
	/// update flags
	bool bNeedUpdatePanel;
	bool bRecording;
	
	
	/// draw method flags
	ofLight light;
	bool bShowAllActor;
	bool bRecAllActor;
	bool bUseSimpleActor;
	bool bUseShading;
};
