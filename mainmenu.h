#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void open_make_act();
    void open_lunina_report();
    void open_settings();

private:
    Ui::MainMenu *ui;
};
#endif // MAINMENU_H

//2,2,2,2,2,2,10,10,10,20,23,23,23,214,423
