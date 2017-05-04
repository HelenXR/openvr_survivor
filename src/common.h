#ifndef OPENVR_SURVIVOR_SRC_COMMON_H_
#define OPENVR_SURVIVOR_SRC_COMMON_H_
#include <openvr_driver.h>
#include <iostream>
#include <windows.h>
//In order to prevent ERROR and GLOG / windows.h in the header file ERROR to redefine the problem. 
//This macro must be defined in the logging.h contains before
#define GLOG_NO_ABBREVIATED_SEVERITIES 
#include <glog/logging.h>

//namespace
using namespace std;
using namespace vr;
using namespace google;

//import lib
#pragma comment(lib,"libglog.lib")

#endif
