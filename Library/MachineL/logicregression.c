/*
*  Toby Opferman
*
*    Logic Regression Library
*
*/

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <matrix.h>
#include <machinel.h>
#include "machinelinternal.h"
#include "logicregression.h"




/*
 * Internal Functions
 */
MACHINEL_STATUS LogicRegression_Internal_Training(HMACHINE hMachine, HMATRIX hFeatureTrainingData, HMATRIX hResultsVector);
MACHINEL_STATUS LogicRegression_Internal_Predict(HMACHINE hMachine, HMATRIX hInputData, double *pPrediction);
double LogicRegression_Internal_Sigmoid(double Z);
double LogicRegression_Internal_CostFunction(PMACHINE_L_INTERNAL pMachineLearningInternal, HMATRIX hTrainingSet, HMATRIX hResults);
double LogicRegression_Internal_ThetaMultiplyX(PMACHINE_L_INTERNAL pMachineLearningInternal, int Matrix_n, double *X);
void LogicRegression_Internal_GradientDescent(PMACHINE_L_INTERNAL pMachineLearningInternal, int Matrix_m, int Matrix_n, double *X, double *Y);


/*********************************************************
*
*  LogicRegression_Create
*
*
*
*********************************************************/
HMACHINE LogicRegression_Create(PMACHINE_LEARNING_CREATE pMachineLearningCreate)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal;

	pMachineLearningInternal = (PMACHINE_L_INTERNAL)LocalAlloc(LMEM_ZEROINIT, sizeof(MACHINE_L_INTERNAL));

	if (pMachineLearningInternal)
	{
		pMachineLearningInternal->NumberOfFeatures = pMachineLearningCreate->NumberOfFeatures;
		pMachineLearningInternal->hTheta = pMachineLearningCreate->hTheta;
		pMachineLearningInternal->TrainingItterations = pMachineLearningCreate->TrainingItterations;
		pMachineLearningInternal->LearningRate = pMachineLearningCreate->LearningRate;
		pMachineLearningInternal->pfnTrainingProc = LogicRegression_Internal_Training;
		pMachineLearningInternal->pfnPredictProc = LogicRegression_Internal_Predict;
	}

	return (HMACHINE)pMachineLearningInternal;
}

/*********************************************************
*
*  LogicRegression_Internal_Training
*
*
*
*********************************************************/
MACHINEL_STATUS LogicRegression_Internal_Training(HMACHINE hMachine, HMATRIX hFeatureTrainingData, HMATRIX hResultsVector)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;
	MACHINEL_STATUS MachineStatus;
	double BeforeGradient;
	double AfterGradient;
	int Matrix_m;
	int Matrix_n;
	double *X;
	double *Y;


	Matrix_m = Matrix_GetNumRows(hFeatureTrainingData);
	Matrix_n = Matrix_GetNumCols(hFeatureTrainingData);

	X = Matrix_GetDirectIndex(hFeatureTrainingData);
	Y = Matrix_GetDirectIndex(hResultsVector);

	BeforeGradient = LogicRegression_Internal_CostFunction(pMachineLearningInternal, hFeatureTrainingData, hResultsVector);

	LogicRegression_Internal_GradientDescent(pMachineLearningInternal, Matrix_m, Matrix_n, X, Y);

	AfterGradient = LogicRegression_Internal_CostFunction(pMachineLearningInternal, hFeatureTrainingData, hResultsVector);

	if (AfterGradient < BeforeGradient)
	{
		MachineStatus = MACHINE_L_SUCCESS;
	}
	else
	{
    	MachineStatus = MACHINE_L_ERROR;
	}

	return MachineStatus;
}


/*********************************************************
*
*  LogicRegression_Internal_Predict
*
*
*
*********************************************************/
MACHINEL_STATUS LogicRegression_Internal_Predict(HMACHINE hMachine, HMATRIX hInputData, double *pPrediction)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;
	double Sigmoid;
	int Matrix_n;
	double *pInputData;

	Matrix_n = Matrix_GetNumCols(hInputData);
	pInputData = Matrix_GetDirectIndex(hInputData);
	
	*pPrediction = LogicRegression_Internal_Sigmoid(LogicRegression_Internal_ThetaMultiplyX(pMachineLearningInternal, Matrix_n, pInputData));

	return MACHINE_L_SUCCESS;
}

