using AOT;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;




public class CSharpCallback
{
    [MonoPInvokeCallback(typeof(ArithmeticOperationDelegate))]
    static int AddTwoNumbersInManagedCode(int a, int b)
    {
        return a + b;
    }

    delegate int ArithmeticOperationDelegate(int a, int b);

    [DllImport("__Internal")]
    extern static int  AddTwoNumbersUsingInlineCallback(int a, int b, ArithmeticOperationDelegate callback);

    [DllImport("__Internal")]
    extern static int MultiplyTwoNumbersUsingExportedFunction(int a, int b );


    ArithmeticOperationDelegate lateBoundCallback;


    [DllImport("__Internal")] 
    extern static ArithmeticOperationDelegate GetLateBoundDivison ();

    [DllImport("__Internal")]
    extern static ArithmeticOperationDelegate GetLateBoundSubtraction ();


    [DllImport("__Internal")]
   // extern static ulong GetLateBoundSubtractionAddress();
   extern static IntPtr GetLateBoundSubtractionAddress(); 

    public static void CallChainFromManaged  (  int a, int b )
    {

        ArithmeticOperationDelegate latebound; 
        try
        {
            Debug.Log(string.Format("{0} + {1} = {2}", a, b, AddTwoNumbersUsingInlineCallback(a, b, AddTwoNumbersInManagedCode)));
            Debug.Log(string.Format("{0} * {1} = {2}", a, b, MultiplyTwoNumbersUsingExportedFunction(a, b)));
            latebound = GetLateBoundDivison();
            Debug.Log(string.Format("{0} / {1} = {2}", a * b, b, latebound(a * b, b)));
        } 
        catch ( EntryPointNotFoundException eeh )
        {
            Debug.Log(string.Format("{0} was not found. If you are trying to use it in UNITY_EDITOR, use a plugin for this", eeh.Message));
        }

        // One approach to late binding. This is safer on checks if your function is not guaranteed to exist.
        try
        {             
            IntPtr lateboundSubtractionPtr = GetLateBoundSubtractionAddress();    
            if ( lateboundSubtractionPtr != IntPtr.Zero )
            {                 
                ArithmeticOperationDelegate lateboundSubtraction = Marshal.GetDelegateForFunctionPointer(lateboundSubtractionPtr, typeof(ArithmeticOperationDelegate)) as ArithmeticOperationDelegate; 
                if ( lateboundSubtraction != null )
                    Debug.Log(string.Format("{0} - {1} = {2}", a, b, lateboundSubtraction(a, b)));
            }
            else
                Debug.Log("Subtraction was not defined"); 
        }
        catch  ( Exception ex )
        {             
            Debug.Log( ex.Message );
        }         
    }


}
