#include "MatrixDisplay.h"


void Matrix_Display(double matrix[3][3]) {
	
	printf("\n");
	printf("%-.2lf     %-.2lf     %-.2lf\n", matrix[0][0], matrix[0][1], matrix[0][2]);
	printf("\n");
	printf("%-.2lf     %-.2lf     %-.2lf\n", matrix[1][0], matrix[1][1], matrix[1][2]);
	printf("\n");
	printf("%-.2lf     %-.2lf     %-.2lf\n", matrix[2][0], matrix[2][1], matrix[2][2]);
	printf("\n");
}

/*
 * 			TO-DO
 * 			-----
 *  - Adjust for any size matrix (Check your python matrix mult code for what you did there)
 *
 *  - Test Code
 *  
 *  - 
 *  */