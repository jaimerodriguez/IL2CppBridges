
typedef int(__stdcall* NativeArithmeticOperation)(int a, int b);
extern "C"
{
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


	/*  App.cpp code:

	void App::Initialize(CoreApplicationView^ applicationView)
	{
		// These are there, just leave them alone 
		SetupOrientation();
		m_AppCallbacks = ref new AppCallbacks();
		m_AppCallbacks->SetCoreApplicationViewEvents(applicationView);


		// This is the line to add
		m_AppCallbacks->Initialized += ref new UnityPlayer::InitializedEventHandler(this, &IL2Sample::App::OnInitialized);

		//This should already be there. 
		applicationView->Activated += ref new TypedEventHandler<CoreApplicationView ^, IActivatedEventArgs^>(this, &App::OnActivated);
	}


	// Add all of the below too.. 

	typedef int(__stdcall* CallbackFunc)(int a, int b);
	extern "C" int __stdcall MultiplyTwoNumbersExported(int a, int b);
	extern "C" void __stdcall  SetLateBoundDivision(CallbackFunc division);
	extern "C" void __stdcall  SetLateBoundSubtraction(CallbackFunc division);

	#include <iostream>
	#include <string>
	using namespace std;

	extern "C" int __stdcall UnmanagedDivision(int a, int b)
	{
	if (b != 0)
	return a / b;
	else
	return UINT32_MAX;
	}

	extern "C" int __stdcall UnmanagedSubtraction(int a, int b)
	{
	return a - b;
	}

	void  App::OnInitialized()
	{
	int result = MultiplyTwoNumbersExported(3, 3);
	SetLateBoundDivision(&UnmanagedDivision);
	SetLateBoundSubtraction(&UnmanagedSubtraction);
	Platform::String ^resultMessage = L"3*3 = " + result;
	::OutputDebugString(resultMessage->Data());

	}


	*/

}