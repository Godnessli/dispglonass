#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <boost/python.hpp>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private:
    Q_SLOT void open_make_act();
    Q_SLOT void open_lunina_report();
    Q_SLOT void open_settings();

private:
    Ui::MainMenu *ui;
};
#endif // MAINMENU_H

//2,2,2,2,2,2,10,10,10,20,23,23,23,214,423
