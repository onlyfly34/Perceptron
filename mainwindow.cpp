#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    //initial variables
    //learning_rate = 0.01;
    QString path = QDir::currentPath();
    qDebug() << path;
    ui->lineEdit->setText("感知機1.txt");
    ui->lineEdit_2->setText("0.08");
    ui->lineEdit_3->setText("0");
    ui->lineEdit_4->setText("1");
    init_plot();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::read_training_data(QString filename){
    ifstream inClientfile(filename.toStdString(),ios::in);
    qDebug() << filename;
    //exception handling
    if(inClientfile){
        qDebug() << "Successfully read the file!";
        for(int i=0;!inClientfile.eof();i++){
            double dimension1,dimension2,expected_value;
            inClientfile >> dimension1 >> dimension2 >> expected_value;
            training_data.push_back(vector<double>());
            training_data[i].push_back(-1);
            training_data[i].push_back(dimension1);
            training_data[i].push_back(dimension2);
            training_data[i].push_back(expected_value);
        }
    }
    else
        qDebug() << "Error in reading the file! The file may not exist";

    inClientfile.close();
}

void MainWindow::start_training(){
    //cdln = current data line number
    int round = 0;
    for(int cdln=0;(unsigned)cdln<training_data.size();cdln++){
        //qDebug() << "damn!";
        round++;
        double result = weights[0]*training_data[cdln][0]+weights[1]*training_data[cdln][1]+weights[2]*training_data[cdln][2];
        double smaller_margin_value = ui->lineEdit_3->text().toDouble();
        if(result > smaller_margin_value && training_data[cdln][3] > smaller_margin_value)
            continue;
        else if(result <= smaller_margin_value && training_data[cdln][3] <= smaller_margin_value)
            continue;
        else{
            if(result > smaller_margin_value){
                weights[0] = weights[0]-learning_rate*training_data[cdln][0];
                weights[1] = weights[1]-learning_rate*training_data[cdln][1];
                weights[2] = weights[2]-learning_rate*training_data[cdln][2];
            }
            else{
                weights[0] = weights[0]+learning_rate*training_data[cdln][0];
                weights[1] = weights[1]+learning_rate*training_data[cdln][1];
                weights[2] = weights[2]+learning_rate*training_data[cdln][2];
            }
            qDebug() << weights[0] << " " << weights[1] << " " << weights[2];
            cdln = -1;
        }
        if(round > 99999)
            break;
    }
    string convert = to_string(-weights[0]);
    QString x0 = QString::fromStdString(convert);
    convert = to_string(weights[1]);
    QString x1 = QString::fromStdString(convert);
    if(weights[1] > 0)
        x1 = "+"+x1;
    convert = to_string(weights[2]);
    QString x2 = QString::fromStdString(convert);
    if(weights[2] > 0)
        x2 = "+"+x2;
    ui->label_5->setText(x0+x1+"*X1"+x2+"*X2 = 0");
    ui->label_3->setText("Training rounds："+QString::fromStdString(to_string(round)));
    qDebug() << weights[0] << " " << weights[1] << " " << weights[2] << " " << round;
}

void MainWindow::on_pushButton_clicked(){
    QString filename = ui->lineEdit->text();
    QString learning_rate_buffer = ui->lineEdit_2->text();
    if(filename != "" && learning_rate_buffer != ""){
        ui->statusBar->showMessage("");
        learning_rate = learning_rate_buffer.toDouble();
        read_training_data(filename);
        start_training();
        ui->statusBar->showMessage("Finish training the perceptron.");
    }
    else if(filename == "" || learning_rate_buffer == ""){
        ui->statusBar->showMessage("Please fill out required fields.");
    }
}

void MainWindow::init_plot(){
    ui->customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
}
