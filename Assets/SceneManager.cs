using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SceneManager : MonoBehaviour {

	// Use this for initialization
	void Start () {

         
        CSharpCallback.CallChainFromManaged (3, 5); 

    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
