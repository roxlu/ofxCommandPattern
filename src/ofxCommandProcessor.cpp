#include "ofxCommandProcessor.h"
#include "ofxCommand.h"
#include "ofMain.h"
#include "ofxLog.h"

ofxCommandProcessor::ofxCommandProcessor()
{
}

ofxCommandProcessor::~ofxCommandProcessor() {
	std::cout << ">>>> ~ofxCommandProcessor" << std::endl;
}

void ofxCommandProcessor::enqueue(ofxCommand* pCommand) {
    OFXLOG("ofxCommandProcessor::enqueue() command:" << pCommand->name);
	queue.push_back(pCommand);
}

void ofxCommandProcessor::clear() {
	std::deque<ofxCommand*>::iterator it = queue.begin();
	while (it != queue.end()) {
		delete (*it);
		++it;
	}
	queue.clear();
}

void ofxCommandProcessor::remove(std::string sName) {
    OFXLOG("ofxCommandProcessor::remove() command:" << sName);
	std::deque<ofxCommand*>::iterator it =  queue.begin();
	while(it != queue.end()) {
		//std::cout << ">> In queue: " << (*it++)->name << std::endl;
		if( (*it)->name == sName) {
		    OFXLOG("ofxCommandProcessor::remove() - delete pointer:" << sName);
			delete (*it);
			it = queue.erase(it);
		}
		else
			++it;

	}
}

bool ofxCommandProcessor::isReady() {
	bool ready = queue.empty();
	std::deque<ofxCommand*>::iterator it =  queue.begin();


	#ifdef LOG_OFXCOMMANDPATTERN
	while(it != queue.end()) {
		std::cout << ">> In queue: " << (*it++)->name << std::endl;
	}
	#endif
	return ready;
}


ofxCommand* ofxCommandProcessor::take() {
	if(queue.empty())
		return NULL;
	ofxCommand* command = queue.front();
	queue.pop_front();
	return command;
}

void ofxCommandProcessor::update() {
	ofxCommand* command = take();
	if(command != NULL) {
		bool complete = command->execute();
		if(!complete) {
			queue.push_front(command); // we want to repeat it directly! (not at the end)
		}
		else {
			// @todo something wrong here! we need to use a boost::ptr_deque
            OFXLOG("ofxCommandProcessor::update() delete:" << command->name);
			delete command;
		}
	}
}



