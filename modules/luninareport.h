#ifndef LUNINAREPORT_H
#define LUNINAREPORT_H

#include <QDialog>

namespace Ui {
class LuninaReport;
}

class LuninaReport : public QDialog
{
    Q_OBJECT

public:
    explicit LuninaReport(QWidget *parent = nullptr);
    ~LuninaReport();

private:
    Q_SLOT void increment();
    Q_SLOT void decrement();

private:
    Ui::LuninaReport *ui;
};

#endif // LUNINAREPORT_H
