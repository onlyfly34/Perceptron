#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
    void on_pushButton_clicked();
    void init_plot();

private:
    Ui::MainWindow *ui;
    void read_training_data(QString filename);
    void start_training();

    double learning_rate;
    vector<vector<double>> training_data;
    double weights[3] = {-1,0,1};
    //double training_data[3] = {-1,0,0};
};

#endif // MAINWINDOW_H
