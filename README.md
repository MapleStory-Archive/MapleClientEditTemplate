# MapleClientCollection

A (increasingly generic) framework that is intended to be used when crafting new MapleStory client redirectors/edits.  
This should work pretty much out of the box and all that is required is potential anticheat and crc bypasses. These are version dependant.  
This solution contains all the Windows library hooks that are required for a MapleStory localhost enabler and is configured to be as noob friendly as possible.  
The Windows hooks are abstracted away (as best as I could) and can be toggled on/off depending on need. For example, if using this with an unvirtualized localhost (eg v83), the WinSock hooks can be disabled with a simple switch and memory editing can be done immediately on injection rather than waiting for the client to be unpacked (since it's already unpacked no need to wait).

### DebugView
In order to see the debug log output you can use this program: https://docs.microsoft.com/en-us/sysinternals/downloads/debugview

### Tutorial
I recorded a tutorial on how to use this package which can be watched here: https://www.youtube.com/watch?v=Yr5IOhc5sPk  
ALSO: Join the discord for support! https://discord.gg/tmyac7tU

### Current Status
This project is in active development by myself (Minimum Delta). Issue/feature requests are appreciated.  
The project is in alpha stage, so do not expect it to work perfectly (even though it should work pretty well).  

### Credits
Darter (aka Moozi) 	- Very helpful mentor  
Ez					- Another mentor and friend  
SunCat				- Great insight from this friendly fellow  
The Muffin Man		- Always staying positive  
DAVHEED				- u know 

### Setup
BE ADVISED:  
The below is no longer required, but I'm keeping it here in-case something breaks in the future. The solution should work out-of-the-box with no configuration required.  
_I'm going to expand this at some point, but there's a few things that need to be configured in order for the project to work:_  
* _Put detours.lib into your Debug folder (folder generated on compilation in main folder)_
* _Paste: $(SolutionDir)Common;%(AdditionalIncludeDirectories) in your (CTemplate) project properties -> c/c++ -> additional include directories. This will let the project see the other files for proper compilation. It will not compile unless this is done properly._
* _Sometimes VS defaults to x64 which is incorrect and will not work. Make sure you're compiling in Debug x86 -> if you try to compile in x64 it'll give detour linker errors._

### Config settings
All generic config settings are in the Common/Common.h file. More information on the specifics of this will come at a later point (documentation is always last, eh). However, I've commented reasonbly well so it should be pretty apparent what each setting does.  

### Compiling for proxy vs regular injection
Ijl15 proxy injection (auto injection on Maple start rather than using a launcher) can be turned on/off by defining MAPLE_INJECT_USE_IJL in Common.h.  
You'll need to rename the compiled DLL to ijl15.dll instead of LEN.dll in order for it to be auto injected.

### GenericLauncher
Regular injection can be done with the GenericLauncher project exe. This program will boot up MapleStory and inject LEN.dll. It's very simple, but works very well.

### Common Project
The Common project contains all the Windows library hooks that are not version specific.  
These can be used by any version and are already configured to be hooked on injection. They can be toggled on/off in the Common.h file.
There are also a number of Maple templates in this project -- feel free to contribute yours!

### When does hooking and memory editing happen?
Windows functions are hooked immediately after injection which happens directly after MapleStory is launched (for ijl proxy and regular GenericLauncher injection).  
Unlike MapleStory function hooks, external API hooks (winhooks) do not need to wait for the client to be unpacked. Assuming that the instaject config option is disabled, when the WinAPI CreateMutexA hook is triggered by MapleStory then MainFunc in dllmain is triggered and any hooks or editing called from that function will be executed. If the instaject option is enabled, this happens on injection.  
It is recommended to have all Maple hooking and memory editing inside or called from the CreateMutexA function. This is the default behavior.  

### MapleAPI Hooking Examples
I have added some simple memory editing and hooking examples to the repo. They are also explained in the above linked YouTube series.



