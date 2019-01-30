/*
* Linear Regression Library
*
*   Toby Opferman
*/


#ifndef __LINEAR_REGRESSION_H__
#define __LINEAR_REGRESSION_H__

typedef PVOID HLINEARREG;

typedef enum _LINEAR_EQUATION_TYPE {
	LINEAR_FLAT,     /* theta + theta1*x + theta2*x2 + ... */
	EXP_INCREASING,  /* theta + theta1*x^2 + theta2*x2^3 + ... */
	EXP_SQUARE       /* theta + theta1*x^2 + theta2*x2^2 + ... */

} LINEAR_EQUATION_TYPE, *PLINEAR_EQUATION_TYPE;

typedef enum _LINEAR_STATUS {

	LINEAR_SUCCESS,
	LINEAR_ERROR,
	LINEAR_UNIMPLEMENTED

} LINEAR_STATUS, *PLINEAR_STATUS;

typedef struct _LINEAR_CREATE {

	LINEAR_EQUATION_TYPE LinearEquationType;
	int NumberOfFeatures;
	HMATRIX hTheta;
	UINT TrainingItterations;
} LINEAR_CREATE, *PLINEAR_CREATE;

HLINEARREG LinearRegression_Create(PLINEAR_CREATE pLinearCreate);

LINEAR_STATUS LinearRegression_Training_GradientDescent(HLINEARREG hLinearReg, HMATRIX hTrainingData, HMATRIX hResultsVector, double LearningRate);
LINEAR_STATUS LinearRegression_Training_NormalEquation(HLINEARREG hLinearReg, HMATRIX hTrainingData, HMATRIX hResultsVector);

double LinearRegression_Predict(HLINEARREG hLinearReg, HMATRIX hInputData);

void LinearRegression_Destroy(HLINEARREG hLinearReg);

#endif

