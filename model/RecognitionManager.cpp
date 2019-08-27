#include "RecognitionManager.h"

//TODO : include in config files and prepare their implementation
/// Constant 
const double MAX_FPS_PERFORMANCE = 60.0; // 60 FPS MAX (NOT IMPLEMENTED YET)
const int NUMBER_SECONDS_BEFORE_UPDATE_FPS=1; //in seconds | 
const int TIME_BEFORE_NEW_AFFORDANCE=5000; //in millisecond | Time lapse between 2 differents affordances on the same object
const int TIME_LAST_AFFORDANCE=1000; // in milliseconds | Time during we check the average affordance
const int TIME_BEFORE_ALERT=30000; // in milliseconds | Time without any action before the system turn on an alarm
const int TIME_BEFORE_ASKING_CLOSURE =300000; // in milliseconds (=300s=5min) | DISABLE
const string EXIT_LOOP_WORD="STOP"; // EXIT_LOOP_WORD = "STOP" | DISABLE
RecognitionManager::RecognitionManager(PrimaryWindow* p_view,ManageSourceVideo* p_controller,map<string,string> setting){
	
	///Add setting to his attribute
	this->setting=setting;
	
	///add this object as a listener of this object of ImageTreatment 
	p_controller->registerObserver(this);
	
	
	///Initialize image recognition
	//TODO : MAYBE (I don't know what is the best) replace this singleton to normal definition => remove instance and use a constructor
	act = ActivityRegion::instance(setting); //instance ActivityRegion 
	cout<<"image recognition initialize\n\n";
	
	///Initialize activities/affordance recognition
	this->objectsMat.instance(); //instance ObjectsMat
	checkedActAffordance=new Affordance("None",0,cv::Rect(),0,0); //Initialize an Object of Affordance to avoid crash when comparaison
	checkedPrecAffordance=new Affordance("None",0,cv::Rect(),0,0); //Initialize an Object of Affordance to avoid crash when comparaison
	cout<<"affordance recognition initialize\n\n";
	
	
	///Initialize plan recognition if path of Domain gived in config
	string pathDomain_planRecognition=setting["plan_recognition"];
	if(pathDomain_planRecognition!=""){
		pl.load(pathDomain_planRecognition);
		cout<<"plan recognition initialize"<<endl;
	}
	else
		cout<<"plan recognition disabled"<<endl;
}

void RecognitionManager::recognitionLoop(ManageSourceVideo *p_controller,PrimaryWindow *p_view){ 
	
	
	startTime = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch()); // time when program has begun
	actualTime = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch()); // time updated at each course of the main loop
    lastsecondTime = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch()); // time updated at each seconds
	delay = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch()); // time updated each time a new real affordance is detected
	
	
	cout<<"MAIN LOOP"<<endl;
	
	while(play){
		
			/// Update variable and display information on console
			updateInternVar();
			
			/// Update input (Camera)
			p_controller->update();
	
			/// Update image recognition (send to YOLO which update vectors of hands and objects)
			this->act->Update(imageColor,imageDepth);
			

			/// Boolean which say if we start to remove the oldest frame keeped in memory
			bool removeLastValue=(actualTime-startTime).count()>TIME_LAST_AFFORDANCE;
			//cout<<"Boolean = "<<removeLastValue<<endl;
			
			///	Update affordance recognition
			updateAffordanceSeen(this->act->getItems(),this->act->getHands(),removeLastValue);			
			
			if(setting["plan_recognition"]!="" && newCheckedAff){
				updatePlanRecognition(checkedActAffordance);
				newCheckedAff=false;
			}
			

			/// Send information to View : OUTPUT
			p_view->updateView(this->act,this->currentAffordance); 
			
			//DISABLED | if you want to limit time during loop is running or you want to see end display 
			/// Ask if exit after TIME_BEFORE_ASKING_CLOSURE seconds
			//askToQuit();
		 
	}
}

