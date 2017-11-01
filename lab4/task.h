#ifndef TASK
#define TASK

#include "matrix.h"

/*Повертає номер стовпця з максимальною сумою елементів для тих стовпців, де в 0-му рядку стоїть 1.*/
unsigned short int FindColumnWithMaximumSummForRow(const Matrix<unsigned short int>& matrix,
                                                const unsigned short int row,
                                                const unsigned short int firstColumn);

/*Повертає номер рядка з мінімальною сумою елементів.*/
unsigned short FindRowWithMinimumSumm(const Matrix<unsigned short> &matrix,
                                      const unsigned short int firstRow);

/*Виконує сортування Симоненка.*/
void DoTask(Matrix<unsigned short int>& matrix);

#endif // TASK

