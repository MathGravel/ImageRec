#pragma once
#include "stdafx.h"
#include <string>


class Items {

	Items();
	void setPosition(int posx, int posy, int posz);
	void setItemType(std::string itemName);
	void setiItemBitmap(HBITMAP img);
};