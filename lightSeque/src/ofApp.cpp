#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	relayMgr.setup();

	gui.setup();

	{
		lightState = state_all_off;
		index = 0;
		switchState(state_seque_loop);
	}

	{
		ofxXmlSettings xml;
		xml.load("config.xml");

		port = xml.getValue("PORT", 19999);

		allOnCmd = xml.getValue("CMD_ALL_ON", "seque_light_all_on");
		allOffCmd = xml.getValue("CMD_ALL_OFF", "seque_light_all_off");
		sequeLoopCmd = xml.getValue("CMD_ALL_LOOP", "seque_light_all_loop");

		gapTimeLen = xml.getValue("GAP_TIME_LEN", 5.0f);

		timer = 0.0f;


		listener.Create();
		listener.SetNonBlocking(true);
		listener.Bind(port);
	}

	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	switch (lightState)
	{
	case ofApp::state_all_on:
		break;
	case ofApp::state_all_off:
		break;
	case ofApp::state_seque_loop:
	{
		if (ofGetElapsedTimef() - timer > gapTimeLen)
		{
			nextLight();
		}
	}
		break;
	case ofApp::state_wait_next:
	{
		if (ofGetElapsedTimef() - timer > gapTimeLen)
		{
			switchState(state_seque_loop);
		}
	}
		break;
	default:
		break;
	}

	char data[100] = "";
	listener.Receive(data, 100);
	string msg = data;
	if ("" != msg)
	{
		if (msg == allOnCmd)
		{
			switchState(state_all_on);
		}
		else if (msg == allOffCmd)
		{
			switchState(state_all_off);
		}
		else if (msg == sequeLoopCmd)
		{
			switchState(state_seque_loop);
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	gui.begin();

	static bool no_titlebar = false;
	static bool no_border = true;
	static bool no_resize = true;
	static bool no_move = true;
	static bool no_scrollbar = false;
	static bool no_collapse = false;
	static bool no_menu = false;

	// Demonstrate the various window flags. Typically you would just use the default.
	if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
	if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Always);

	showGui();
	gui.end();


	for (int i = 0;i < relayMgr.size(); i++)
	{
		bool state = relayMgr.isOn(i);

		ofPushStyle();
		if (state)
		{
			ofSetColor(0, 255, 0);
		}
		else
		{
			ofSetColor(128, 128, 128);
		}
		ofCircle(500 + i * 20, 500,5);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

void ofApp::showGui()
{
	static bool open = true;
	ImGui::Begin("ImGui Demo", &open, window_flags);

	for (int i = 0;i < relayMgr.size(); i++)
	{
		ImGui::PushID(i);

		ImGui::Text("id:%d", i); ImGui::SameLine();

		if (ImGui::Button("ON", ImVec2(100, 25)))
		{
			relayMgr.setOn(i);
		}
		ImGui::SameLine(200);
		if (ImGui::Button("OFF", ImVec2(100, 25)))
		{
			relayMgr.setOff(i);
		}
		ImGui::PopID();

		ImGui::Separator();
	}
	if (ImGui::Button("ALL ON", ImVec2(100, 25)))
	{
		switchState(state_all_on);
	}
	if (ImGui::Button("ALL OFF", ImVec2(100, 25)))
	{
		switchState(state_all_off);
	}
	if (ImGui::Button("LOOP", ImVec2(100, 25)))
	{
		switchState(state_seque_loop);
	}

	ImGui::End();
}

void ofApp::switchState(LightState _state)
{
	if (_state == lightState)return;
	
	switch (_state)
	{
	case ofApp::state_all_on:
	{
		lightState = state_all_on;
		relayMgr.setAllOn();
	}
		break;
	case ofApp::state_all_off:
	{
		lightState = state_all_off;
		relayMgr.setAllOff();
	}
		break;
	case ofApp::state_seque_loop:
	{
		lightState = state_seque_loop;
		relayMgr.setAllOff();
		index = 0;
		timer = ofGetElapsedTimef();
		
	}
		break;
	case ofApp::state_wait_next:
	{
		lightState = state_wait_next;
		timer = ofGetElapsedTimef();
	}
	break;
	default:
		break;
	}
}

void ofApp::nextLight()
{
	if (index == relayMgr.size())
	{
		switchState(state_wait_next);
	}


	relayMgr.setOn(index);

	if (index == relayMgr.size()-1)
	{
		for (int i = relayMgr.size()-2; i >= 0; i--)
		{
			relayMgr.setOff(i);
		}
	}

	timer = ofGetElapsedTimef();
	index++;
}