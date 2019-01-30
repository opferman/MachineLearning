/* 
 *  Toby Opferman
 *
 *    Vector and Matrix Library
 *
 */

#include <windows.h>
#include <math.h>
#include <matrix.h>

typedef struct _MATRIX_INTERNAL {

	double *pMatrixValues;
	int Rows;
	int Cols;

} MATRIX_INTERNAL, *PMATRIX_INTERNAL;

/*********************************************************
 *
 *  Matrix_Create
 *
 *
 *
 *********************************************************/
HMATRIX Matrix_Create(int rows, int cols, double InitialValue)
{
	PMATRIX_INTERNAL pMatrixInternal;
	int Index;

	pMatrixInternal = LocalAlloc(LMEM_ZEROINIT, sizeof(MATRIX_INTERNAL));

	if (pMatrixInternal != NULL)
	{
		pMatrixInternal->Cols = cols;
		pMatrixInternal->Rows = rows;

		pMatrixInternal->pMatrixValues = (double *)LocalAlloc(LMEM_ZEROINIT, sizeof(double)*cols*rows);

		if (pMatrixInternal->pMatrixValues == NULL)
		{
			LocalFree(pMatrixInternal);
			pMatrixInternal = NULL;
		}	
		else
		{
			if (InitialValue != 0.0)
			{
				for (Index = 0; Index < rows*cols; Index++)
				{
					pMatrixInternal->pMatrixValues[Index] = InitialValue;
				}
			}
		}
	}

	return (HMATRIX)pMatrixInternal;
}

/*********************************************************
*
*  Matrix_CreateFullMatrix
*
*
*
*********************************************************/
HMATRIX Matrix_CreateFullMatrix(int rows, int cols, double *pMatrixValues)
{
	PMATRIX_INTERNAL pMatrixInternal;
	int Index;

	pMatrixInternal = Matrix_Create(rows, cols, 0.0);

	if (pMatrixInternal)
	{
		for (Index = 0; Index < rows*cols; Index++)
		{
			pMatrixInternal->pMatrixValues[Index] = pMatrixValues[Index];
		}
	}

	return (HMATRIX)pMatrixInternal;
}


/*********************************************************
*
*  Matrix_CreateCopy
*
*
*
*********************************************************/
HMATRIX Matrix_CreateCopy(HMATRIX hMatrix)
{
	PMATRIX_INTERNAL pMatrixInternal;
	PMATRIX_INTERNAL pMatrixInternalCopy;
	int Index;

	pMatrixInternal = (PMATRIX_INTERNAL)hMatrix;

	pMatrixInternalCopy = Matrix_Create(pMatrixInternal->Rows, pMatrixInternal->Cols, 0.0);

	if (pMatrixInternalCopy)
	{
		for (Index = 0; Index < pMatrixInternal->Rows*pMatrixInternal->Cols; Index++)
		{
			pMatrixInternalCopy->pMatrixValues[Index] = pMatrixInternal->pMatrixValues[Index];
		}
	}

	return (HMATRIX)pMatrixInternalCopy;
}


/*********************************************************
*
*  Matrix_CreateIdenticalSize
*
*
*
*********************************************************/
HMATRIX Matrix_CreateIdenticalSize(HMATRIX hMatrix, double InitialValue)
{
	PMATRIX_INTERNAL pMatrixInternal;
	PMATRIX_INTERNAL pMatrixInternalCopy;

	pMatrixInternal = (PMATRIX_INTERNAL)hMatrix;

	pMatrixInternalCopy = Matrix_Create(pMatrixInternal->Rows, pMatrixInternal->Cols, InitialValue);

	return (HMATRIX)pMatrixInternalCopy;
}


/*********************************************************
*
*  Matrix_GetNumCols
*
*
*
*********************************************************/
int Matrix_GetNumCols(HMATRIX hMatrix)
{
	return ((PMATRIX_INTERNAL)hMatrix)->Cols;
}


/*********************************************************
*
*  Matrix_GetNumRows
*
*
*
*********************************************************/
int Matrix_GetNumRows(HMATRIX hMatrix)
{
	return ((PMATRIX_INTERNAL)hMatrix)->Rows;
}


