// Copyright Filippo Sanfilippo
#include "main.h"
#include "dhdc.h"

#define REFRESH_INTERVAL  0.1   // sec

extern "C"
{

char _lasterrormessage[255];	/* Used to store a DLL error message for transfer to 20-sim */

/* the function that is being called from 20-sim
 *
 * inarr[0] = fx;
 * inarr[1] = fy;
 * inarr[2] = fz;
 * inarr[3] = ta;
 * inarr[4] = tb;
 * inarr[5] = tg;
 * inarr[6] = t;
 *
 * outarr[0] = px;
 * outarr[1] = py;
 * outarr[2] = pz;
 * outarr[3] = oa;
 * outarr[4] = ob;
 * outarr[5] = og;
 * outarr[6] = a;
 *
 */

DLLEXPORT int omega7 (double *inarr, int inputs, double *outarr, int outputs, int major)
{
    if (inputs != 7)
    {
        sprintf(_lasterrormessage, "%s: expects 7 inputs (Fxyz, Tabgt)", __FUNCTION__);
        return FUNCTION_FAILED;	/* Return failed to stop the simulator */
    }

    if (outputs != 7)
    {
        sprintf(_lasterrormessage, "%s: returns 7 outputs (xyz,etc)", __FUNCTION__);
        return FUNCTION_FAILED;	/* Return failed to stop the simulator */
    }

    if (major)
    {
         // apply forces
        if (dhdSetForceAndTorqueAndGripperTorque (inarr[0], inarr[1], inarr[2], inarr[3], inarr[4], inarr[5], inarr[6]) < DHD_NO_ERROR) {
            //printf ("error: cannot set force (%s)\n", dhdErrorGetLastStr());
            sprintf(_lasterrormessage, "error: cannot set force (%s)\n", dhdErrorGetLastStr());
            return FUNCTION_FAILED;	/* Return failed to stop the simulator */
        }

        // write down position and orientation
        if (dhdGetPositionAndOrientationDeg (&outarr[0], &outarr[1], &outarr[2], &outarr[3], &outarr[4], &outarr[5]) < 0) {
            //printf ("error: cannot read position and orientation (%s)\n", dhdErrorGetLastStr());
            sprintf(_lasterrormessage, "error: cannot read position and orientation (%s)\n", dhdErrorGetLastStr());
            return FUNCTION_FAILED;	/* Return failed to stop the simulator */
        }

        // write down position and orientation
        if (dhdGetGripperAngleDeg (&outarr[6]) < 0) {
            //printf ("error: cannot read gripper angle (%s)\n", dhdErrorGetLastStr());
            sprintf(_lasterrormessage, "error: cannot read gripper angle (%s)\n", dhdErrorGetLastStr());
            return FUNCTION_FAILED;	/* Return failed to stop the simulator */
       }
    }

    return FUNCTION_OK;	    /* Success */
}

/****** Initialization and cleanup ******************************************/
/**
 * Initialize() [optional]
 *
 * This function is called by the 20-sim simulator BEFORE starting the simulation experiment
 * (and only once in a multiple run experiment) to initialize the dll properly.
 */
DLLEXPORT int Initialize()
{
    strcpy(_lasterrormessage, "");

   // variables I need to control the haptic loop
 //   double t1,t0  = dhdGetTime ();

    // welcome message
    int major, minor, release, revision;
    dhdGetSDKVersion (&major, &minor, &release, &revision);
//    printf ("\n");
//    printf ("Force Dimension - Gravity Compensation Example %d.%d.%d.%d\n", major, minor, release, revision);
//    printf ("(C) 2011 Force Dimension\n");
//    printf ("All Rights Reserved.\n\n");

    // required to change asynchronous operation mode
    dhdEnableExpertMode ();

    // open the first available device
    if (dhdOpen () < 0) {
        sprintf (_lasterrormessage, "error: cannot open device (%s)\n", dhdErrorGetLastStr());
        dhdSleep (2.0);
        return INITIALIZE_FAILED;
    }

    // identify device
//    printf ("%s device detected\n\n", dhdGetSystemName());

    // display instructions
//    printf ("press BUTTON or 'q' to quit\n");
//    printf ("      's' to disable asynchronous USB operation\n");
//    printf ("      'a' to enable asynchronous USB operation\n");
//    printf ("      'l' to enable read-loop USB operation\n\n");



    // enable fordoublece
    dhdEnableForce (DHD_ON);

    return INITIALIZE_OK; /* Indicate that the dll was initialized successfully. */
}

/**
 * InitializeRun() [optional]
 *
 * This function is called by the 20-sim simulator BEFORE starting the simulation experiment
 * (and only once in a multiple run experiment) to initialize the dll properly.
 */
DLLEXPORT int InitializeRun()
{
    strcpy(_lasterrormessage, "");

    return INITIALIZE_OK; /* Indicate that the dll was initialized successfully. */
}

/**
 * TerminateRun() [optional]
 *
 * This function is called by 20-sim after each finished run
 */
DLLEXPORT int TerminateRun()
{

    /* Cleanup / reset your DLL here for the next run (e.g. in a multiple run experiment) */

    return TERMINATE_OK; // Indicate that the dll was terminated successfully.
}

/**
 * Terminate() [optional]
 *
 * This function is called by 20-sim on a DLL unload
 */
DLLEXPORT int Terminate()
{

    /* Cleanup all allocated memory here */
	dhdClose();

    return TERMINATE_OK; // Indicate that the dll was terminated successfully.
}

/****** Other ************************************************************/

/**
 * LastErrorMessage()
 *
 * This function is called by 20-sim on a return(FUNCTION_FAILED) of your DLL function.
 * It can be used to send an error message to the simulator warning pane.
 */
DLLEXPORT char* LastErrorMessage()
{
    return _lasterrormessage;
}

/**
 * RegisterModelPath()
 *
 * This function is called by 20-sim before Initialize() to learn the DLL where the model is located.
 */
DLLEXPORT int RegisterModelPath(const char * modelPath)
{
    return FUNCTION_OK;
}

} // extern "C"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