///Update variable
void RecognitionManager::updateInternVar(){
				countFrame++;
				countFPS++;
				actualTime = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
				/// Update display and information each seconds
				if ((actualTime-lastsecondTime).count()>NUMBER_SECONDS_BEFORE_UPDATE_FPS*1000 ){
					countSeconds+=NUMBER_SECONDS_BEFORE_UPDATE_FPS;
					fps=countFPS/NUMBER_SECONDS_BEFORE_UPDATE_FPS;
					//cout<<"\nTime: "<<countSeconds<<" seconds\n"; 
					//cout<<"FPS: "<<fps<<"\n\n";
					countFPS=0;
					lastsecondTime = actualTime;
				}
				
}

///Exit way (absolutly not necessary) | Unused
void RecognitionManager::askToQuit(){				

			string answer;
			if ((actualTime-startTime).count() > TIME_BEFORE_ASKING_CLOSURE){
				cout<<"Write "<<EXIT_LOOP_WORD<<" to exit the loop :"<<endl;
				cin>>answer;
				
				if(answer!=EXIT_LOOP_WORD){
						cout<<"Reload"<<endl;
						startTime=actualTime;
				}
				else{
						play=!play;
				}
			}
}

//TODO : change to give only a pointer
///Called by Observee (ImageTreatment) 
void RecognitionManager::updateByObservee(cv::Mat imageColor,cv::Mat imageDepth){
		this->imageColor=imageColor;
		this->imageDepth=imageDepth;
		//cout<<feedMatrixColor<<endl; //display matrix of colors RGB with number on console
}

//TO UPGRADE (to many check here could gain some time and the method is not good enough)
void RecognitionManager::updateAffordanceSeen(DetectedObjects items,DetectedObjects hands,bool removeLastValue){
        if (!items.empty() && !hands.empty()) {
            currentAffordance = affordances.findAffordances(items, hands, objectsMat,removeLastValue);
            if (!currentAffordance.empty()) {
                    for (auto it : currentAffordance)
						currentAffordances.push(it);
            }
        }
		else{
			currentAffordance=affordances.addNull(objectsMat,removeLastValue);
			if (!currentAffordance.empty()) {
                    for (auto it : currentAffordance)
						currentAffordances.push(it);
            }
        }
        Affordance* newAffordance=MatriceAffordance::updateAffordance(currentAffordances);
        objectsMat.addnewAff(newAffordance,removeLastValue);
        
        ///To see an affordance from each frame
        //cout<<"New affordance is : "<<newAffordance->getName()<<" | Probability : "<<newAffordance->getObjectProbability()*100<<"%\n";
        ///check if aff not null and (doesn't = the 2 before except if more than 5 sec ) and 3 time in last 0.8 s  
        if(newAffordance->getName()!="NULL" && ( ((newAffordance->getName()!=checkedActAffordance->getName()) && (newAffordance->getName()!=checkedPrecAffordance->getName())) || ((actualTime-delay).count()> TIME_BEFORE_NEW_AFFORDANCE) ) && objectsMat.moreThanAffFrame(newAffordance->getName(),(int)((fps*(TIME_LAST_AFFORDANCE/(double)1000)/5.0)+1))){
				
				checkedPrecAffordance=checkedActAffordance;
				checkedActAffordance=newAffordance;
				newCheckedAff=true;
				delay=actualTime;
				//cout<<"Previous affordance was with : "<<checkedPrecAffordance->getName()<<"\n";
				cout<<"Current affordance with : "<<checkedActAffordance->getName()<<"\n\n";
				cout<<endl;
		}
}

void RecognitionManager::updatePlanRecognition(Affordance* checkedActAffordance){

		   auto checking = pl.update(checkedActAffordance);
                   if (!checking){
                       //std::cout << "L'action a retourne False" << std::endl;
                       pl.Reset();
                   } else {
                       tempActions = pl.getNextActions();
		       tempGoal = pl.getGoalsProba();
			 /// Update plan recognition

                   
                   
                   cout<<"\n\nAction_predicted :\n";
                   for(int i=0;i<tempActions.size();i++)
							cout<<"Action "<<i+1<<" : "<<tempActions[i].first<<" | "<<tempActions[i].second*100<<"%\n";
                   
                   
                   
		   cout<<"\nGoal_predicted :\n";
                   for(int i=0;i<tempGoal.size();i++)
							cout<<"Goal "<<i+1<<" : "<<tempGoal[i].first<<" | "<<tempGoal[i].second*100<<"%\n";
                   }



                  
                   cout<<endl;
}