/*********************************************************
*
*  Matrix_Transpose
*
*
*
*********************************************************/
MATRIX_STATUS Matrix_Transpose(HMATRIX hMatrix)
{
	PMATRIX_INTERNAL pMatrixInternal = (PMATRIX_INTERNAL)hMatrix;
	PMATRIX_INTERNAL pMatrixInternalCopy;
	int Swap;
	double SwapDouble;
	int RowIndex, ColIndex;
	HMATRIX hCopyTempMatrix;
	MATRIX_STATUS MatrixStatus = MATRIX_SUCCESS;
	double *pDoublePointerSwap;

	/*
	 * Vectors or 1xn matrices can be swapped just by swapping the rows by cols
	 */
	if (pMatrixInternal->Rows != 1 && pMatrixInternal->Cols != 1)
	{
		/*
		 * Handle the nxn case.
		 */
		if (pMatrixInternal->Rows == pMatrixInternal->Cols)
		{
			for (RowIndex = 0; RowIndex < pMatrixInternal->Rows; RowIndex++)
			{
				for (ColIndex = RowIndex + 1; ColIndex < pMatrixInternal->Cols; ColIndex++)
				{
					SwapDouble = pMatrixInternal->pMatrixValues[RowIndex*pMatrixInternal->Cols + ColIndex];
					pMatrixInternal->pMatrixValues[RowIndex*pMatrixInternal->Cols + ColIndex] = pMatrixInternal->pMatrixValues[ColIndex*pMatrixInternal->Rows + RowIndex];
					pMatrixInternal->pMatrixValues[ColIndex*pMatrixInternal->Rows + RowIndex] = SwapDouble;
				}
			}
		}
		else
		{
			/*
			 * Easy Hack, but slow.
			 */
			pMatrixInternalCopy = Matrix_Create(pMatrixInternal->Cols, pMatrixInternal->Rows, 0.0);
			if (pMatrixInternalCopy)
			{
				for(ColIndex = 0; ColIndex < pMatrixInternal->Rows; ColIndex++)
				{
					for (RowIndex = 0; RowIndex < pMatrixInternal->Cols; RowIndex++)
					{
						pMatrixInternalCopy->pMatrixValues[RowIndex*pMatrixInternal->Rows + ColIndex] = pMatrixInternal->pMatrixValues[ColIndex*pMatrixInternal->Cols + RowIndex];
					}
                }
				
				pDoublePointerSwap                 = pMatrixInternal->pMatrixValues;
				pMatrixInternal->pMatrixValues     = pMatrixInternalCopy->pMatrixValues;
				pMatrixInternalCopy->pMatrixValues = pDoublePointerSwap;

				Matrix_Destroy(pMatrixInternalCopy);
			}
			else
			{
				MatrixStatus = MATRIX_ERROR;
			}
		}
	}

	/*
	 * Reverse the mxn Matrix
	 */
	Swap = pMatrixInternal->Rows;
	pMatrixInternal->Rows = pMatrixInternal->Cols;
	pMatrixInternal->Cols = Swap;

	return MATRIX_SUCCESS;
}



/*********************************************************
*
*  Matrix_FastSwap
*
*
*
*********************************************************/
MATRIX_STATUS Matrix_FastSwap(HMATRIX hMatrixA, HMATRIX hMatrixB)
{
	MATRIX_STATUS MatrixStatus = MATRIX_ERROR;
	PMATRIX_INTERNAL pMatrixInternalA = (PMATRIX_INTERNAL)hMatrixA;
	PMATRIX_INTERNAL pMatrixInternalB = (PMATRIX_INTERNAL)hMatrixB;
	double *pSwap;

	if ((pMatrixInternalA->Cols == pMatrixInternalB->Cols) && (pMatrixInternalA->Rows == pMatrixInternalB->Rows))
	{
		pSwap = pMatrixInternalA->pMatrixValues;
		pMatrixInternalA->pMatrixValues = pMatrixInternalB->pMatrixValues;
		pMatrixInternalB->pMatrixValues = pSwap;
	}

	return MatrixStatus;
}

/*********************************************************
*
*  Matrix_Multiply
*
*
*
*********************************************************/
MATRIX_STATUS Matrix_Multiply(PMATRIX_MATH_PARAMS pMatrixMathParams)
{
	MATRIX_STATUS MatrixStatus;

	MatrixStatus = MATRIX_SUCCESS;

	return MatrixStatus;
}

/*********************************************************
*
*  Matrix_Divide
*
*
*
*********************************************************/
MATRIX_STATUS Matrix_Divide(PMATRIX_MATH_PARAMS pMatrixMathParams)
{
	MATRIX_STATUS MatrixStatus;

	MatrixStatus = MATRIX_SUCCESS;

	return MatrixStatus;
}


/*********************************************************
*
*  Matrix_Add
*
*
*
*********************************************************/
MATRIX_STATUS Matrix_Add(PMATRIX_MATH_PARAMS pMatrixMathParams)
{
	MATRIX_STATUS MatrixStatus;

	MatrixStatus = MATRIX_SUCCESS;

	return MatrixStatus;
}


/*********************************************************
*
*  Matrix_Subtract
*
*
*
*********************************************************/
MATRIX_STATUS Matrix_Subtract(PMATRIX_MATH_PARAMS pMatrixMathParams)
{
	MATRIX_STATUS MatrixStatus;

	MatrixStatus = MATRIX_SUCCESS;

	return MatrixStatus;
}

/*********************************************************
*
*  Matrix_Power
*
*
*
*********************************************************/
MATRIX_STATUS Matrix_Power(PMATRIX_MATH_PARAMS pMatrixMathParams)
{
	MATRIX_STATUS MatrixStatus;

	MatrixStatus = MATRIX_SUCCESS;

	return MatrixStatus;
}


/*********************************************************
*
*  Matrix_Destroy
*
*
*
*********************************************************/
void Matrix_Destroy(HMATRIX hMatrix)
{
	PMATRIX_INTERNAL pMatrixInternal = (PMATRIX_INTERNAL)hMatrix;

	LocalFree(pMatrixInternal->pMatrixValues);
	LocalFree(pMatrixInternal);
}

/*********************************************************
*
*  Matrix_GetDirectIndex
*
*
*
*********************************************************/
double * Matrix_GetDirectIndex(HMATRIX hMatrix)
{
	return ((PMATRIX_INTERNAL)hMatrix)->pMatrixValues;
}