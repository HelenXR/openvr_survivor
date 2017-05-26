openvr_survivor							
===============

About
-----
This is an open source project based on steamVR development driver, used in the windows platform, can provide you with the following functions:

 - If you're a regular gamer, you can experience all the SteamVR games on the keyboard as long as you have a set of PC and use compiled files.
 - If you're a hardware  driver developer, you can customize your HMD, handle controllers, and other peripherals based on this, so that they can be used on the steam platform, and you can refer to the already tuned virtual device, driver.
 - If you're an application developer, you can debug your application based on analog devices and troubleshoot hardware devices.
 - If you're already an experienced developer and passionate about VR, we want to be able to work together on some of the more interesting things, such as distortion, ATW, TW, tracking, prediction, and so forth. Join us.

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

Third party dependency Library
------------------------------
- glog:log system, in order to better use, in the source code on the basis of some changes made.
- glm:used to handle four element transformations.
- hidapi:HID data interaction.
- openvr:openVR interface.
- doxygen:for document.

Development plan
----------------

- Fix the handle shake while moving
- Protect critical resources,e.g.HMD's pose and handle controllers' pose.
- Add distortion function.
- Added direct rendering support.
- Problem fixing and improvement.

This is a preliminary version, relying on personal strength is difficult to perfect, if you have good suggestions or would like to join us, please contact me, my mailbox:helenhololens@gmail.com.
