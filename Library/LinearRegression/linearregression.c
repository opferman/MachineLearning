/*
*  Toby Opferman
*
*    Vector and Matrix Library
*
*/

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <matrix.h>
#include <linearregression.h>


/*
 * Functions
 */
double LinearRegression_Internal_J_CostFunc_Flat(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY);
double LinearRegression_Internal_Gradient_Flat(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY, int JIndex, double LearningRate);
double LinearRegression_Internal_CostFunc_Flat(HMATRIX Thetha, HMATRIX hValuesX);

HLINEARREG LinearRegression_Create_Flat(PLINEAR_CREATE pLinearCreate);

//LINEAR_STATUS LinearRegression_Internal_CostFunc_Increasing(HMATRIX Thetha, HMATRIX hValuesX, int RowOfX);
//LINEAR_STATUS LinearRegression_Internal_CostFunc_Square(HMATRIX Thetha, HMATRIX hValuesX, int RowOfX);

typedef double (*PLINEAR_JCOSTFUNC)(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY);
typedef double (*PLINEAR_GRADFUNC)(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY, int JIndex, double LearningRate);
typedef double(*PLINEAR_COSTFUNC)(HMATRIX Thetha, HMATRIX hValuesX);

typedef struct _LINEAR_INTERNAL {

	PLINEAR_JCOSTFUNC pfnJCostFunc;
	PLINEAR_GRADFUNC pfnGradFunc;
	PLINEAR_COSTFUNC pfnCostFunc;
	int NumberOfFeatures;
	HMATRIX hTheta;
	UINT TrainingItterations;

} LINEAR_INTERNAL, *PLINEAR_INTERNAL;


/*********************************************************
*
*  LinearRegression_Create
*
*
*
*********************************************************/
HLINEARREG LinearRegression_Create(PLINEAR_CREATE pLinearCreate)
{
	HLINEARREG hLinearReg = NULL;

	switch (pLinearCreate->LinearEquationType)
	{
	      case LINEAR_FLAT:
			  hLinearReg = LinearRegression_Create_Flat(pLinearCreate);
			  break;
	}


	return hLinearReg;

}


/*********************************************************
*
*  LinearRegression_Create_Flat
*
*
*
*********************************************************/
HLINEARREG LinearRegression_Create_Flat(PLINEAR_CREATE pLinearCreate)
{
	PLINEAR_INTERNAL pLinearInternal;

	pLinearInternal = (PLINEAR_INTERNAL)LocalAlloc(LMEM_ZEROINIT, sizeof(LINEAR_INTERNAL));

	if (pLinearInternal)
	{
		pLinearInternal->NumberOfFeatures  = pLinearCreate->NumberOfFeatures;
		pLinearInternal->pfnJCostFunc      = LinearRegression_Internal_J_CostFunc_Flat;
		pLinearInternal->pfnCostFunc       = LinearRegression_Internal_CostFunc_Flat;
		pLinearInternal->pfnGradFunc       = LinearRegression_Internal_Gradient_Flat;
		pLinearInternal->hTheta            = pLinearCreate->hTheta;
		pLinearInternal->TrainingItterations = pLinearCreate->TrainingItterations;
	}

	return (HLINEARREG)pLinearInternal;
}



/*********************************************************
*
*  LinearRegression_Training_GradientDescent
*
*
*
*********************************************************/
LINEAR_STATUS LinearRegression_Training_GradientDescent(HLINEARREG hLinearReg, HMATRIX hTrainingData, HMATRIX hResultsVector, double LearningRate)
{
	PLINEAR_INTERNAL pLinearInternal = (PLINEAR_INTERNAL)hLinearReg;
	LINEAR_STATUS LinearStatus = LINEAR_SUCCESS;
	UINT TrainingIndex;
	UINT SummationIndex;
	UINT VariableIndex;
	UINT Training;
	HMATRIX hTempTheta;
	HMATRIX hSwap;
	int J_Index;
	double *pCurrentTheta;
	double *pTempTheta;
	double J_Cost;
	
	hTempTheta = Matrix_CreateIdenticalSize(pLinearInternal->hTheta, 0.0);

	if (hTempTheta)
	{
		for (Training = 0; Training < pLinearInternal->TrainingItterations; Training++)
		{
			pCurrentTheta = Matrix_GetDirectIndex(pLinearInternal->hTheta);
			pTempTheta = Matrix_GetDirectIndex(hTempTheta);

			J_Cost = pLinearInternal->pfnJCostFunc(pLinearInternal->hTheta, hTrainingData, hResultsVector);

			for (J_Index = 0; J_Index < Matrix_GetNumRows(pLinearInternal->hTheta); J_Index++)
			{
				pTempTheta[J_Index] = pLinearInternal->pfnGradFunc(pLinearInternal->hTheta, hTrainingData, hResultsVector, J_Index, LearningRate);
			}

			Matrix_FastSwap(pLinearInternal->hTheta, hTempTheta);
		}
	}

	return LinearStatus;
}


