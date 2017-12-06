#pragma once


#include "stdafx.h"
#include "iostream"
#include "sstream"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringA(os_.str().c_str());  \
}

