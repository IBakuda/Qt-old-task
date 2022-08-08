#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MyModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_action_Save_triggered();

    void on_action_Load_triggered();

    void on_pushButton_Add_clicked();

    void on_pushButton_Remove_clicked();

private:
    Ui::MainWindow *ui;
    MyModel * _model;
};
#endif // MAINWINDOW_H
