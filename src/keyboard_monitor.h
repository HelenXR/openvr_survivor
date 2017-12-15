/** @file 
	@brief Use the keyboard instead of the head and hand controllers movement.

	@date 2017.05.25
	@author helenxr(helenhololens@gmail.com)
	
	if you want,you can use this in everywhere.
	if you want to contribute to this project,join us.
*/

#ifndef OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#define OPENVR_SURVIVOR_SRC_KEYBOARD_MONITOR_H_
#include "common.h"
#include <windows.h>
#include "virtual_key_codes.h"

//whether keyboard is Click.Virtual-Key Codes comes from MSDN(https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx)
#define KBC(x)               (GetAsyncKeyState(x) & 0x0001)
//whether keyboard is Touch.Virtual-Key Codes comes from MSDN(https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx)
#define KBT(x)               (GetAsyncKeyState(x) & 0x8000)

/**
	Use the keyboard instead of the head and hand controllers movement.
*/
class KeyBoardMonitor{
public:
	/**
		sington patterns.
		@return pointer to the unique object.
	*/
	static KeyBoardMonitor* GetInstance();
	/**
		destructor.
	*/
	~KeyBoardMonitor();
	/**
		@return hmd's pose.
	*/
	DriverPose_t GetHMDPose();
	/**
		@param[in] hand_controller which hand controller.
		@return the hand controller's pose.
	*/
	DriverPose_t GetControllerPose(const EHandController& hand_controller);
	/**
		@param[in] hand_controller which hand controller.
		@return KeyBoardForControllerButton.
	*/
	KeyBoardForControllerButton GetControllerButtonState(const EHandController& hand_controller);
	bool GetDetectKeyBoardThreadState();
	/**
		@param[in] new_state false:stop true:running
	*/
	bool SetDetectKeyBoardThreadState(const bool& new_state);
protected:	
	/**
		singleton pattern constructor.
	*/
	KeyBoardMonitor();
private:
	/**
		detect all keyboard input.
	*/
	void DetectKeyBoardThread();
	/**
		update HMD pose,it operate by keyboard.
	*/
	void KeyBoardForHMDPoseUpdate();
	/**
		update hand controller pose,it operate by keyboard.
	*/
	void KeyBoardForControllerPoseAndButtonUpdate();
	std::thread m_tDetectKeyBoardThread;	//<keyboard detect thread.
	bool m_bDetectKeyBoardThreadState;		//< false:stop true:running
	DriverPose_t m_sHMDPose;				//< hmd's pose
	DriverPose_t m_sControllerPose[HAND_CONTROLLER_COUNT];	//< hand controller's pose
	static KeyBoardMonitor *m_pKeyBoardMonitor;				//< pointer to unique object
	KeyBoardForControllerButton m_KeyBoardForControllerButton[HAND_CONTROLLER_COUNT];//<hand controller's button state.
};
#endif
