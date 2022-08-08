#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>



#include "MyModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _model = new MyModel(this);

    ui->tableView->setModel(_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Save_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save File",".xml","XML (*.xml)");
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return;
    QString xml = _model->serializeXML();
    file.write(xml.toLatin1());
    file.close();
}

void MainWindow::on_action_Load_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Select file");
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QByteArray data = file.readAll();
    _model->deserializeXML(data);
    file.close();
}

void MainWindow::on_pushButton_Add_clicked()
{
    QString Check = _model->checkBeforeAdd(ui->lineEdit_ID->text(),ui->lineEdit_TEXT->text());
    if(Check == "OK")
    {
        _model->addPerson(ui->lineEdit_ID->text().toInt(), ui->lineEdit_TEXT->text());
        ui->lineEdit_ID->setText("");
        ui->lineEdit_TEXT->setText("");
    }

    else if(Check == "IDNotInt"){QMessageBox::warning(this,"Внимание","ID не число");}
    else if(Check == "EmptyID"){QMessageBox::warning(this,"Внимание","ID пуст");}
    else if(Check == "EmptyTEXT"){QMessageBox::warning(this,"Внимание","TEXT пуст");}
}

void MainWindow::on_pushButton_Remove_clicked()
{

    bool check = _model->checkBeforeRemove(ui->tableView->currentIndex().row());

    if (check == true)
        _model->removePerson(ui->tableView->currentIndex().row());
    else
        QMessageBox::warning(this,"Внимание","Выберите элемент для удаления");
}
