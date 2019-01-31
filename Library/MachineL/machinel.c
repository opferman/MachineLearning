/*
*  Toby Opferman
*
*    Machine Learning Library
*
*/

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <matrix.h>
#include <machinel.h>
#include "machinelinternal.h"
#include "logicregression.h"
#include "linearregression.h"



/*********************************************************
*
*  MachineLearning_Create
*
*
*
*********************************************************/
HMACHINE MachineLearning_Create(PMACHINE_LEARNING_CREATE pMachineLearningCreate)
{
	HMACHINE hMachine = NULL;

	switch (pMachineLearningCreate->MachineLearningType)
	{
	      case MACHINE_LINEAR_REGRESSION:
			  hMachine = LinearRegression_Create(pMachineLearningCreate);
			  break;
	}


	return hMachine;

}


/*********************************************************
*
*  MachineLearning_Predict
*
*
*
*********************************************************/
MACHINEL_STATUS MachineLearning_Predict(HMACHINE hMachine, HMATRIX hInputData, double *pPrediction)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;

	return pMachineLearningInternal->pfnPredictProc(hMachine, hInputData, pPrediction);
}

/*********************************************************
*
*  MachineLearning_Training
*
*
*
*********************************************************/
MACHINEL_STATUS MachineLearning_Training(HMACHINE hMachine, HMATRIX hFeatureTrainingData, HMATRIX hResultsVector)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;

	return pMachineLearningInternal->pfnTrainingProc(hMachine, hFeatureTrainingData, hResultsVector);
}


/*********************************************************
*
*  MachineLearning_Destroy
*
*
*
*********************************************************/
void MachineLearning_Destroy(HMACHINE hMachine)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;

	Matrix_Destroy(pMachineLearningInternal->hTheta);
	LocalFree(pMachineLearningInternal);
}



/*********************************************************
*
*  MachineLearning_SetInitialTheta
*
*
*
*********************************************************/
void MachineLearning_SetInitialTheta(HMACHINE hMachine, HMATRIX hTheta)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;

	Matrix_Destroy(pMachineLearningInternal->hTheta);
	pMachineLearningInternal->hTheta = hTheta;
}




/*********************************************************
*
*  MachineLearning_SetLearningRate
*
*
*
*********************************************************/
void MachineLearning_SetLearningRate(HMACHINE hMachine, double LearningRate)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;

	pMachineLearningInternal->LearningRate = LearningRate;
}




/*********************************************************
*
*  MachineLearning_SetTrainingItterations
*
*
*
*********************************************************/
void MachineLearning_SetTrainingItterations(HMACHINE hMachine, UINT TrainingItterations)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;

	pMachineLearningInternal->TrainingItterations = TrainingItterations;
}

