/*
*  Toby Opferman
*
*    Linear Regression Implementatino
*
*/

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <matrix.h>
#include <machinel.h>
#include "MachineLInternal.h"
#include "linearregression.h"


/*
 * Functions
 */

MACHINEL_STATUS LinearRegression_Internal_Training(HMACHINE hMachine, HMATRIX hFeatureTrainingData, HMATRIX hResultsVector);
MACHINEL_STATUS LinearRegression_Internal_Predict(HMACHINE hMachine, HMATRIX hInputData, double *pPrediction);
double LinearRegression_Internal_GradientDescent(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY, int JIndex, double LearningRate);
double LinearRegression_Internal_J_CostFunc(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY);

/*********************************************************
*
*  LinearRegression_Create
*
*
*
*********************************************************/
HMACHINE LinearRegression_Create(PMACHINE_LEARNING_CREATE pMachineLearningCreate)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal;

	pMachineLearningInternal = (PMACHINE_L_INTERNAL)LocalAlloc(LMEM_ZEROINIT, sizeof(MACHINE_L_INTERNAL));

	if (pMachineLearningInternal)
	{
		pMachineLearningInternal->NumberOfFeatures    = pMachineLearningCreate->NumberOfFeatures;
		pMachineLearningInternal->hTheta              = pMachineLearningCreate->hTheta;
		pMachineLearningInternal->TrainingItterations = pMachineLearningCreate->TrainingItterations;
		pMachineLearningInternal->LearningRate        = pMachineLearningCreate->LearningRate;
		pMachineLearningInternal->pfnTrainingProc     = LinearRegression_Internal_Training;
		pMachineLearningInternal->pfnPredictProc      = LinearRegression_Internal_Predict;
	}

	return (HMACHINE)pMachineLearningInternal;
}



/*********************************************************
*
*  LinearRegression_Internal_Training
*
*
*
*********************************************************/
MACHINEL_STATUS LinearRegression_Internal_Training(HMACHINE hMachine, HMATRIX hFeatureTrainingData, HMATRIX hResultsVector)
{
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;
	MACHINEL_STATUS MachineStatus;
	HMATRIX hTempTheta;
	UINT TrainingItteration;
	UINT J_Index;
	double BeforeTrainingJ_Cost;
	double *pTempTheta;
	double AfterTrainingJ_Cost;

	hTempTheta = Matrix_CreateIdenticalSize(pMachineLearningInternal->hTheta, 0.0);

	if (hTempTheta)
	{
		BeforeTrainingJ_Cost = LinearRegression_Internal_J_CostFunc(pMachineLearningInternal->hTheta, hFeatureTrainingData, hResultsVector);

		for (TrainingItteration = 0; TrainingItteration < pMachineLearningInternal->TrainingItterations; TrainingItteration++)
		{
			pTempTheta = Matrix_GetDirectIndex(hTempTheta);

			for (J_Index = 0; J_Index < (UINT)Matrix_GetNumRows(pMachineLearningInternal->hTheta); J_Index++)
			{
				pTempTheta[J_Index] = LinearRegression_Internal_GradientDescent(pMachineLearningInternal->hTheta, hFeatureTrainingData, hResultsVector, J_Index, pMachineLearningInternal->LearningRate);
			}

			Matrix_FastSwap(pMachineLearningInternal->hTheta, hTempTheta);
		}

		AfterTrainingJ_Cost = LinearRegression_Internal_J_CostFunc(pMachineLearningInternal->hTheta, hFeatureTrainingData, hResultsVector);

		if (AfterTrainingJ_Cost < BeforeTrainingJ_Cost)
		{
			MachineStatus = MACHINE_L_SUCCESS;
		}
		else
		{
			MachineStatus = MACHINE_L_ERROR;
		}

		Matrix_Destroy(hTempTheta);
	}

	return MachineStatus;
}


/*********************************************************
*
*  LinearRegression_Internal_GradientDescent
*
*
*
*********************************************************/
double LinearRegression_Internal_GradientDescent(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY, int JIndex, double LearningRate)
{
	double NewTheta;
	int Matrix_m;
	int Matrix_n;
	int Index_M;
	int Index_N;
	double *pX;
	double *pY;
	double *pTheta;
	double Function_H;
	double J_Loop;

	/*
	*   tj = tj - a(1/m) E (h(x)-y)xj
	*/
	Matrix_m = Matrix_GetNumRows(hValuesX);
	Matrix_n = Matrix_GetNumCols(hValuesX);

	pTheta = Matrix_GetDirectIndex(Thetha);
	pX = Matrix_GetDirectIndex(hValuesX);
	pY = Matrix_GetDirectIndex(hValuesY);

	J_Loop = 0;
	for (Index_M = 0; Index_M < Matrix_m; Index_M++)
	{
		Function_H = 0.0;

		for (Index_N = 0; Index_N < Matrix_n; Index_N++)
		{
			Function_H += pTheta[Index_N] * pX[Index_M*Matrix_n + Index_N];
		}
		Function_H = (Function_H - pY[Index_M])*pX[Index_M*Matrix_n + JIndex];
		J_Loop += Function_H;
	}

	NewTheta = pTheta[JIndex] - ((LearningRate)*(1.0 / (double)Matrix_m))*J_Loop;

	return NewTheta;
}





/*********************************************************
*
*  LinearRegression_Internal_J_CostFunc
*
*
*
*********************************************************/
double LinearRegression_Internal_J_CostFunc(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY)
{
	double J_Answer;
	double h_Answser;
	double *pTheta;
	double *pX;
	double *pY;
	int Matrix_m;
	int Matrix_n;
	int Index_M;
	int Index_N;

	/*
	 *  J()=1/2m E (h(x) - y)^2
	 */ 

	Matrix_m = Matrix_GetNumRows(hValuesX);
	Matrix_n = Matrix_GetNumCols(hValuesX);

	pTheta = Matrix_GetDirectIndex(Thetha);
	pX = Matrix_GetDirectIndex(hValuesX);
	pY = Matrix_GetDirectIndex(hValuesY);
	
	J_Answer = 0.0;

	for (Index_M = 0; Index_M < Matrix_m; Index_M++)
	{
		h_Answser = 0.0;

		for (Index_N = 0; Index_N < Matrix_n; Index_N++)
		{
			h_Answser += pTheta[Index_N] * pX[Index_M*Matrix_n + Index_N];
		}

		J_Answer = J_Answer + ((h_Answser - pY[Index_M])*(h_Answser - pY[Index_M]));
	}

	J_Answer = J_Answer * (1.0 / (2.0 * ((double)Matrix_m)));
		
	return J_Answer;
}


/*********************************************************
*
*  LinearRegression_Internal_Predict
*
*
*
*********************************************************/
MACHINEL_STATUS LinearRegression_Internal_Predict(HMACHINE hMachine, HMATRIX hInputData, double *pPrediction)
{
	MACHINEL_STATUS MachineStatus = MACHINE_L_SUCCESS;
	PMACHINE_L_INTERNAL pMachineLearningInternal = (PMACHINE_L_INTERNAL)hMachine;
	double *pTheta;
	double *pX;
	double h_Answser;
	int n;
	int Index;

	/*
	*  t0*x0 + t1*x1 + ...
	*/

	n = Matrix_GetNumCols(hInputData);

	pTheta = Matrix_GetDirectIndex(pMachineLearningInternal->hTheta);
	pX = Matrix_GetDirectIndex(hInputData);

	*pPrediction = 0.0;

	for (Index = 0; Index < n; Index++)
	{
		*pPrediction += pTheta[Index] * pX[Index];
	}


	return MachineStatus;
}

