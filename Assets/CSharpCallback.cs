using AOT;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;




public class CSharpCallback
{
    [MonoPInvokeCallback(typeof(AddTwoNumbersDelegate))]
    static int AddTwoNumbersInManagedCode(int a, int b)
    {
        return a + b;
    }

    delegate int AddTwoNumbersDelegate(int a, int b);

    [DllImport("__Internal")]
    extern static int  AddTwoNumbersUsingInlineCallback(int a, int b, AddTwoNumbersDelegate callback);

    [DllImport("__Internal")]
    extern static int MultiplyTwoNumbersUsingExportedFunction(int a, int b );


    AddTwoNumbersDelegate lateBoundCallback;


    [DllImport("__Internal")] 
    extern static AddTwoNumbersDelegate GetLateBoundDivison ();

    [DllImport("__Internal")]
    extern static AddTwoNumbersDelegate GetLateBoundSubtraction ();


    [DllImport("__Internal")]
   // extern static ulong GetLateBoundSubtractionAddress();
   extern static IntPtr GetLateBoundSubtractionAddress(); 

    public static void CallChainFromManaged  (  int a, int b )
    {

        AddTwoNumbersDelegate latebound; 
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


        try
        {
            latebound = GetLateBoundSubtraction();
            // if (latebound.Method != null)
           // if ( latebound != null && latebound.Method != null )
            {
                Debug.Log(string.Format("{0} - {1} = {2}", a , b, latebound(a, b)));
            }

        } catch (Exception ex)
        {
            Debug.Log(ex.Message); 
        }
        
         
        try
        {

            //  ulong address = GetLateBoundSubtractionAddress(); 
            // IntPtr lateboundSubtractionPtr = new IntPtr((long)address); 
            // if ( address != 0 && lateboundSubtractionPtr !=  null  )
            IntPtr lateboundSubtractionPtr = GetLateBoundSubtractionAddress();    
            if ( lateboundSubtractionPtr != null )
            {
                //        var lateboundSubtraction = Marshal.GetDelegateForFunctionPointer<AddTwoNumbersDelegate>(lateboundSubtractionPtr); 
                AddTwoNumbersDelegate lateboundSubtraction = Marshal.GetDelegateForFunctionPointer(lateboundSubtractionPtr, typeof(AddTwoNumbersDelegate)) as AddTwoNumbersDelegate; 
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
