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
1. It exports functions from the loose .cpp file, so that the host process/project can call these functions in your GameAssembly.dll. 
There is no magic in the exports; it is standard ___declspec(dllexport)_
1. It uses known P/Invoke and marshalling to make the calls between C# and C++. You can see these in [CSharpCallback.cs](https://github.com/jaimerodriguez/IL2CppBridges/blob/master/Assets/CSharpCallback.cs). 

## How to use? 
1. Clone the repo
1. In Build Settings, Switch Target Universal Windows Platform. Ensure you are also using IL2CPP (the default on 2017 builds). 
1. Build from Unity. _I wrote this and tested it using 2017.2.f3, but any recent Unity build should work._
1. Open the VS generated solution and copy the commented out code from the CPPMethods.cpp into your App.cpp file in the generated project. Instructions are in the file. 
1. Run the app. See the output in Visual Studio's output window. 

## Issues/Feedback
Feel free to file an issue if you have a suggestion or have issues. Create a PR if you have a better sample or want to tweak the UX. I kept it raw (it is just Debug.Log) on purpose, for easiest learning.  
