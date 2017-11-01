#ifndef MATRIX_H
#define MATRIX_H

#include "vector"

#include "random"


using namespace std;


/*Матриця цілих чисел.*/
template<typename Type>
class Matrix
{
public:
    /*Конструктор за замовченням.*/
    Matrix(){
        matrix = vector< vector<Type>* >();
        rowCount = 0;
        columnCount = 0;
    }

    /*Конструктор з параметрами.*/
    Matrix(const unsigned short int newRowCount,
           const unsigned short int newColumnCount,
           const vector< vector<Type>* >* newMatrix = NULL){

        matrix = vector< vector<Type>* >();
        rowCount = newRowCount;
        columnCount = newColumnCount;

        if(NULL == newMatrix){
            for(unsigned short int row = 0; row < rowCount; row++){

                matrix.push_back( (new vector<Type>()));

                for(unsigned short int column = 0; column < columnCount; column++){
                    matrix[row]->push_back((Type)0);
                }

                //matrix[row]->shrink_to_fit();
            }
        }else{
            for(unsigned short int row = 0; row < rowCount; row++){

                matrix.push_back(new vector<Type>());

                for(unsigned short int column = 0; column < columnCount; column++){
                    matrix[row]->push_back((*newMatrix)[row]->at(column));
                }

                //matrix[row]->shrink_to_fit();
            }
        }

        //matrix.shrink_to_fit();
    }

    /*Конструктор копіювання.*/
    Matrix(const Matrix& other){
        matrix = vector< vector<Type>* >();
        rowCount = other.rowCount;
        columnCount = other.rowCount;

        for(unsigned short int row = 0; row < rowCount; row++){

            matrix.push_back(new vector<Type>());

            for(unsigned short int column = 0; column < columnCount; column++){
                matrix[row].push_back(other.matrix[row][column]);
            }

            matrix[row].shrink_to_fit();
        }

        matrix.shrink_to_fit();
    }

    /*Деструктор.*/
    ~Matrix(){
        for(unsigned short int row = 0; row < rowCount; row++){
            delete matrix[row];
        }
    }

    /*Заповнює матрицю випадковими числами.*/
    void GenerateRandom(const unsigned short int filling){
        for(unsigned short int row = 0; row < rowCount; row++){
            for(unsigned short int column = 0; column < columnCount; column++){
                unsigned short int r = rand() % (101);
                if(r < filling){
                    matrix[row]->at(column) = 1;
                }else{
                    matrix[row]->at(column) = 0;
                }

            }
        }
    }

    /*Міняє місцями два рядки матриці, задані їх номерами.*/
    void ExchangeRows(const unsigned short int a,
                      const unsigned short int b){
        if(a == b){
            return;
        }
        if(IsValidRowNumber(a) && IsValidRowNumber(b)){
            vector<Type>* temp = matrix[a];
            matrix[a] = matrix[b];
            matrix[b] = temp;
        }
    }

    /*Міняє місцями два стовпці матриці, задані їх номерами.*/
    void ExchangeColumns(const unsigned short int a,
                         const unsigned short int b){
        if(a == b){
            return;
        }
        if(IsValidColumnNumber(a) && IsValidColumnNumber(b)){
            for(unsigned short int row = 0; row < rowCount; row++){
                Type temp = matrix[row]->at(a);
                matrix[row]->at(a) = matrix[row]->at(b);
                matrix[row]->at(b) = temp;
            }
        }
    }

    /*Повертає суму елементів у рядку, заданому його номером.*/
    Type GetSumInRow(const unsigned short int rowNumber) const{
        Type ret = 0;
        if(!IsValidRowNumber(rowNumber)){
            return ret;
        }

        for(unsigned short int column = 0; column < columnCount; column++){
            ret += matrix[rowNumber]->at(column);
        }

        return ret;
    }

    /*Повертає суму елементів у стовпці, заданому його номером.*/
    Type GetSumInColumn(const unsigned short int colNumber) const{
        Type ret = 0;
        if(!IsValidColumnNumber(colNumber)){
            return ret;
        }

        for(unsigned short int row = 0; row < rowCount; row++){
            ret += matrix[row]->at(colNumber);
        }

        return ret;
    }

    /*Повертає елемент, що знаходиться на визначеному місці в матриці.*/
    Type GetAt(const unsigned short int row,
               const unsigned short int column) const{
        if(IsValidRowNumber(row) && IsValidColumnNumber(column)){
            return matrix[row]->at(column);
        }

        return 0;
    }

    /*Встановлює нове значення елементу, що знаходиться на визначеному місці в матриці.*/
    void SetAt(const unsigned short int row,
               const unsigned short int column,
               Type& newValue){
        if(IsValidRowNumber(row) && IsValidColumnNumber(column)){
            matrix[row]->at(column) = newValue;
        }
    }

    /*Чи вказує дане число на рядок матриці?*/
    bool IsValidRowNumber(unsigned short int rowNumber) const{
        return (rowNumber >= 0) && (rowNumber < rowCount);
    }

    /*Чи вказує дане число на стовпець матриці?*/
    bool IsValidColumnNumber(unsigned short int columnNumber) const{
        return (columnNumber >= 0) && (columnNumber< columnCount);
    }

    /*Повертає число рядків у матриці.*/
    unsigned short int GetRowCount() const{
        return rowCount;
    }

    /*Повертає число стовпців у матриці.*/
    unsigned short int GetColumnCount() const{
        return columnCount;
    }

private:
    vector< vector<Type>* > matrix;

    unsigned short int rowCount;

    unsigned short int columnCount;
};

#endif // MATRIX_H
