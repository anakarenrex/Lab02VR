#include "vrpn_Analog.h"
#include "vrpn_Button.h"
#include "vrpn_Tracker.h"

#include <iostream>
#include <cstdlib>
using namespace std;

//Macros
#define ACTIVATED 0
#define DESACTIVATED 1
#define AREA_A 2
#define AREA_B 3
#define AREA_C 4
#define AREA_D 5
#define BORDERLINE_AREA 6 
#define MIDDEL_X_AREA 0.5
#define MIDDEL_Y_AREA 0.5
#define MAX_X_AREA 0.5
#define MAX_Y_AREA 0.5
#define GAP_BORDER_LINE 0.02


//Global variables
double X_AXIS = 0.0;
double Y_AXIS = 0.0;
int currentArea = AREA_A;
int currentState = ACTIVATED;
int previousArea = AREA_A;

void showTheArea(int currentArea) {
	system("CLS");
	printf("\a");

	if (currentArea == AREA_A) {
		cout << "AREA : A";
	}
	else if (currentArea == AREA_B) {
		cout << "AREA : B";
	}
	else if (currentArea == AREA_C) {
		cout << "AREA : C";
	}
	else if (currentArea == AREA_D) {
		cout << "AREA : D";
	}
}

//Function for handling analog sensors (in this case, mouse moving)
void VRPN_CALLBACK handle_analog(void* userData, const vrpn_ANALOGCB a)
{

	// 1-Track the position
	X_AXIS = a.channel[0];
	Y_AXIS = a.channel[1];

	previousArea = currentArea;

	//2- interpretation of the position
	if (currentState == ACTIVATED) {

		if ((X_AXIS >= MIDDEL_X_AREA - GAP_BORDER_LINE && X_AXIS <= MIDDEL_X_AREA + GAP_BORDER_LINE) ||
			(Y_AXIS >= MIDDEL_Y_AREA - GAP_BORDER_LINE && Y_AXIS <= MIDDEL_Y_AREA + GAP_BORDER_LINE)) {
			currentArea = BORDERLINE_AREA;
		}
		if (X_AXIS < MIDDEL_X_AREA - GAP_BORDER_LINE && Y_AXIS < MIDDEL_Y_AREA - GAP_BORDER_LINE) {
			currentArea = AREA_A;
		}
		else if (X_AXIS > MIDDEL_X_AREA + GAP_BORDER_LINE && Y_AXIS < MIDDEL_Y_AREA - GAP_BORDER_LINE) {
			currentArea = AREA_B;
		}
		else if (X_AXIS < MIDDEL_X_AREA - GAP_BORDER_LINE && Y_AXIS >  MIDDEL_Y_AREA + GAP_BORDER_LINE) {
			currentArea = AREA_C;
		}
		else {
			currentArea = AREA_D;
		}

		//3- refresh the screen
		if (currentArea != previousArea && currentArea != BORDERLINE_AREA && currentState == ACTIVATED) {
			showTheArea(currentArea);
		}

	}
}

//Function for handling buttons pressed (1 is pressed, 0 is not pressed)
void VRPN_CALLBACK handle_button(void* userData, const vrpn_BUTTONCB b)
{

	if (b.button == 2 && b.state == 1 && currentState == ACTIVATED) {
		printf("\a");
	}
	else if (b.button == 0 && b.state == 1 && currentState == ACTIVATED) {
		system("CLS");
		currentState = DESACTIVATED;
		printf("You deactivated the functions, please click on the left button for activation\n");
	}
	else if (b.button == 0 && b.state == 1 && currentState == DESACTIVATED) {
		system("CLS");
		currentState = ACTIVATED;
		printf("You activated the functions \a \n");
	}
}

//Function for handling tracking sensors (in this case, kinect)
void VRPN_CALLBACK handle_tracker(void* userData, const vrpn_TRACKERCB t)
{
	cout << "Tracker '" << t.sensor << "' : " << t.pos[0] << "," << t.pos[1] << "," << t.pos[2] << endl;
}


int main(int argc, char* argv[])
{
	vrpn_Analog_Remote* vrpnAnalog = new vrpn_Analog_Remote("Mouse0@localhost");
	vrpn_Button_Remote* vrpnButton = new vrpn_Button_Remote("Mouse0@localhost");
	//vrpn_Tracker_Remote* vrpnTracker = new vrpn_Tracker_Remote("Tracker0@10.4.128.64");

	vrpnAnalog->register_change_handler(0, handle_analog);
	vrpnButton->register_change_handler(0, handle_button);
	//vrpnTracker->register_change_handler(0, handle_tracker);

	while (1)
	{
		vrpnAnalog->mainloop();
		vrpnButton->mainloop();
		//vrpnTracker->mainloop();
	}

	return 0;
}