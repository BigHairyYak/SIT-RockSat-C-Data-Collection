#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <WiringPi.h>		//Requires WiringPi library to be installed
#include <time.h>

using namespace std;

//c++ -pthread -lwiringpi record.cpp record

void dev (string devno);
void countDownOver(int source);
void gSwitchTriggered();
void log(string message);

#define PIN 2				//Pin to be used for G-switch
bool start = false;

time_t = now;				//To be refreshed every log update

int main(){
		wiringPiSetup();									//Sets up GPIO pins for G-switch	
		pinMode(PIN, INPUT);								//PIN waits for G-switch	
		wiringPiISR(PIN, INT_EDGE_BOTH, &gSwitchTriggered);	//Establish interrupt for G-switch
		
		freopen ("TestLog.log", "w", stdout);				//Redirect output to TestLog.log
		
		log("Initializing Log");
		
		clock_t t = clock();	
		log ("BEGINNING COUNTDOWN");			//THREE-MINUTE COUNTDOWN
		
		signal (SIGALRM, countDownOver);		//Trigger interrupt when countdown ends
		alarm(180);								//Start 180-second (3-minute) countdown
		
		while (!start); 						//Loop to waits
		
		log ("The countdown ran for " + to_string((float) (clock() - t)/CLOCKS_PER_SEC) + " seconds");
		
		t = clock();			//Reset clock to 0 around LINUX program start
		
		thread rec1(dev, "0");	//Start 4 threads, each running:
		thread rec2(dev, "1");	//arecord -D plughw:[devno],0 sound[devno].wav
		thread rec3(dev, "2");
		thread rec4(dev, "3");
		
		//THREADS NEED TO BE SYNCHRONIZED
		
		rec1.join();	rec2.join();	rec3.join();	rec4.join();		//THIS IS NOT SYNCHRONIZED
		
		fclose(stdout);		//Close log
}

void dev(string devno){
	log("attempting to record on device + "devno);
	string toRecord = "arecord -r44100 -D plughw:" + devno + ",0"		//44kHz and get device
					  + "sound" + devno + ".wav";						//Save sound file as wav
					  
	system(toRecord.c_str());
}

void gSwitchTriggered(){			//Interrupt for when G-switch is triggered
	start = true;					//Begin recording
	alarm(0);						//Cancel any existing alarms
}

void countDownOver(int source){		//Interrupt to be called by countdown ending
	if (source == SIGALRM)			
		start = true;				//Begin recording, no need to cancel alarm
)

void log(string message){
	now = time(0);
	string timeS = ctime(&now);
	timeS.erase(timeS.find('\n',0),1);
	cout << timeS << " - " << message << endl;
}