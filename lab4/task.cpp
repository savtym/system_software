#include "matrix.h"


/*Повертає номер рядка з мінімальною сумою елементів.*/
unsigned short FindRowWithMinimumSumm(const Matrix<unsigned short> &matrix,
                                      const unsigned short int firstRow)
{
    unsigned short int ret = firstRow;

    unsigned short int summ = 0;// matrix.GetSumInRow(firstRow);
    for(unsigned short int c = firstRow; c < matrix.GetColumnCount(); c++){
        summ += matrix.GetAt(firstRow, c);
    }

    for(unsigned short int row = firstRow + 1; row < matrix.GetRowCount(); row++){
        unsigned short int rowSumm = 0;
        for(unsigned short int c = firstRow; c < matrix.GetRowCount(); c++){
            rowSumm += matrix.GetAt(row, c);
        }
        if(rowSumm < summ){
            ret = row;
            summ = rowSumm;
        }
    }

    return ret;
}

/*Повертає номер стовпця з максимальною сумою елементів для тих стовпців, де в 0-му рядку стоїть 1.*/
unsigned short FindColumnWithMaximumSummForRow(const Matrix<unsigned short> &matrix,
                                            const unsigned short int row,
                                            const unsigned short int firstColumn)
{
    unsigned short int ret = firstColumn;
    for(unsigned int c = firstColumn; c < matrix.GetColumnCount(); c++){
        if(matrix.GetAt(row, c) == 1){
            ret = c;
            break;
        }
    }

    unsigned short int summ = 0;// matrix.GetSumInColumn(ret);
    for(unsigned short int r = row; r < matrix.GetRowCount(); r++){
        summ += matrix.GetAt(r, ret);
    }


    for(unsigned short int column = ret + 1; column < matrix.GetColumnCount(); column ++){
        if(matrix.GetAt(row, column) == 1){
            unsigned short int columnSumm = 0;
            for(unsigned short int r = row; r < matrix.GetRowCount(); r++){
                columnSumm += matrix.GetAt(r, column);
            }
            if(columnSumm > summ){
                ret = column;
                summ = columnSumm;
            }
        }
    }

    return ret;
}

/*Виконує сортування Симоненка.*/
void DoTask(Matrix<unsigned short int>&matrix)
{
    unsigned int startIteration = 0;
    /*Спочатку треба знайти безумовні призначення.*/
    for(unsigned int iteration = 0; iteration < matrix.GetRowCount(); iteration++){
        /*Чи містить рядок тільки одну одиницю?*/
        unsigned int colOfOne = matrix.GetColumnCount() + 1;
        for(unsigned int col = 0; col < matrix.GetColumnCount(); col++){
            if(matrix.GetAt(iteration, col) == 1){
                if(colOfOne < matrix.GetColumnCount()){
                    colOfOne = matrix.GetColumnCount() + 1;
                    break;
                }
                colOfOne = col;
            }
        }

        /*Якщо містить тільки одну одиницю, то чи містить той стовпець тільки одну одиницю?*/
        if(colOfOne < matrix.GetColumnCount()){
            unsigned int rowOfOne = iteration;
            for(unsigned int row = 0; row < matrix.GetRowCount(); row++){
                if((matrix.GetAt(row, colOfOne) == 1) && (row != iteration)){
                    rowOfOne = row;
                    break;
                }
            }

            /*Якщо одиниця одна для стовпця і рядка, ми знайшли безумовне призначення.*/
            if(rowOfOne == iteration){
                matrix.ExchangeRows(startIteration, rowOfOne);
                matrix.ExchangeColumns(startIteration, colOfOne);
                startIteration++;
            }
        }
    }


    for(unsigned short int iteration = startIteration; iteration < matrix.GetRowCount(); iteration++){
        matrix.ExchangeRows(iteration,
                            FindRowWithMinimumSumm(matrix, iteration));

        matrix.ExchangeColumns(iteration,
                               FindColumnWithMaximumSummForRow(matrix, iteration, iteration));
    }
}


