/*
 * Matrix Library
 *
 *   Toby Opferman
 */


#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef PVOID HMATRIX;

#define MATRIX_INPUT_FLAG_OPERAND_A_MATRIX   1
#define MATRIX_INPUT_FLAG_OPERAND_B_MATRIX   2
#define MATRIX_INPUT_FLAG_OUTPUT_UNALLOCATED 4

#define MATRIX_OUTPUT_MATRIX 1
#define MATRIX_INDEX (m, r, c, maxc) (m[r*maxc + c])

typedef struct _MATRIX_MATH_PARAMS {

    union {    
       HMATRIX hMatrixA;
       double NumberA;
    } OperandA;

    union {
       HMATRIX hMatrixB;
       double NumberB;
    } OperandB;

    union {
       HMATRIX hMatrix;
       double Number;
    } Output;

    ULONG64 MatrixInputFlags;
    ULONG64 MatrixOutputFlags;

} MATRIX_MATH_PARAMS, *PMATRIX_MATH_PARAMS;

typedef enum _MATRIX_STATUS {
  MATRIX_SUCCESS,
  MATRIX_ERROR,
  MATRIX_MISMATCH
} MATRIX_STATUS, *PMATRIX_STATUS;

HMATRIX Matrix_Create(int rows, int cols, double InitialValue);
HMATRIX Matrix_CreateCopy(HMATRIX hMatrix);
HMATRIX Matrix_CreateIdenticalSize(HMATRIX hMatrix, double InitialValue);
HMATRIX Matrix_CreateFullMatrix(int rows, int cols, double *pMatrixValues);

MATRIX_STATUS Matrix_FastSwap(HMATRIX hMatrixA, HMATRIX hMatrixB);

double * Matrix_GetDirectIndex(HMATRIX hMatrix);
int Matrix_GetNumCols(HMATRIX hMatrix);
int Matrix_GetNumRows(HMATRIX hMatrix);
MATRIX_STATUS Matrix_Transpose(HMATRIX hMatrix);

MATRIX_STATUS Matrix_Multiply(PMATRIX_MATH_PARAMS pMatrixMathParams);
MATRIX_STATUS Matrix_Divide(PMATRIX_MATH_PARAMS pMatrixMathParams);
MATRIX_STATUS Matrix_Add(PMATRIX_MATH_PARAMS pMatrixMathParams);
MATRIX_STATUS Matrix_Subtract(PMATRIX_MATH_PARAMS pMatrixMathParams);
MATRIX_STATUS Matrix_Power(PMATRIX_MATH_PARAMS pMatrixMathParams);

void Matrix_Destroy(HMATRIX hMatrix);


#endif

