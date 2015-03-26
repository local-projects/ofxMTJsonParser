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
#include "MyParsingArgs.h"

class MyJsonParserThread : public ofxMtJsonParserThread<MyParseableObject>{

public:

	void parseJsonSubsetThread(){

		const ofxJSONElement & jsonRef = *json; //pointers mess up the json syntax somehow


		//both included
		int start = config.startIndex;
		int end = config.endIndex;

		//force cast to real config we supplied in ofApp
		MyParsingArgs * myArgs = (MyParsingArgs *)args;

		//only parse our subset of the JSON objects: [start .. end]
		for(int i = start; i <= end; i++){

			try{
				if (myArgs->verbose){
					printMutex->lock();
					ofLogNotice("MyJsonParserThread") << "Thread " << config.threadID <<" parsing OBJ #" << i;
					printMutex->unlock();
				}


				//make new object
				MyParseableObject * o = new MyParseableObject();

				//parse stuff into it!
				o->setTitle( ofxMtJsonParserUtils::initFromJsonString(jsonRef[i], "title_raw", true, printMutex) );
				o->setDescription( ofxMtJsonParserUtils::initFromJsonString(jsonRef[i], "dimensions", true, printMutex) );

				////////////////////////////////////////////////////////////////////
				// THIS IS KEY! store the new parsed object in the superclass array
				// and update numParsedObjects! ////////////////////////////////////

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

		const ofxJSONElement & jsonRef = *json_; //pointers mess up the json syntax somehow

		//CH JSON
		if(jsonRef.isArray()){
			return jsonRef.size();
		}


		//CWRU JSON
//		if(jsonRef.isObject()){
//			if(jsonRef["data"].isObject()){
//				int numObjects = jsonRef["data"].size();
//				//printf("numObjects: %d\n", numObjects);
//				return numObjects;
//			}
//		}


		ofLogError("MyJsonParserThread") << "JSON has unexpected format";
		return 0;
	}

private:


};

#endif
