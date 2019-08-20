/**
 * \file      view/PrimaryWindow.h
 * \author    Mathieu Gravel Edited by Alexandre Gonzalvez
 * \version   2.0
 * \date      8 August 2019
 * \brief     interface of the program 
 * \details   If boolean = True
 * 			  	take a the matrix of color, position of hands and objects in input
 * 			  	draw rectangle over it int the matrix
 * 			 	 and display it
 * 			  Else do nothing
 * 			TO ADD : 
 * 				 DISPLAY
 * 					   display current action and 2 previous or more
 * 					   display next action (most likely) (like 3)
 * 					   display current plan (most likely) (like 3)
 * 				 	   Alert when the user do an action which is unlikely
 * 				BUTTON
 * 					   To start the program
 * 					   To choose option (controller(realSense/webcam/video/kinect), ect ... (maybe FPS, GPU/CPU, langage))
 * 				AGEWELL & GDAC/UQAM & author (J.Massardi/M.Gravel/E.Beaudry)
*/

#pragma once

#include <vector>

///To use Mat (data from model)
#include <opencv2/opencv.hpp>

/// To show image
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/// To have access to ActivityRegion attribut and Affordance to draw over hands and objects
#include "../model/recoActivite/recoImage/ActivityRegion.h"
#include "../model/recoActivite/recoAffordance/Affordance.h"

using namespace cv;

class PrimaryWindow{
	public :
	
	/**
	* \fn PrimaryWindow(bool display);
	* \brief Constructor
	* \details enable all composant only if display is true
	* \param display : send by configuration file | without one display is ON
	*/
	PrimaryWindow(bool display);
	
	/**
	* \fn virtual ~View();
	* \brief Destructor
	*/
	virtual ~PrimaryWindow(){}
	
	/**
	* \fn updateView(cv::Mat imageColor);
	* \brief update View 
	* \details called by RecognitionManager at each course of the loop
	* 		call treatPicture which draw rectangle over the initial picture
	* \param act : Object of ActivityRegion: contains imageColor, position of hands and objects
	* \param currentAffordance :  vector of Object of AffordanceTime contain position of hands and objects

	*/
	void updateView(ActivityRegion *act,std::vector<AffordanceTime*> currentAffordance);
	
	/**
	* \fn treatPicture(ActivityRegion* act);
	* \brief draw rectangle over hands and 
	* \details called by updateView
	* \param act : Object of ActivityRegion: contains imageColor, position of hands and objects
	* \param currentAffordance :  vector of Object of AffordanceTime contain position of hands and objects
	*/
	void treatPicture(ActivityRegion* act,std::vector<AffordanceTime*> currentAffordance);
	
	private :
	
	AffordanceTime * pastAffordance;
	bool display;
};