/*********************************************************
*
*  LogicRegression_Sigmoid
*
*
*
*********************************************************/
double LogicRegression_Internal_Sigmoid(double Z)
{
	double Result = 0;

	Result = 1.0 / (1 + exp(-1.0*Z));

	return Result;
}


/*********************************************************
*
*  LogicRegression_Internal_CostFunction
*
*
*
*********************************************************/
double LogicRegression_Internal_CostFunction(PMACHINE_L_INTERNAL pMachineLearningInternal, HMATRIX hTrainingSet, HMATRIX hResults)
{
	double Result;
	double *pTrainingSet;
	double *pResults;
	double *pTheta;
	double Sigmoid;
	int Index;
	int Matrix_m;
	int Matrix_n;

	Matrix_m = Matrix_GetNumRows(hTrainingSet);
	Matrix_n = Matrix_GetNumCols(hTrainingSet);

	pTrainingSet = Matrix_GetDirectIndex(hTrainingSet);
	pResults = Matrix_GetDirectIndex(hResults);
	pTheta = Matrix_GetDirectIndex(pMachineLearningInternal->hTheta);

	Result = 0.0;

	for (Index = 0; Index < Matrix_m; Index++)
	{
		Sigmoid = LogicRegression_Internal_Sigmoid(LogicRegression_Internal_ThetaMultiplyX(pMachineLearningInternal, Matrix_n, &pTrainingSet[Index*Matrix_n]));
		Result += ((pResults[Index] * log(Sigmoid)) + ((1- pResults[Index])*log((1 - Sigmoid))));
	}

	Result = (1.0 / (double)Matrix_m)*Result;

	return Result;
}


/*********************************************************
*
*  LogicRegression_Internal_ThetaMultiplyX
*
*
*
*********************************************************/
double LogicRegression_Internal_ThetaMultiplyX(PMACHINE_L_INTERNAL pMachineLearningInternal, int Matrix_n, double *X)
{
	double *Theta; 
	double Result;
	int Index;

	Theta = Matrix_GetDirectIndex(pMachineLearningInternal->hTheta);
	Result = 0.0;

	for (Index = 0; Index < Matrix_n; Index++)
	{
		Result += (Theta[Index]*X[Index]);
	}
	
	return Result;
}

/*********************************************************
*
*  LogicRegression_Internal_GradientDescent
*
*
*
*********************************************************/
void LogicRegression_Internal_GradientDescent(PMACHINE_L_INTERNAL pMachineLearningInternal, int Matrix_m, int Matrix_n, double *X, double *Y)
{
	int Index;
	int TrainingIndex;
	double Sigmoid;
	double *Result;
	double *pTheta;
	HMATRIX hTempResults;

	pTheta = Matrix_GetDirectIndex(pMachineLearningInternal->hTheta);
	hTempResults = Matrix_CreateIdenticalSize(pMachineLearningInternal->hTheta, 0.0);
	
	if (hTempResults)
	{
		Result = Matrix_GetDirectIndex(hTempResults);
		for (Index = 0; Index < Matrix_n; Index++)
		{
			Result[Index] = 0.0;
			for (TrainingIndex = 0; TrainingIndex < Matrix_m; TrainingIndex++)
			{
				Sigmoid = LogicRegression_Internal_Sigmoid(LogicRegression_Internal_ThetaMultiplyX(pMachineLearningInternal, Matrix_n, &X[TrainingIndex*Matrix_n]));
				Result[Index] = Result[Index] + (Sigmoid - Y[TrainingIndex])*X[TrainingIndex*Matrix_n + Index];
			}

			Result[Index] = Result[Index] * (1.0 / (double)Matrix_m);

			if (Index != 0)
			{
				Result[Index] = Result[Index] + (pMachineLearningInternal->LearningRate / (double)Matrix_m)*pTheta[Index];
			}
		}

		Matrix_FastSwap(pMachineLearningInternal->hTheta, hTempResults);
		Matrix_Destroy(hTempResults);
	}
}