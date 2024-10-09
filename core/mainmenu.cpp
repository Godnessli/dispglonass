#include "mainmenu.h"
#include "makeact.h"
#include "settings.h"
#include "luninareport.h"
#include "readtransportworktable.h"
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
    MakeAct *m = new MakeAct;
    m -> show();
}

void MainMenu::open_lunina_report()
{
    ReadTransportWorkTable *rtwt = new ReadTransportWorkTable;

    std::string filepath = rtwt -> get_filepath_from_user();
    std::vector<std::string> routes;
    routes.push_back("34");
    routes.push_back("55");
    routes.push_back("396А");
    routes.push_back("370");

    std::vector<std::vector<std::string>> twVector = rtwt -> table_data(filepath, routes);

    qDebug() << twVector;
}

void MainMenu::open_settings()
{
    static Settings *s = new Settings;
    s -> show();
}
