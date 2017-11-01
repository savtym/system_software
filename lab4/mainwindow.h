#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "matrix.h"
#include "task.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_generateSource_clicked();

    void on_getResult_clicked();

private:
    Ui::MainWindow *ui;

    Matrix<unsigned short int>* matrix;

    QString MatrixToString(){
        QString ret = "";

        for(unsigned short int row = 0; row < matrix->GetRowCount(); row++){
            for(unsigned short int column = 0; column < matrix->GetColumnCount(); column++){
                ret.append(QString::fromStdString(to_string(matrix->GetAt(row, column))));
                ret.append("\t");
            }
            ret.append("\n");
        }

        return ret;
    }
};

#endif // MAINWINDOW_H
