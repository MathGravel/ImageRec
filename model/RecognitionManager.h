/**
 * \file      RecognitionManager.h
 * \author    Alexandre Gonzalvez
 * \version   2.0
 * \date      8 August 2019
 * \brief     Manage all project
 * \details   Function launching the entire project
 * 				Define Paramater => CONSTANT (In .cpp) //TODO : put in setting if constant are pertinent
 * 				Initialize Controller
 * 				Initialize Activities recognition, Plan recognition
 * 				Initialize view
 * 				Start main loop of recognition which update every composant of the program 
*/
#pragma once

#include <iostream> 
#include <vector>

/// To count time
#include <chrono> 

///To observe camera
#include "../controller/ManageSourceVideo.h"
#include "Observer.h"
#include <opencv2/opencv.hpp> //To read Matrix of picture

///To update view (interface)
#include "../view/PrimaryWindow.h"

///To update recognition Image recognition
#include "recoActivite/recoImage/ActivityRegion.h"

///To update recognition activities
#include "recoActivite/recoAffordance/ObjectAff.h"
#include "recoActivite/recoAffordance/MatriceAffordance.h"
#include "Traces.h"

///To update plan recognition
#include "recoPlan/Policy.h"


using namespace cv;
using namespace std;

class RecognitionManager : public Observer{
	
	public:
	
	/**
	* \fn RecognitionManager(PrimaryWindow* p_view,ManageSourceVideo* p_controller,string pathSetting);
	* \brief Constructor
	* \details Initialize recoImage, recoActivity and recoPlan
	* \param  p_controller : pointer on Object of ManageSourceVideo created in main.cpp
	* \param  p_view : pointer on Object of PrimaryView created in main.cpp
	* \param  setting : map<string,string> which contain every path and information to use (fisrt String = name_of_information, second String = information)
	*/
	RecognitionManager(PrimaryWindow* p_view,ManageSourceVideo* p_controller,map<string,string> setting);
	
	/**
	* \fn virtual ~RecognitionManager();
	* \brief Destructor
	*/
	virtual ~RecognitionManager(){}
	
	/**
	 * \fn recognitionLoop(ManageSourceVideo* p_feedSource);
	 * \brief main loop of recognition which update every composant of the program
	 * \details 
	 * 			take information(image) from controller
	 * 			then update model 
	 * 					activities recognition return the affordance(When a hand interact with an object)
	 * 					then update recognition plan
	 * 			finally update view
	 * \param p_controller : adress of main object of controller to add this to listenner and ask for update
	 * \param p_view : adress of main object of view to show to user 
	 */
	void recognitionLoop(ManageSourceVideo* p_controller,PrimaryWindow* p_view); 
	
	/**
	* \fn updateInternVar();
	* \brief Update variable and display information on console
	*/
	void updateInternVar();
	
	/** 
	* \fn askToQuit();
	* \brief Ask if exit after TIME_BEFORE_ASKING_CLOSURE seconds | UNUSED
	* \details not usefull and never call for now 
	* (we prefer let program run indefinitely)
	* However : Could be useful in many ways (things which can be seen after the main loop)
	*/
	void askToQuit();
	
	/** 
	* \fn updateByObservee(cv::Mat imageColor,cv::Mat imageDepth);
	* \brief update colorMatrix and DepthMatrix 
	* \details belongs to Observer routine (called by controller)
	* TODO : just past a pointer of an unconstant matrix because it's use for see and modify
	* \param	imageColor : 3 matrix which represent RGB of the last image ~ Not sure 
	* \param 	imageDepth : a matrix which represent the depth of last image
	*/
	void updateByObservee(Mat imageColor,Mat imageDepth);
	
	/** 
	* \fn updateAffordanceSeen(DetectedObjects items,DetectedObjects hands,bool removeLastValue);
	* \brief With hands and objects position, and last affordances found the current one then check it
	* \details Update a vector of Affordance sort by their probability (MatriceAffordance)
	* 		   		Could have an affordance with nothing
	* 		   		If we don't see any hands or objects only affordance is NULL with 1 for probability
	*		   Then in function to former image (TIME_LAST_AFFORDANCE) tell what is the most likely affordance
	* 				Can say NULL
	* 			Then look on the average one
	* 			Then check it
	* \param	items : DetectedObjects basically a vector of objects
	* \param 	hands : DetectedObjects basically a vector of hands
	* \param	removeLastValue : boolean which say if we have to remove the first affordance of the vector | after TIME_LAST_AFFORDANCE we start to supress this one
	*/
	void updateAffordanceSeen(DetectedObjects items,DetectedObjects hands,bool removeLastValue);
	
	
	void updatePlanRecognition(Affordance* checkedActAffordance);
	

	private:
	 
	
	///---------Variables----------
	
	///Setting of this application
	map<string,string> setting;
	Traces tr;
	/// time variable
	chrono::milliseconds startTime;// time when program has begun
	chrono::milliseconds actualTime;// time updated at each course of the main loop
    chrono::milliseconds lastsecondTime;// time updated at each seconds
    chrono::milliseconds delay;// time updated each time a new real affordance is detected
    //chrono::milliseconds timeBeforeCheckAff;//time updated when we have checked if the affordance is good, 
    
    ///Initialize play
    bool play=true;	 // = true, because no interface which launch the program yet
    
    /// Count
	long long int countFrame = 0; //number of frame in the whole process (Useless)
	int countSeconds=0;	//count each seconds to know for how many time the loop is running
	int countFPS=0; // count Frame but is reset each seconds so we can evaluate how many FPS we have
	int fps;
	
	///feedSource from Controller
	Mat imageColor; //3 matrix which represent RGB of the last image ~ Not sure 
	Mat imageDepth; //a matrix which represent the depth of last image
	
	///ImageRecognition
	ActivityRegion* act; //Initialize activity recognition
	
	//TODO : reduce this list if possible to complicated
	///	ActivityRecognition | AffordanceRecognition
	MatriceAffordance objectsMat; //(keep in record last affordance seen)
	ObjectAffordances affordances;
	std::vector<AffordanceTime*> currentAffordance;
	Affordance* checkedActAffordance; // Last affordance checked | the one to print
	Affordance* checkedPrecAffordance; // Last affordance checked | the one to print
	std::stack<AffordanceTime*> currentAffordances;
	
	///thread
	thread * threadth;
	///mutex for thread protection
	mutex mtx;
	
	/// Boolean
	bool isStopped = false;
	atomic_bool check;
	bool newCheckedAff=false;
	
	
	/// Plan Recognition
	Policy pl;
	std::vector<std::pair<std::string,float>> tempActions;
	std::vector<std::pair<std::string,float>> tempGoal;
	
	/// 
	
	/// Every information to display on the view
	map<string, map<string,string>> informations;
	


};

