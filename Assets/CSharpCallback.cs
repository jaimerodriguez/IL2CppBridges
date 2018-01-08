using AOT;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;




public class CSharpCallback
{

    delegate int ArithmeticOperationDelegate(int a, int b);


    [DllImport("__Internal")]
    extern static int AddTwoNumbersInNative(int a, int b);


    [MonoPInvokeCallback(typeof(ArithmeticOperationDelegate))]
    static int SubtractTwoNumbersInManagedCode(int a, int b)
    {
        return a - b;
    }

    [DllImport("__Internal")]
    extern static int  SubtractTwoNumbersUsingInlineCallback(int a, int b, ArithmeticOperationDelegate callback);


    [DllImport("__Internal")]
    extern static int MultiplyTwoNumbersUsingExportedFunction(int a, int b );


    [DllImport("__Internal")]
    extern static ArithmeticOperationDelegate GetLateBoundDivison();


    [DllImport("__Internal")]
    extern static ArithmeticOperationDelegate GetLateBoundSubtraction();

    [DllImport("__Internal")] 
    extern static IntPtr GetLateBoundSubtractionAddress();



    [DllImport("__Internal")]
    extern static void SetComparisonCallback (ArithmeticOperationDelegate callback);

    [MonoPInvokeCallback(typeof(ArithmeticOperationDelegate))]
    static int CompareManagedCallback(int a, int b)
    {
        return a - b;
    }

    [StructLayout(LayoutKind.Sequential, CharSet= CharSet.Unicode)]
    struct MActivationParamsAsReturnValue
    {
        public bool isFirstActivation; 
        //Make this  IntPtr so it is a blittable struct 
        public IntPtr commandLinePtr ; 
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    struct MActivationParamsAsParam
    {
        public bool isFirstActivation;
        [MarshalAs(UnmanagedType.LPWStr)]
        public string commandLine;
        public int commandLineBufferSize; 
    };



    [DllImport("__Internal", CallingConvention = CallingConvention.StdCall)] 
    extern static bool TryGetActivationParams( ref MActivationParamsAsParam p );

    [DllImport("__Internal", CallingConvention = CallingConvention.StdCall)]
    extern static MActivationParamsAsReturnValue GetActivationParams( );


    public static void CallChainFromManaged  (  int a, int b )
    {

        ArithmeticOperationDelegate latebound; 
        try
        {
            // Simplest: Just DllImport ... 
            Log(string.Format("{0} + {1} = {2}", a, b, AddTwoNumbersInNative (a, b)));

            // not with a callback.. 
            Log(string.Format("{0} - {1} = {2}", a, b, SubtractTwoNumbersUsingInlineCallback(a, b, SubtractTwoNumbersInManagedCode)));

            // This one function is just a DllImport too.. (like Addition) 
            // but in this case the function can also be called from C++. 
            Log(string.Format("{0} * {1} = {2}", a, b, MultiplyTwoNumbersUsingExportedFunction(a, b)));

            //now we use a latebound function. 
            // Gotcha here is that this does not do great error handling..  this might throw if the latebound method pointer is not set 
            latebound = GetLateBoundDivison();
            Log(string.Format("{0} / {1} = {2}", a * b, b, latebound(a * b, b)));


            MActivationParamsAsParam activationParams = new MActivationParamsAsParam();
            activationParams.commandLineBufferSize = 1024;
            activationParams.commandLine = new String('\0', activationParams.commandLineBufferSize); 
            if ( TryGetActivationParams ( ref activationParams ))
            {
                Log(string.Format("is First: {0}, params:{1}", activationParams.isFirstActivation, activationParams.commandLine));
                 
            }

            MActivationParamsAsReturnValue activationParams2 = GetActivationParams();
            if (activationParams2.commandLinePtr != IntPtr.Zero)
            {
                string commandLine = Marshal.PtrToStringUni(activationParams2.commandLinePtr);
                int targetLevel = 1;
                bool isRoomScale = false ; 
                if ( !TryParseCommandLine ( commandLine ))
                {
                    Log("Failed to parse: " + commandLine ); 
                } 
            } 
            else
            {
                Log("No commandline "); 
            }
        } 
        catch ( EntryPointNotFoundException eeh )
        {
            Log(string.Format("{0} was not found. If you are trying to use it in UNITY_EDITOR, use a plugin for this", eeh.Message));
        }

        // Another approach to late binding. 
        // This is safer on checks if your function is not guaranteed to exist.
        try
        {             
            IntPtr lateboundSubtractionPtr = GetLateBoundSubtractionAddress();    
            if ( lateboundSubtractionPtr != IntPtr.Zero )
            {                 
                ArithmeticOperationDelegate lateboundSubtraction = Marshal.GetDelegateForFunctionPointer(lateboundSubtractionPtr, typeof(ArithmeticOperationDelegate)) as ArithmeticOperationDelegate; 
                if ( lateboundSubtraction != null )
                    Log(string.Format("{0} - {1} = {2}", a, b, lateboundSubtraction(a, b)));
            }
            else
                Log("Subtraction was not defined"); 
        }
        catch  ( Exception ex )
        {             
            Log( ex.Message );
        }


        SetComparisonCallback(CompareManagedCallback);


    }

    static  bool TryParseCommandLine( string commandLine)
    { 
        //TODO: do real parsing of Uri... here i am just illustrating calling the roomscale APIs ;) 
        string commandLineLowerCase = commandLine.ToLower();
        bool isRoomScale = commandLineLowerCase.Contains("roomscale=true");         
        if ( isRoomScale )
        {
            UnityEngine.XR.XRDevice.SetTrackingSpaceType(UnityEngine.XR.TrackingSpaceType.RoomScale);
            Log("Setting SpaceType to Roomscale"); 
        }
        else
        {
            UnityEngine.XR.XRDevice.SetTrackingSpaceType(UnityEngine.XR.TrackingSpaceType.Stationary);
        }
        return true; 

    } 
    static void Log ( string s )
    {
        Debug.Log("***" + s + "***"); 
    }
}
