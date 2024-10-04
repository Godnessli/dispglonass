#include "core/mainmenu.h"
#include <locale.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    QApplication a(argc, argv);
    MainMenu w;
    w.show();
    return a.exec();
}
