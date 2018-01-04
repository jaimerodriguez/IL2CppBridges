
typedef int(__stdcall* CallbackFunc)(int a, int b);
extern "C"
{
	__declspec(dllexport) int __stdcall  MultiplyTwoNumbersExported(int a, int b)
	{
		return a*b;
	}

	static CallbackFunc divisionCallback = 0;
	static CallbackFunc subtractionCallback = 0;
	// static unsigned long subtractionAddress = 0;
	static void * subtractionAddress = 0;

	__declspec(dllexport) void __stdcall  SetLateBoundDivision(CallbackFunc division)
	{
		divisionCallback = division;
	}

	__declspec(dllexport) void __stdcall  SetLateBoundSubtraction(CallbackFunc operation)
	{
		// subtractionAddress = (unsigned long)operation;
		subtractionAddress = (void*)operation;
		subtractionCallback = operation;
	}


	CallbackFunc GetLateBoundDivison()
	{
		return divisionCallback;
	}

	CallbackFunc GetLateBoundSubtraction()
	{
		return subtractionCallback;
	}

	// unsigned long GetLateBoundSubtractionAddress()
	void* GetLateBoundSubtractionAddress() 
	{
		return  subtractionAddress;
	}


	int __stdcall AddTwoNumbersUsingInlineCallback(int a, int b, CallbackFunc callback)
	{
		return callback(a, b);
	}

	int __stdcall  MultiplyTwoNumbersUsingExportedFunction(int a, int b)
	{
		return MultiplyTwoNumbersExported(a, b);
	}


	/*  App.cpp code:

	void App::Initialize(CoreApplicationView^ applicationView)
	{
	SetupOrientation();
	m_AppCallbacks = ref new AppCallbacks();
	m_AppCallbacks->SetCoreApplicationViewEvents(applicationView);
	m_AppCallbacks->Initialized += ref new UnityPlayer::InitializedEventHandler(this, &IL2Sample::App::OnInitialized);

	applicationView->Activated += ref new TypedEventHandler<CoreApplicationView ^, IActivatedEventArgs^>(this, &App::OnActivated);
	}



	typedef int(__stdcall* CallbackFunc)(int a, int b);
	extern "C" int __stdcall MultiplyTwoNumbersExported(int a, int b);
	extern "C" void __stdcall  SetLateBoundDivision(CallbackFunc division);
	extern "C" void __stdcall  SetLateBoundSubtraction(CallbackFunc division);

	#include <iostream>
	#include <string>
	using namespace std;

	extern "C" int __stdcall UnmanagedDivision( int a , int b )
	{
	return a / b;
	}

	void IL2Sample::App::OnInitialized()
	{
	int result = MultiplyTwoNumbersExported( 3, 3);
	SetLateBoundDivision(&UnmanagedDivision);
	SetLateBoundSubtraction(&UnmanagedDivision);
	Platform::String ^resultMessage = L"3*3 = " + result;
	::OutputDebugString(resultMessage->Data());

	}


	*/

}