/*********************************************************
*
*  LinearRegression_Predict
*
*
*
*********************************************************/
double LinearRegression_Predict(HLINEARREG hLinearReg, HMATRIX hInputData)
{
	PLINEAR_INTERNAL pLinearInternal = (PLINEAR_INTERNAL)hLinearReg;

	return pLinearInternal->pfnCostFunc(pLinearInternal->hTheta, hInputData);
}

/*********************************************************
*
*  LinearRegression_Training_NormalEquation
*
*
*
*********************************************************/
LINEAR_STATUS LinearRegression_Training_NormalEquation(HLINEARREG hLinearReg, HMATRIX hTrainingData, HMATRIX hResultsVector)
{
	return LINEAR_UNIMPLEMENTED;
}


/*********************************************************
*
*  LinearRegression_Destroy
*
*
*
*********************************************************/
void LinearRegression_Destroy(HLINEARREG hLinearReg)
{
	PLINEAR_INTERNAL pLinearInternal = (HLINEARREG)hLinearReg;

	Matrix_Destroy(pLinearInternal->hTheta);
	LocalFree(pLinearInternal);
}


/*********************************************************
*
*  LinearRegression_Internal_J_CostFunc_Flat
*
*
*
*********************************************************/
double LinearRegression_Internal_J_CostFunc_Flat(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY)
{
	double J_Answer;
	double h_Answser;
	double *pTheta;
	double *pX;
	double *pY;
	int m;
	int n;
	int Index;
	int Index2;

	/*
	 *  J()=1/2m E (h(x) - y)^2
	 */ 

	m = Matrix_GetNumRows(hValuesX);
	n = Matrix_GetNumCols(hValuesX);

	pTheta = Matrix_GetDirectIndex(Thetha);
	pX = Matrix_GetDirectIndex(hValuesX);
	pY = Matrix_GetDirectIndex(hValuesY);
	
	J_Answer = 0.0;

	for (Index = 0; Index < m; Index++)
	{
		h_Answser = 0.0;

		for (Index2 = 0; Index2 < n; Index2++)
		{
			h_Answser += pTheta[Index2] * pX[Index*n + Index2];
		}

		J_Answer = J_Answer + ((h_Answser - pY[Index])*(h_Answser - pY[Index]));
	}

	J_Answer = J_Answer * (1.0 / (2.0 * ((double)m)));
		
	return J_Answer;
}


/*********************************************************
*
*  LinearRegression_Internal_CostFunc_Flat
*
*
*
*********************************************************/
double LinearRegression_Internal_CostFunc_Flat(HMATRIX Thetha, HMATRIX hValuesX)
{
	double *pTheta;
	double *pX;
	double h_Answser;
	int n;
	int Index;

	/*
	*  t0*x0 + t1*x1 + ...
	*/

	n = Matrix_GetNumCols(hValuesX);

	pTheta = Matrix_GetDirectIndex(Thetha);
	pX = Matrix_GetDirectIndex(hValuesX);

	h_Answser = 0.0;

	for (Index = 0; Index < n; Index++)
	{
		h_Answser += pTheta[Index] * pX[Index];
	}


	return h_Answser;
}

/*********************************************************
*
*  LinearRegression_Internal_Gradient_Flat
*
*
*
*********************************************************/
double LinearRegression_Internal_Gradient_Flat(HMATRIX Thetha, HMATRIX hValuesX, HMATRIX hValuesY, int JIndex, double LearningRate)
{
	double NewTheta;
	int m;
	int n;
	int Index;
	int Index2;
	double *pX;
	double *pY;
	double *pTheta;
	double h_Answser;
	double J_Loop;

   /*
	*   tj = tj - a(1/m) E (h(x)-y)xj
	*/
	m = Matrix_GetNumRows(hValuesX);
	n = Matrix_GetNumCols(hValuesX);

	pTheta = Matrix_GetDirectIndex(Thetha);
	pX = Matrix_GetDirectIndex(hValuesX);
	pY = Matrix_GetDirectIndex(hValuesY);

	J_Loop = 0;
	for (Index = 0; Index < m; Index++)
	{
		h_Answser = 0.0;

		for (Index2 = 0; Index2 < n; Index2++)
		{
			h_Answser += pTheta[Index2] * pX[Index*n + Index2];
		}
		h_Answser = h_Answser - pY[Index];
		h_Answser = h_Answser*pX[Index*n + JIndex];
		J_Loop += h_Answser;
	}

	NewTheta = pTheta[JIndex] - ((LearningRate)*(1.0 / (double)m))*J_Loop;

	return NewTheta;
}

