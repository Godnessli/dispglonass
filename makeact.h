#ifndef MAKEACT_H
#define MAKEACT_H

#include <QDialog>
#include <thread>

namespace Ui {
class MakeAct;
}

class MakeAct : public QDialog
{
    Q_OBJECT

public:
    explicit MakeAct(QWidget *parent = nullptr);
    ~MakeAct();

protected slots:
    void increment();

private:
    Ui::MakeAct *ui;
};

#endif // MAKEACT_H
