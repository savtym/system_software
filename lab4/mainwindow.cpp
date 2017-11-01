#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    matrix = new Matrix<unsigned short int>(5, 5, NULL);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete matrix;
}

void MainWindow::on_generateSource_clicked()
{
    matrix->GenerateRandom(ui->filling->value());
    ui->sourceMatrixView->setText(MatrixToString());
}

void MainWindow::on_getResult_clicked()
{
    DoTask(*matrix);
    ui->resultMatrixView->setText(MatrixToString());
}
