/*
* Machine Learning Library
*
*   Toby Opferman
*/


#ifndef __MACHINE_LEARNING_H__
#define __MACHINE_LEARNING_H__

typedef PVOID HMACHINE;

typedef enum _MACHINE_LEARNING_TYPE {

	MACHINE_LINEAR_REGRESSION,
	MACHINE_LOGIC_REGRESSION,
	MACHINE_NEURAL_NETWORK

} MACHINE_LEARNING_TYPE, *PMACHINE_LEARNING_TYPE;

typedef enum _MACHINEL_STATUS {

	MACHINE_L_SUCCESS,
	MACHINE_L_ERROR,
	MACHINE_L_UNIMPLEMENTED

} MACHINEL_STATUS, *PMACHINEL_STATUS;

typedef struct _MACHINE_LEARNING_CREATE {

	MACHINE_LEARNING_TYPE MachineLearningType;
	int NumberOfFeatures;
	HMATRIX hTheta;
	UINT TrainingItterations;
	double LearningRate;

} MACHINE_LEARNING_CREATE, *PMACHINE_LEARNING_CREATE;

HMACHINE MachineLearning_Create(PMACHINE_LEARNING_CREATE pMachineLearningCreate);
MACHINEL_STATUS MachineLearning_Training(HMACHINE hMachine, HMATRIX hFeatureTrainingData, HMATRIX hResultsVector);
MACHINEL_STATUS MachineLearning_Predict(HMACHINE hMachine, HMATRIX hInputData, double *pPrediction);
void MachineLearning_Destroy(HMACHINE hMachine);

void MachineLearning_SetInitialTheta(HMACHINE hMachine, HMATRIX hTheta);
void MachineLearning_SetLearningRate(HMACHINE hMachine, double LearningRate);
void MachineLearning_SetTrainingItterations(HMACHINE hMachine, UINT TrainingItterations);

#endif

