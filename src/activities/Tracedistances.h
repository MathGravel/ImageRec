/**
 * \file      TraceDistances.h
 * \author    Mathieu Gravel
 * \version   1.0
 * \date      13 June 2019
 * \brief     
 * \details   
 */
#ifndef TRACEDISTANCES_H
#define TRACEDISTANCES_H

#include <string>
#include<vector>
#include<map>

#include<sstream>
#include<fstream>

#include "Affordance.h"
#include "DetectedObject.h"



class TraceDistances
{
public:

	/**
	* \fn TraceDistances(std::string traceLoc,std::vector<std::string> classes,std::pair<int,int> screen)
	* \brief Constructor of class RecoManager  
	* \details    	
	* \param traceLoc 	String : directory name which contain every kitchen_object trace
	* \param classes 	vector of String : kitchen_object name
	* \param screen 	2 int associated : Represent the screenSize
	*/
    TraceDistances(std::string traceLoc,std::vector<std::string> classes,std::pair<int,int> screen);

	/**
	* \fn ~TraceDistances()
	* \brief Destructor of class TraceDistances  || Do nothing yet : To change i guess
	*/
    ~TraceDistances();
    void addHandDist(std::string classe,double distance, char hand);
    void dumpBuffer();

private:
    std::map<std::string,std::stringstream> buffer;
    std::string fileLoc;
    std::map<std::string,std::string> trace;

    std::pair<int,int> screenSize;
    std::string last;

};

#endif // TRACEDISTANCES_H
