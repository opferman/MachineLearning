/*
*  Toby Opferman
*
*    Matrix Test
*/


#include <windows.h>
#include <stdio.h>
#include <matrix.h>


double MatrixNumbersList[] =
{
	0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.5, 12.6, 13.7, 14.9, 15.9, 16.7, 18.8, 20.0, 21.0, 25.9, 44.4, 40.0, 50.0, 431.3, 56.5, 455.2, 443.2
};


double MatrixNumbersList2[] =
{
	0.1, 0.2, 0.3
};

/* 
 * Internal Prototypes
 */
void MatrixTest_TransposeTest(void);
void MatrixTest_DisplayMatrix(HMATRIX hMatrix);

/*
 * Main()
 */
int main(void)
{
	MatrixTest_TransposeTest();

	return 0;
}

/*
* MatrixTest_TransposeTest
*/
void MatrixTest_TransposeTest(void)
{
	HMATRIX hMatrix;
	UINT Index;
	UINT Index2;

	printf("Square Matrices\n");

	for (Index = 2; Index < 7; Index++)
	{
		hMatrix = Matrix_CreateFullMatrix(Index, Index, MatrixNumbersList);

		if (hMatrix)
		{
			printf("Before Transpose\n");
			MatrixTest_DisplayMatrix(hMatrix);

			if (Matrix_Transpose(hMatrix) == MATRIX_SUCCESS)
			{
				printf("After Transpose\n");
				MatrixTest_DisplayMatrix(hMatrix);
			}

			Matrix_Destroy(hMatrix);
		}
	}
	printf("Non Square Matrices\n");

	for (Index = 2; Index < 7; Index++)
	{
		for (Index2 = Index+1; Index2 < 7; Index2++)
		{
			hMatrix = Matrix_CreateFullMatrix(Index, Index2, MatrixNumbersList);

			if (hMatrix)
			{
				printf("Before Transpose\n");
				MatrixTest_DisplayMatrix(hMatrix);

				if (Matrix_Transpose(hMatrix) == MATRIX_SUCCESS)
				{
					printf("After Transpose\n");
					MatrixTest_DisplayMatrix(hMatrix);
				}

				Matrix_Destroy(hMatrix);
			}

			hMatrix = Matrix_CreateFullMatrix(Index2, Index, MatrixNumbersList);

			if (hMatrix)
			{
				printf("Before Transpose\n");
				MatrixTest_DisplayMatrix(hMatrix);

				if (Matrix_Transpose(hMatrix) == MATRIX_SUCCESS)
				{
					printf("After Transpose\n");
					MatrixTest_DisplayMatrix(hMatrix);
				}

				Matrix_Destroy(hMatrix);
			}
		}
	}

}

/*
* MatrixTest_DisplayMatrix
*/
void MatrixTest_DisplayMatrix(HMATRIX hMatrix)
{
	int Row;
	int Col;
	int RowIndex;
	int ColIndex;
	double *pNumbers;

	Row = Matrix_GetNumRows(hMatrix);
	Col = Matrix_GetNumCols(hMatrix);
	pNumbers = Matrix_GetDirectIndex(hMatrix);

	for (RowIndex = 0; RowIndex < Row; RowIndex++)
	{
		for (ColIndex = 0; ColIndex < Col; ColIndex++)
		{
			printf("%1.2f ", (float)pNumbers[RowIndex*Col + ColIndex]);
		}
		printf("\n");
	}
}