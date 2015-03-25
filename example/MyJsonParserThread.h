//
//  MyJsonParserThread.h
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 25/03/15.
//
//

#ifndef BaseApp_MyJsonParserThread_h
#define BaseApp_MyJsonParserThread_h


#include "ofxMtJsonParser.h"
#include "ofxJSON.h"
#include "MyParseableObject.h"

class MyJsonParserThread : public ofxMtJsonParserThread<MyParseableObject>{

public:

	void parseJsonSubsetThread(){

		ofxJSONElement & jsonRef = *json; //pointers mess up the json syntax somehow

		vector<string>allKeys = jsonRef["data"].getMemberNames();

		//both included
		int start = config.startIndex;
		int end = config.endIndex;

		//only parse our subset of the JSON objects: [start .. end]
		for(int i = start; i <= end; i++){

			try{
				string key = allKeys[i];
				Json::Value &thisObject = jsonRef["data"][key];

				//make new object
				MyParseableObject * o = new MyParseableObject();

				//parse stuff into it!
				o->setTitle(thisObject["title"].asString());
				o->setDescription(thisObject["description"].asString());

				////////////////////////////////////////////////////////////////////
				// THIS IS KEY! store the new parsed object in the superclass array
				// and update progress!/////////////////////////////////////////////
				parsedObjects.push_back(o);
				numParsedObjects = i - start;
				////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////

			} catch (Exception exc) {
				printMutex->lock();
				ofLogError("MyJsonParserThread") << exc.what() << " " << exc.message() << " " << exc.displayText() << " WHILE PARSING " << i;
				printMutex->unlock();
			}
		}
	}

	int getNumEntriesInJson(ofxJSONElement* json_){

		ofxJSONElement & jsonRef = *json_; //pointers mess up the json syntax somehow

		if(jsonRef.isObject()){
			if(jsonRef["data"].isObject()){
				return jsonRef["data"].size();
			}
		}
		ofLogError("MyJsonParserThread") << "JSON has unexpected format";
		return 0;
	}

private:


};

#endif
