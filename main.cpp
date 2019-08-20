/**
 * \file      main.cpp
 * \author    Mathieu Gravel - Edited by Alexandre Gonzalvez
 * \version   2.0
 * \date      25 June 2019
 * \brief     main.cpp : Function launching the entire project
 * \details   Function launching the entire project
 * 				Create Model, View and Controller
 * 				Take configuration files in argument
*/
using namespace std;

#include <iostream>
#include <map> //Setting area

///Model
#include "model/RecognitionManager.h"

///View
#include "view/PrimaryWindow.h"

///Controller
#include "controller/ManageSourceVideo.h"

///Function 
/**
	* \fn loadingSetting(string fileSetting)
	* \brief load setting
	* \param  fileSetting : name of the file which contains the setting, he as to be in the same folder than the main.cpp
	* \return setting : map<string,string> which contain every path and information to use (fisrt String = name_of_information, second String = information)
*/
map<string,string> loadingSetting(string fileSetting){
    ifstream fichier(fileSetting, ios::in);
	
	map<string,string>  setting;	
    if (fichier) {
        string line;
		
        while(getline(fichier, line)) {
			if(line.substr(0,1)!="#"){
				string name = line.substr(0, line.find("="));
				string value = line.substr(line.find("=")+1);
				setting.insert ( pair<string,string>(name,value) );
			}
        }
        fichier.close();
    }
    else {
        cout << "Impossible to open configuration file !" << endl;
    }
    return setting;
}

/**
	* \fn int main(int argc, char *argv[]);
	* \brief launch the program
	* \param  argc 	number of arguments
	* \param  argv	vector of String (argument written when called)
	* \return 42 when program stop without crash
*/
int main(int argc, char *argv[])
{
	cout<<"Begin\n"<<endl;
	
	map<string,string> setting;
	
	///Load settings
	if(argc>1){
		setting=loadingSetting(argv[1]);
		cout<<"Setting Loaded\n\n";
	}
	else{
		cout<<"No setting files gived, default paramater\n\n";
		//Default are with webcam(640x480), view ON, planRecognition=smallDomain, DefaultArea=Ball, pathVideo=None
		//Order doesn't matter
		setting = {{"Display_View","True"},{"plan_recognition","model/plan/ressources/domains/smallDomain.txt"},{"Input_Video","Webcam"},{"pathSourceVideo","color.mp4"},{"pathSourceVideoDepth","depth.mp4"},{"Webcam_Width","640"},{"Webcam_Height","480"},{"GPUKitchen_dat","model/recoImage/ressources/models/yolo-kitchen.data"},{"GPUKitchen_cfg","model/recoImage/ressources/models/corpusKitchen.cfg"},{"GPUKitchen_wei","model/recoImage/ressources/models/corpusKitchen.backup"},{"GPUKitchen_wei","model/recoImage/ressources/models/corpusKitchen.backup"},{"GPUKitchen_lbl","model/recoImage/ressources/models/classes-kitchen.name"},{"GPUDefault_dat","model/recoImage/ressources/models/classes-kitchen.name"},{"GPUDefault_dat","model/recoImage/ressources/models/classes-kitchen.name"},{"GPUDefault_dat","model/recoImage/ressources/models/balls.classes"},{"GPUDefault_cfg","model/recoImage/ressources/models/balls.cfg"},{"GPUDefault_wei","model/recoImage/ressources/models/balls.backup"},{"GPUDefault_lbl","model/recoImage/ressources/models/balls.name"},{"CPUKitchen_network","model/recoImage/ressources/models/yolov3-kitchen.backup"},{"CPUKitchen_networkDef","model/recoImage/ressources/models/yolov3-kitchen.cfg"},{"CPUKitchen_Name","model/recoImage/ressources/models/c-kitchen.name"},{"CPUDefault_network","model/recoImage/ressources/models/balls.backup"},{"CPUDefault_networkDef","model/recoImage/ressources/models/balls.cfg"},{"CPUDefault_Name","model/recoImage/ressources/models/c-balls.name"}};
	}
	
	/// Display setting in console
	// /*
	cout<<"\nSetting :\n";
	for (auto const& x : setting){
			cout << x.first  // string (key)
              << ':' 
              << x.second<<"\n"; // string's value 
	}
	// */
	
	/// Start video acquisition
	ManageSourceVideo controller(setting);
	cout<<"Controller On\n";
	
	/// Create View object (Window if boolean is true) TODO: Window create RecognitionManager when button play is handle (require create a complex window)
	PrimaryWindow view(setting["Display_View"]=="True"); 

	/// Initalize reconnaissance
	RecognitionManager model(&view,&controller,setting);
	cout<<"\nModel On\n";
	
	cout<<"\nPress a key or wait 5 seconds to start the main loop\n\n";
	waitKey(5000); //flush input buffer to resolv swhen argc > 0
	
	///Start main loop
	model.recognitionLoop(&controller,&view); 
	
	cout<<"\n\nEnd"<<endl;
	
    return 42;
}

