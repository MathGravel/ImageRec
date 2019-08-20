#include "PrimaryWindow.h"

PrimaryWindow::PrimaryWindow(bool display):display(display){
	if(display){
		namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
		cout<<"View ON\n\n";
	}
	else{
		cout<<"View as been disable\n";
	}
	
}

void PrimaryWindow::updateView(ActivityRegion *act,std::vector<AffordanceTime*> currentAffordance){
	if(display){
			treatPicture(act,currentAffordance);
			imshow( "Display window", act->currentImage );    // Show our image inside it.
			waitKey(1);      //	Wait an instant
	}
}


//TODO : review this function, in particular / Seems to do useless things

void PrimaryWindow::treatPicture(ActivityRegion* act,std::vector<AffordanceTime*> currentAffordance) {
	if(display){
		int fontface = cv::FONT_HERSHEY_SIMPLEX;
		double scale = 1;
		int thickness = 2;
		int baseline = 0;
		cv::Mat pic = act->currentImage;
		if (!act->regions.empty()) {
			for (auto &reg : act->regions) {
				cv::rectangle(pic, reg, cv::Scalar(100, 100, 100), 3);
			}
		} else if (!act->items.empty()) {
			for (auto &reg :  act->items) {
					cv::rectangle(pic, reg.getObjPos(), cv::Scalar(reg.getRed(),reg.getGreen(),reg.getBlue()), 6);
					std::string val=reg.getObjName() + " "+std::to_string((int) floor(reg.getProb() * 100)) + "%";
					//std::string val = showName?  reg.getObjName() + " "  : "";
					//val += showPercentage ?  std::to_string((int) floor(reg.getProb() * 100)) + "%" : "";
					
					cv::Scalar color(reg.getRed(),reg.getGreen(),reg.getBlue());
					if (pastAffordance != NULL && pastAffordance->getName() == reg.getObjName()) {
						color= cv::Scalar(255,255,255);
					}

					cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
					cv::Rect textBox(reg.getObjPos());
					textBox.y -= text.height;

					textBox.height = text.height;
					textBox.width = text.width * 1.04;
					cv::rectangle(pic,textBox,color,-1);

					cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 0, 0),
						thickness, 8);

			}
		}

		int i  = act->hands.getObjects().size();
		for (auto & hand :  act->hands) {
		   
			cv::rectangle(pic, hand.getObjPos(), cv::Scalar(hand.getRed(),hand.getGreen(),hand.getBlue()), 6);
			std::string val=hand.getObjName() + " "+std::to_string((int) floor(hand.getProb() * 100)) + "%";
			//std::string val = showName?  hand.getObjName() + " "  : "";
			//val += showPercentage ?  std::to_string((int) floor(hand.getProb() * 100)) + "%" : "";

			cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
			cv::Rect textBox(hand.getObjPos());
			textBox.y -= text.height;

			textBox.height = text.height;
			textBox.width = text.width * 1.04;
			cv::rectangle(pic,textBox,cv::Scalar(hand.getRed(),hand.getGreen(),hand.getBlue()),-1);

			cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 0, 0),
				thickness, 8);

		}

		if (! currentAffordance.empty()) {
			for (std::vector<AffordanceTime *>::const_iterator it =  currentAffordance.begin(); it !=  currentAffordance.end();it++) {

				Affordance &pos = (*it)->getAffordance();
				pastAffordance = *it;
				
				cv::rectangle(pic, pos.getRegion(), cv::Scalar(250, 0, 0), 4);
				std::string val = pos.getName() + " " + std::to_string((int) floor(pos.getObjectProbability() * 100)) + "%";
				//std::string val = showName?  pos.getName() + " "  : "";
				//val += showPercentage ?  std::to_string((int) floor(pos.getObjectProbability() * 100)) + "%" : "";

				cv::Size text = cv::getTextSize(val, fontface, scale, thickness, &baseline);
				cv::Rect textBox(pos.getRegion());
				textBox.y -= text.height;

				textBox.height = text.height;
				textBox.width = text.width * 1.04;
				cv::rectangle(pic,textBox,cv::Scalar(255,255,255),-1);
				cv::putText(pic, val, cv::Point(textBox.x, textBox.y + text.height), fontface, scale, CV_RGB(0, 0, 0),
					thickness, 8);
			}

		}
		//this->imageTreated = pic;
		//cv::resize(this->imageTreated,this->resizedPicture,cv::Size(screenSize.first,screenSize.second));
		//programFeed.push_back(resizedPicture.clone());

	}
}

