#include "makeact.h"
#include "ui_makeact.h"
#include "getacts.h"

MakeAct::MakeAct(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakeAct)
{
    ui->setupUi(this);

    GetActs *getacts = new GetActs;

    addRouteDialog -> setModal(true);
    addRouteDialog -> setLayout(addRouteLayout);
    addRouteLayout -> addWidget(route, 0, 0, 1, 2);
    addRouteLayout -> addWidget(accept, 1, 0);
    addRouteLayout -> addWidget(reject, 1, 1);

    QSettings *routesFile = new QSettings(QString(QCoreApplication::applicationDirPath() +
                                                  "/" + "routes.ini"), QSettings::IniFormat);
    routesFile -> beginGroup("Routes");
    int size = routesFile -> beginReadArray("GovernmentContract");

    for(int i = 0; i < size; ++i)
    {
        routesFile -> setArrayIndex(i);
        ui -> routeList -> addItem(QString::number(routesFile -> value("route").toInt()));
    }

    routesFile -> endArray();
    routesFile -> endGroup();

    delete routesFile;

    connect(ui -> addRoute, &QPushButton::clicked, this, &MakeAct::add_route_to_table);
    connect(ui -> removeRoute, &QPushButton::clicked, this, &MakeAct::remove_route_from_table);
    connect(ui -> makeReport, &QPushButton::clicked, this, &MakeAct::make_report);
    connect(ui -> downloadActs, &QPushButton::clicked, this, &MakeAct::download_act);
    connect(ui -> exit, &QPushButton::clicked, this, &MakeAct::exit);
}

MakeAct::~MakeAct()
{
    delete reject;
    reject = nullptr;
    delete accept;
    accept = nullptr;
    delete route;
    route = nullptr;
    delete addRouteLayout;
    addRouteLayout = nullptr;
    delete addRouteDialog;
    addRouteDialog = nullptr;
    delete ui;
}

void MakeAct::add_route_to_table()
{
    connect(accept, &QPushButton::clicked, this, &MakeAct::write_routes);
    addRouteDialog -> show();
}

void MakeAct::write_routes()
{
    QSettings *routesFile = new QSettings(QString(QCoreApplication::applicationDirPath() +
                                                    "/" + "routes.ini"), QSettings::IniFormat);
    routesFile -> beginGroup("Routes");
    routesFile -> beginWriteArray("GovernmentContract");

    std::vector<int> routes;
    std::string routeNumber;

    for(auto symbol : route -> text().toStdString())
    {
        if(symbol == ',')
        {
            routes.push_back(std::stoi(routeNumber));
            routeNumber = "";
        }
        else if(route -> text().toStdString().size() == 1 && symbol != ',')
        {
            routeNumber += symbol;
            routes.push_back(std::stoi(routeNumber));
            routeNumber = "";
        }
        else
            routeNumber += symbol;

        route -> setText(route -> text().removeFirst());
    }

    for(int i = 0; i < routes.size(); ++i)
    {
        routesFile -> setArrayIndex(i);
        routesFile -> setValue("route", routes[i]);
    }

    for(auto routeNumber : routes)
        ui -> routeList -> addItem(QString::number(routeNumber));

    update_list();

    addRouteDialog -> close();

    disconnect(accept, &QPushButton::clicked, this, &MakeAct::write_routes);
    delete routesFile;
}

void MakeAct::remove_route_from_table()
{
    ui -> routeList -> takeItem(ui -> routeList -> currentRow());
    update_list();
}

void inline MakeAct::update_list()
{
    QSettings *routesFile = new QSettings(QString(QCoreApplication::applicationDirPath() +
                                                  "/" + "routes.ini"), QSettings::IniFormat);
    routesFile -> beginGroup("Routes");
    routesFile -> beginWriteArray("GovernmentContract");

    std::vector<int> routes;

    for(int i = 0; i < ui -> routeList -> count(); ++i)
    {
        routes.push_back(ui -> routeList -> item(i) -> text().toInt());
    }

    std::sort(routes.begin(), routes.end(), std::less<int>());

    routes.erase(std::unique(routes.begin(), routes.end()), routes.end());

    ui -> routeList -> clear();

    for(auto routeNumber : routes)
        ui -> routeList -> addItem(QString::number(routeNumber));

    for(int i = 0; i < routes.size(); ++i)
    {
        routesFile -> setArrayIndex(i);
        routesFile -> setValue("route", ui -> routeList -> item(i) -> text());
    }

    routesFile -> endArray();
    routesFile -> endGroup();

    delete routesFile;
}

void MakeAct::download_act()
{
    std::vector<std::string> *routes_vector = new std::vector<std::string>;

    for(int i = 0; i < ui -> routeList -> count(); ++i)
    {
        routes_vector -> push_back(ui -> routeList -> item(i) -> text().toStdString());
    }

    GetActs *ga = new GetActs;

    ga -> download_spreadsheet(routes_vector);

    delete ga;
    ga = nullptr;
}

void MakeAct::make_report()
{
    std::ifstream routes_data("A:/Projects/dispglonass/python_scripts/data.json", std::ifstream::binary);
    Json::Value route;
    routes_data >> route;

    std::cout << route["34"];
}

void MakeAct::exit()
{
    MakeAct::~MakeAct();
}
