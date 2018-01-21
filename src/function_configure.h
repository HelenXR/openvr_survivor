/** @file 
	@brief configure some function.

	@date 2017.05.25
	@author helenxr(helenhololens@gmail.com)
	
	if you want,you can use this in everywhere.
	if you want to contribute to this project,join us.
*/

#ifndef OPENVR_SURVIVOR_SRC_FUNCTION_CONFIGURE_H_
#define OPENVR_SURVIVOR_SRC_FUNCTION_CONFIGURE_H_

//if your HMD don't have IMU hardware to provide rotate information,you can use keyboard instead for your HMD.
#define HMD_ROTATE_BY_KEYBOARD
//if your HMD don't have position hardware to provide position information,you can use keyboard instead for your HMD.
#define HMD_POSITION_BY_KEYBOARD
//if your controller don't have IMU hardware to provide rotate information,you can use keyboard instead for your controller.
#define CONTROLLER_ROTATE_BY_KEYBOARD
//if your controller don't have position hardware to provide position information,you can use keyboard instead for your controller.
#define CONTROLLER_POSITION_BY_KEYBOARD

//use ximmerse tracking module
//#define USE_XIMMERSE_SIX_DOF_TRACKING_MODULE
//use nolo tracking module
//#define USE_NOLO_SIX_DOF_TRACKING_MODULE

#endif
