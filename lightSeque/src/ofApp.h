#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxRelayLoopMgr.h"
#include "ofxUDPManager.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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

		void showGui();

		ofxRelayLoopMgr relayMgr;

		ofxImGui::Gui gui;
		ImGuiWindowFlags window_flags = 0;


		enum LightState
		{
			state_all_on,
			state_all_off,
			state_seque_loop,
			state_wait_next,
		}lightState;
		void switchState(LightState _state);

		int index;
		ofxUDPManager listener;
		int port;
		string allOnCmd;
		string allOffCmd;
		string sequeLoopCmd;
		float gapTimeLen;

		float timer;

		void nextLight();
};
