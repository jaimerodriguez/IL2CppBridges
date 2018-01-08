
#include <wchar.h> 
#include <objbase.h>

typedef int(__stdcall* NativeArithmeticOperation)(int a, int b);

typedef  struct _ReadinessStatus
{
	bool canCompare;
	NativeArithmeticOperation comparisonOperator;
} ReadinessStatus;


typedef struct _NActivationParams
{
	bool isFirstActivation;
	wchar_t* CommandLine = nullptr;
} NActivationParams;


typedef struct _NActivationParamsPreAllocated
{
	bool isFirstActivation;
	wchar_t* CommandLine = nullptr;
	int commandLineBufferSize;
} NActivationParamsPreAllocated;

__declspec(dllexport) ReadinessStatus ComparisonCallbackStatus;
__declspec(dllexport) NActivationParams g_ActivationParams;


#ifdef __cplusplus 
extern "C"
{
#endif 
	int __stdcall AddTwoNumbersInNative(int a, int b)
	{
		return a + b;
	}

	int __stdcall SubtractTwoNumbersUsingInlineCallback(int a, int b, NativeArithmeticOperation callback)
	{
		return callback(a, b);
	}

	void __stdcall SetComparisonCallback(NativeArithmeticOperation comparison)
	{
		//TODO: is this right check?? 
		ComparisonCallbackStatus.canCompare = (comparison != nullptr);
		ComparisonCallbackStatus.comparisonOperator = comparison;

	};

	__declspec(dllexport) int __stdcall  MultiplyTwoNumbersExported(int a, int b)
	{
		return a*b;
	}

	static NativeArithmeticOperation divisionCallback = 0;
	static NativeArithmeticOperation subtractionCallback = 0;
	// static unsigned long subtractionAddress = 0;
	static void* subtractionAddress = 0;

	__declspec(dllexport) void __stdcall  SetLateBoundDivision(NativeArithmeticOperation division)
	{
		divisionCallback = division;
	}

	__declspec(dllexport) void __stdcall  SetLateBoundSubtraction(NativeArithmeticOperation operation)
	{
		// subtractionAddress = (unsigned long)operation;
		subtractionAddress = (void*)operation;
		subtractionCallback = operation;
	}


	NativeArithmeticOperation GetLateBoundDivison()
	{
		return divisionCallback;
	}

	NativeArithmeticOperation GetLateBoundSubtraction()
	{
		return subtractionCallback;
	}

	// unsigned long GetLateBoundSubtractionAddress()
	void* GetLateBoundSubtractionAddress()
	{
		return  subtractionAddress;
	}


	int __stdcall AddTwoNumbersUsingInlineCallback(int a, int b, NativeArithmeticOperation callback)
	{
		return callback(a, b);
	}

	int __stdcall  MultiplyTwoNumbersUsingExportedFunction(int a, int b)
	{
		return MultiplyTwoNumbersExported(a, b);
	}



	bool __stdcall TryGetActivationParams(NActivationParamsPreAllocated* pActivationParams)
	{
		pActivationParams->isFirstActivation = g_ActivationParams.isFirstActivation;
		int len = 0;
		if (g_ActivationParams.CommandLine != nullptr)
		{
			len = wcslen(g_ActivationParams.CommandLine) + 1;
			if (len < pActivationParams->commandLineBufferSize)
				wcsncpy(pActivationParams->CommandLine, g_ActivationParams.CommandLine, len);
			else
				return false;
		}
		return true;
	}

	NActivationParams __stdcall  GetActivationParams()
	{
		NActivationParams result;
		result.isFirstActivation = g_ActivationParams.isFirstActivation;
		result.CommandLine = g_ActivationParams.CommandLine;
		return result;
	}

#ifdef __cplusplus 
}
#endif 



