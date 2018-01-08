# IL2Cpp to C# interop sample
A couple code snippets that demonstrate how to interoperate between C# methods (pre IL2CPP generation) and native code (post IL2CPP generation or the host) for a Unity project.


## Why? 
For Universal Windows Apps (which include Windows Mixed Reality Apps), IL2CPP offers a good way to reuse existing C# plugins compiled to full .NET 3.5 framework without having to re-write code to target the subset of .NET APIs supported in Universal Windows Apps. 
This works great, but seldomly when doing this you want to call from the host into Unity (C# code) to maybe pass initialization/launch parameters, or to detect first time activation (vs. subsequent ones).

## How does it work? 
There is no magic. It uses standard C# P/Invoke and marshalling techniques.  
Here are the few details that a few Unity devs might not already know, since IL2CPP is still 'new'.   
1. It adds a .cpp file as a loose file in your Unity project. You can do this and configure it for a specific platform, the same way you configure plugins in the inspector. In this case, I made it WSA Player specific. 
The file is [CPPMethods.cpp](https://github.com/jaimerodriguez/IL2CppBridges/blob/master/Assets/CPPMethods.cpp)
1. It exports functions and global variables from the loose .cpp file, so that the host process/project can call these functions in your GameAssembly.dll. 
There is no magic in the exports; it is standard ___declspec(dllexport)_
1. It uses known P/Invoke and marshalling to make the calls between C# and C++. You can see these in [CSharpCallback.cs](https://github.com/jaimerodriguez/IL2CppBridges/blob/master/Assets/CSharpCallback.cs). 

## How to use? 
1. **Clone the repo**
1. **Open in Unity.** _I wrote this and tested it using 2017.2.f3, but any recent Unity build should work._  
 Note: You can't run the sample in the Unity Editor as most of the sample is referencing methods that don't get compiled until the IL2CPP code is generated and compiled. This is 'by design' in this sample. 
1. In Build Settings, **select Universal Windows Platform as target**, and **Click the Switch Platform button.**  
 Note: Sample assumes you are using IL2CPP; that is the default on 2017, so you won't have do do anything, just don't change it.  
 Note: The sample calls a Unity XR API (XRDevice.SetTrackingSpaceType), but Unity does not crash if you do not enable VR support, so I leave that as optional.  If you want to enable it, check the _Player Settings -> XR Settings -> Virtual Reality Supported_ checkbox. 
1. **Build** from Unity. 
1. **Open the VS generated solution** (IL2CppBridges.sln) and 
2. **Copy the commented out code from the CPPMethods.cpp into your App.cpp and App.h files in the generated project.**  
 Instructions are in the file. There are two blocks of commented out code that you can copy to replace the full contents of the generated files.  Also, the file is copied (by Unity) to the Il2CppOutputProject project under the _Source\CppPlugins_ path. 
1. **Build**. Ensure you change target x64 (as Unity defaults to ARM) and Local Machine. 
1. **Run the app in the VS debugger**. See the output in Visual Studio's output window. All the lines have a *** prefix.  

## When to use, or why? 
This sample was written to show:
1. How to parse parameters from protocol launcher activation in a WinMR app. You will find that code in the App::OnActivated method.  
2. We needed to pass the query string from the protocol launcher to Unity, and have these readily available as Unity loaded even the first scene, so we used the (native) global variable g_ActivationParams to hold the state. 
3. To retrive the state, we used PInvoke to call the native GetActivationParams function from a C# MonoBehaviour. 

Though the flow is simple, the original (verbal) explanation was not clear, and quickly evolved into a few questions on marshaling, and loose .cpp files, etc. so we I am leaving this as a reference for the next time that scenario comes up. 


## Issues/Feedback
Feel free to file an issue if you have a suggestion or have issues. Create a PR if you have a better sample or want to tweak the UX. I kept it raw (it is just Debug.Log) on purpose, for easiest learning.  
