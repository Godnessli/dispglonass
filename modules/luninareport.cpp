#include "luninareport.h"
#include "ui_luninareport.h"

LuninaReport::LuninaReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LuninaReport)
{
    ui->setupUi(this);

    connect(ui -> pushButton, &QPushButton::clicked, this, &LuninaReport::increment);
    connect(ui -> pushButton_2, &QPushButton::clicked, this, &LuninaReport::decrement);
}

LuninaReport::~LuninaReport()
{
    delete ui;
}

void LuninaReport::increment()
{
    ui -> label_2 -> setText(QString::number(ui -> label_2 -> text().toInt() + 1));
}

void LuninaReport::decrement()
{
    ui -> label_2 -> setText(QString::number(ui -> label_2 -> text().toInt() - 1));
}
