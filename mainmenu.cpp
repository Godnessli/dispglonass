#include "mainmenu.h"
#include "makeact.h"
#include "settings.h"
#include "luninareport.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    connect(ui -> pushButton, &QPushButton::clicked, this, &MainMenu::open_make_act);
    connect(ui -> pushButton_2, &QPushButton::clicked, this, &MainMenu::open_lunina_report);
    connect(ui -> pushButton_4, &QPushButton::clicked, this, &MainMenu::open_settings);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::open_make_act()
{
    static MakeAct *m = new MakeAct;
    m -> show();
}

void MainMenu::open_lunina_report()
{
    static LuninaReport *l = new LuninaReport;
    l -> show();
}

void MainMenu::open_settings()
{
    static Settings *s = new Settings;
    s -> show();
}
