openvr_survivor							
===============

About
-----
This is an open source project based on steamVR development driver, used in the windows platform, can provide you with the following functions:

 - If you're a regular gamer, you can experience all the SteamVR games on the keyboard as long as you have a set of PC and use compiled files.
 - If you're a hardware  driver developer, you can customize your HMD, handle controllers, and other peripherals based on this, so that they can be used on the steam platform, and you can refer to the already tuned virtual device, driver.
 - If you're an application developer, you can debug your application based on analog devices and troubleshoot hardware devices.
 - If you're already an experienced developer and passionate about VR, we want to be able to work together on some of the more interesting things, such as distortion, ATW, TW, tracking, prediction, and so forth. Join us.

SteamVR game example:

![the_body_vr](https://github.com/HelenXR/Documents/blob/master/picture/openvr_survivor_the_body_game.jpg)
How to use
----------
Use CMake to build visual studio project, and then open the "openvr_survivor.sln", compiled the driver file package openvr_survivor, and then copy it to the "Steam\steamapps\common\SteamVR\drivers" directory, and then modify the default.vrsettings:

```
    /**
       file under the path:openvr_survivor\resources\settings
       set your monitor resolution.
   */
      "windowWidth" : 1920,
      "windowHeight" : 1080,
      "renderWidth" : 1920,
      "renderHeight" : 1080,
```
more build information in [wiki](https://github.com/HelenXR/openvr_survivor/wiki)
[compiled driver](https://github.com/HelenXR/Documents/blob/master/package/openvr_survivor.rar)

HMD 6dof:
```
rotate information:
	yaw:'a'  left, 'd' right.
	pitch:'w' up, 's' or 'x'down.
	roll:'q' anticlockwise,'c' clockwise.
	other function:'c' clear rotate.
position information:
	forward:'VK_NUMPAD8',back:'VK_NUMPAD2'.
    	right:'VK_NUMPAD6',left:'VK_NUMPAD4'.
	up:'VK_NUMPAD9',down:'VK_NUMPAD1'.
	other function:'VK_NUMPAD5' clear position.
	note:The numeric key above is the numeric key on the right.
```
right hand controller:
```
right controller rotate information:
	yaw:'j'  left, 'l' right.
	pitch:'i' up, 'k' or ',' down.
	roll:'u' anticlockwise,'o' clockwise.
	other function:'.' clear rotate.
right controller position information:
	forward:'VK_UP',back:'VK_DOWN'.
	right:'VK_RIGHT',left:'VK_LEFT'.
	up:'VK_NUMPAD0',down:'VK_RCONTROL'.
	other function:'VK_DECIMAL' clear position.
right controller button information:
	Menu button:'VK_BACK'
	Trackedpad Left:'VK_KEY_F'
	Trackedpad Up:'VK_KEY_T'
	Trackedpad Right:'VK_KEY_H'
	Trackedpad Down:'VK_KEY_G' or 'VK_KEY_B'
	System Button:'VK_ESCAPE'
	Trigger:'VK_SPACE','7','9'
	Grip Button:'VK_KEY_9'
	Trackpad Press:'VK_KEY_0'
```

Third party dependency Library
------------------------------
- glog:log system, in order to better use, in the source code on the basis of some changes made.
- glm:used to handle four element transformations.
- hidapi:HID data interaction.
- openvr:openVR interface.
- doxygen:for document.

Development plan
----------------
- ~~optimization hand controller control.~~
- Fix the handle shake while moving
- Protect critical resources,e.g.HMD's pose and handle controllers' pose.
- ~~Add distortion function.~~
- Added direct rendering support.
- Problem fixing and improvement.
- ~~Add recenter function.~~
- ~~Add six dof module support(ximmerse & nolo).~~

This is a preliminary version, relying on personal strength is difficult to perfect, if you have any good suggestions ,you can post in [gitter chat room](https://gitter.im/openvr_survivor/Lobby),and if you would like to join us, please contact me, my mailbox:helenhololens@gmail.com. 
