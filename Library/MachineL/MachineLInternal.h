/*
* Machine Learning Library - Internal
*
*   Toby Opferman
*/


#ifndef __MACHINE_LEARNING_INTERNAL
#define __MACHINE_LEARNING_INTERNAL


typedef MACHINEL_STATUS (*PFN_TRAININGPROC)(HMACHINE hMachine, HMATRIX hFeatureTrainingData, HMATRIX hResultsVector);
typedef MACHINEL_STATUS(*PFN_PREDICTPROC)(HMACHINE hMachine, HMATRIX hInputData, double *pPrediction);

typedef struct _MACHINE_L_INTERNAL {
	
	PFN_TRAININGPROC pfnTrainingProc;
	PFN_PREDICTPROC pfnPredictProc;

	MACHINE_LEARNING_TYPE MachineLearningType;
	int NumberOfFeatures;
	HMATRIX hTheta;
	UINT TrainingItterations;
	double LearningRate;

} MACHINE_L_INTERNAL, *PMACHINE_L_INTERNAL;



#endif