/*  App.cpp code:

 #include "pch.h"
#include "App.h"
#include "UnityGenerated.h"

using namespace IL2CppBridges;
using namespace Platform;
using namespace UnityPlayer;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;

void App::Initialize(CoreApplicationView^ applicationView)
{
    SetupOrientation();
    m_AppCallbacks = ref new AppCallbacks();
    m_AppCallbacks->SetCoreApplicationViewEvents(applicationView);
	m_AppCallbacks->Initialized += ref new UnityPlayer::InitializedEventHandler(this, &App::OnInitialized);

    applicationView->Activated += ref new TypedEventHandler<CoreApplicationView ^, IActivatedEventArgs^>(this, &App::OnActivated);

}

void App::SetWindow(CoreWindow^ window)
{
    m_CoreWindow = window;

    ApplicationView::GetForCurrentView()->SuppressSystemOverlays = true;

    m_AppCallbacks->SetCoreWindowEvents(window);
    m_AppCallbacks->InitializeD3DWindow();
}

void App::Load(String^ entryPoint)
{
}

void App::Run()
{
    m_AppCallbacks->Run();
}

void App::Uninitialize()
{
    m_AppCallbacks = nullptr;
}

IFrameworkView^ App::CreateView()
{
    return this;
}

#include <iostream>
#include <string>
#include <ppltasks.h> 

using namespace std;
using namespace concurrency;



typedef int(__stdcall* CallbackFunc)(int a, int b);
typedef struct _NActivationParams
{
	bool isFirstActivation;
	wchar_t* CommandLine = nullptr;
} NActivationParams;

typedef struct _ReadinessStatus
{
	bool canCompare;
	CallbackFunc comparisonOperator;
} ReadinessStatus;


__declspec(dllimport)  NActivationParams g_ActivationParams;
__declspec(dllimport) ReadinessStatus ComparisonCallbackStatus;

extern "C"
{
	extern int __stdcall MultiplyTwoNumbersExported(int a, int b);
	extern void __stdcall  SetLateBoundDivision(CallbackFunc division);
	extern void __stdcall  SetLateBoundSubtraction(CallbackFunc division);
	int __stdcall UnmanagedDivision(int a, int b)
	{
		return a / b;
	}

	void DoAsyncComparison()
	{
		create_task([]
		{
			bool isTimeOut = false;
			int maxretries = 15;
			int retries = 0;
			while (retries++ < maxretries)
			{
				if (ComparisonCallbackStatus.canCompare)
				{
					int result = ComparisonCallbackStatus.comparisonOperator(5, 3);
					Platform::String ^resultMessage = L"5>3 = " + (result > 0);
					::OutputDebugString(resultMessage->Data());
					break;
				}
				::Sleep(1000);
			}
		});
	}
}

void App::OnInitialized()
{
	g_ActivationParams.isFirstActivation = true;

	// Add a default value just for testing.. if you did not configure protocol handler or launch app using protocol handler
	if ( g_ActivationParams.CommandLine == nullptr ) 
		g_ActivationParams.CommandLine = L"il2cppbridge://targetlevel=16&roomscale=true";
	 
	int result = MultiplyTwoNumbersExported(3, 3);
	Platform::String ^resultMessage = L"3*3 = " + result;
	
	SetLateBoundDivision(&UnmanagedDivision);

	DoAsyncComparison();

}

void App::OnActivated(CoreApplicationView^ sender, IActivatedEventArgs^ args)
{
	if (args->Kind == ActivationKind::Protocol)
	{
		ProtocolActivatedEventArgs^ protocolArgs = dynamic_cast<ProtocolActivatedEventArgs^>(args);
		Platform::String^ launchUri = protocolArgs->Uri->ToString(); 
		int destlen = launchUri->Length() + 1; 
		g_ActivationParams.CommandLine = new wchar_t[destlen]; 		
		wcsncpy_s (g_ActivationParams.CommandLine, destlen, launchUri->Data(), (size_t)launchUri->Length());   	 
	}
	m_CoreWindow->Activate();
}

void App::SetupOrientation()
{
	Unity::SetupDisplay();
}

*/

 
/* App.h 
#pragma once

namespace IL2CppBridges
{
	ref class App sealed :
		public Windows::ApplicationModel::Core::IFrameworkView,
		public Windows::ApplicationModel::Core::IFrameworkViewSource
	{
	public:
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
		virtual void Load(Platform::String^ entryPoint);
		virtual void Run();
		virtual void Uninitialize();

		virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();

	private:
		UnityPlayer::AppCallbacks^ m_AppCallbacks;
		Windows::UI::Core::CoreWindow^ m_CoreWindow;
		void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ sender, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
		void SetupOrientation();

		//This line below is the only difference from default generated code.
		void OnInitialized();

	};
}

*/
