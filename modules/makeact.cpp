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
    connect(ui -> makeReport, &QPushButton::clicked, this, &MakeAct::start_make_report);
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

boost::json::value read_json(std::istream& is, boost::system::error_code& ec)
{
    boost::json::stream_parser p;
    std::string line;

    while(std::getline(is, line))
    {
        p.write(line, ec);
        if(ec)
            return nullptr;
    }

    p.finish(ec);
    if(ec)
        return nullptr;
    return p.release();
}

void MakeAct::start_make_report()
{
    for(int i = 0; i < ui -> routeList -> count(); ++i)
    {
        make_route_report(ui -> routeList -> item(i) -> text().toStdString());

    }
}

std::vector<std::vector<std::string>> MakeAct::make_route_report(const std::string& route)
{
    std::ifstream inputJson("A:/Projects/dispglonass/python_scripts/data.json");
    boost::json::value routesDataJson;
    std::vector<std::vector<std::string>> reportTableModel;

    if(!inputJson.is_open())
        std::cerr << "Error openning the file!" << std::endl;
    else
    {
        inputJson >> routesDataJson;
        int planRacesPerDay, factRacesPerDay;
        QDate dateOfRaces = QDate(2024, 06, 1);
        for(int i = 3; i < routesDataJson.as_object()[route].as_array().size(); ++i)
        {
            if(routesDataJson.as_object()[route].as_array()[i].as_array().size() > 0)
            {
                if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[0].as_string().c_str()) == dateOfRaces.toString("dd.MM.yyyy"))
                {
                    qDebug() << i;
                    std::vector<std::string> dayReport;
                    try {
                        planRacesPerDay = std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[1].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[4].as_string().c_str());
                        factRacesPerDay = std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[2].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[5].as_string().c_str());

                        dayReport.push_back(dateOfRaces.toString("dd.MM.yyyy").toStdString());
                        dayReport.push_back(std::to_string(planRacesPerDay));
                        dayReport.push_back(std::to_string(factRacesPerDay));

                        int exploitationsReason = 0;
                        int technicalReason = 0;
                        int accidentReason = 0;
                        int deviationReason = 0;
                        int otherReason = 0;

                        while(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[0].as_string().c_str()) == dateOfRaces.toString("dd.MM.yyyy"))
                        {
                            if(planRacesPerDay == factRacesPerDay)
                                break;
                            else
                            {
                                if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[12].as_string().c_str()) == QString("Э"))
                                    if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                        exploitationsReason++;
                                    else
                                        exploitationsReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                else if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[12].as_string().c_str()) == QString("Т"))
                                    if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                        technicalReason++;
                                    else
                                        technicalReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                else if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[12].as_string().c_str()) == QString("ДТП"))
                                    if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                        accidentReason++;
                                    else
                                        accidentReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                else if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[14].as_string().c_str()) == QString("Несоблюдение времени начала/окончания движения"))
                                    if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                        deviationReason++;
                                    else
                                        deviationReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                else
                                    if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                        otherReason++;
                                    else
                                        otherReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());

                                ++i;

                                if(routesDataJson.as_object()[route].as_array()[i].as_array().size() < 4)
                                    break;
                            }
                        }

                        --i;

                        dayReport.push_back(std::to_string(exploitationsReason));
                        dayReport.push_back(std::to_string(technicalReason));
                        dayReport.push_back(std::to_string(accidentReason));
                        dayReport.push_back(std::to_string(deviationReason));
                        dayReport.push_back(std::to_string(otherReason));

                    } catch(std::exception) {
                        dayReport.push_back("-");
                        dayReport.push_back("-");
                        dayReport.push_back("-");
                        dayReport.push_back("-");
                        dayReport.push_back("-");
                        dayReport.push_back("-");
                        dayReport.push_back("-");
                        dayReport.push_back("-");
                    }

                    reportTableModel.push_back(dayReport);
                    dateOfRaces = dateOfRaces.addDays(1);
                }
            }
        }
    }

    return reportTableModel;
}

void MakeAct::exit()
{
    MakeAct::~MakeAct();
}
