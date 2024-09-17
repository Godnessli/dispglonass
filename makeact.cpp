#include "makeact.h"
#include "ui_makeact.h"
#include <iostream>
#include <sstream>

MakeAct::MakeAct(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakeAct)
{
    ui->setupUi(this);

    connect(ui -> pushButton, &QPushButton::clicked, this, &MakeAct::increment);
}

MakeAct::~MakeAct()
{
    delete ui;
}

void MakeAct::increment()
{
    ui -> label_2 -> setText(QString::number(ui -> label_2 -> text().toInt() + 1));
}
