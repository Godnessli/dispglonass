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
    connect(ui -> pushButton_3, &QPushButton::clicked, this, &MainMenu::greet_from_python);
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
    static LuninaReport *l = new LuninaReport;
    l -> show();
}

void MainMenu::open_settings()
{
    static Settings *s = new Settings;
    s -> show();
}

void MainMenu::greet_from_python()
{
    Py_Initialize();

    try {
        boost::python::object main = boost::python::import("__main__");
        boost::python::object dict = main.attr("__dict__");
        std::string script = "def jam():  \n"
                             "  return 10 * NUM";
        script = script.replace(script.length() - 3, script.length() - 1, std::to_string(6345));

        std::cout << script << std::endl;

        boost::python::object result = boost::python::exec(script.c_str(), dict, dict);
        boost::python::object returned = dict["jam"];

        int res = boost::python::extract<int>(returned());

        std::cout << res;
    } catch (boost::python::error_already_set) {
        PyErr_Print();
    }

    Py_Finalize();
}
