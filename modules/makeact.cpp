#include "makeact.h"
#include "ui_makeact.h"
#include "getacts.h"
#include "buildreporttable.h"

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
        ui -> routeList -> addItem(routesFile -> value("route").toString());
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

    std::vector<std::string> routes;
    std::string routeNumber;

    std::stringstream ss(route -> text().toStdString());

    while(getline(ss, routeNumber, ',')) {
        routes.push_back(routeNumber);
    }

    for(int i = 0; i < routes.size(); ++i)
    {
        routesFile -> setArrayIndex(i);
        routesFile -> setValue("route", QString::fromStdString(routes[i]));
    }

    for(auto routeNumber : routes)
        ui -> routeList -> addItem(QString::fromStdString(routeNumber));

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

    std::vector<std::string> routes;

    for(int i = 0; i < ui -> routeList -> count(); ++i)
    {
        routes.push_back(ui -> routeList -> item(i) -> text().toStdString());
    }

    std::sort(routes.begin(), routes.end(), std::less<std::string>());

    routes.erase(std::unique(routes.begin(), routes.end()), routes.end());

    ui -> routeList -> clear();

    for(auto routeNumber : routes)
    {
        ui -> routeList -> addItem(QString::fromStdString(routeNumber).toUpper().replace(" ", ""));
    }

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
    std::vector<std::vector<std::vector<std::string>>> tableData;
    std::string routes[ui -> routeList -> count()];
    for(int i = 0; i < ui -> routeList -> count(); ++i)
    {
        tableData.push_back(make_route_report(ui -> routeList -> item(i) -> text().toStdString()));
        routes[i] = ui -> routeList -> item(i) -> text().toStdString();
        qDebug() << routes[i];
    }


    BuildReportTable *brt = new BuildReportTable(tableData, routes);
    delete brt;
    brt = nullptr;
}

std::vector<std::vector<std::string>> MakeAct::make_route_report(const std::string& route)
{
    try {

        #ifdef __MINGW32__
            std::ifstream inputJson("data.json");
        #endif
        #ifdef __linux__
            std::ifstream inputJson("data.json");
        #endif

        boost::json::value routesDataJson;
        std::vector<std::vector<std::string>> reportTableModel;

        if(!inputJson.is_open())
            std::cerr << "Error openning the file!" << std::endl;
        else
        {
            inputJson >> routesDataJson;
            int planRacesPerDay, factRacesPerDay;
            QDate dateOfRaces = QDate(2024, 6, 1);
            if(routesDataJson.as_object()[route].as_array()[routesDataJson.as_object()[route].as_array().size() / 2].as_array().size() > 0)
            {
                for(int i = 3; i < routesDataJson.as_object()[route].as_array().size() - 1; ++i)
                {
                    qDebug() << i;
                    if(routesDataJson.as_object()[route].as_array()[i].as_array().size() > 0)
                    {
                        if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[0].as_string().c_str()) == dateOfRaces.toString("dd.MM.yyyy"))
                        {
                            qDebug() << "check 2";
                            std::vector<std::string> dayReport;
                            try {
                                if(routesDataJson.as_object()[route].as_array()[i].as_array().size() < 4)
                                {
                                    planRacesPerDay = std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[1].as_string().c_str());
                                    factRacesPerDay = std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[2].as_string().c_str());
                                }
                                else
                                {
                                    if(routesDataJson.as_object()[route].as_array()[i].as_array()[4].as_string().size() < 1)
                                    {
                                        planRacesPerDay = std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[1].as_string().c_str());
                                        factRacesPerDay = std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[2].as_string().c_str());
                                    }
                                    else
                                    {
                                        planRacesPerDay = std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[1].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[4].as_string().c_str());
                                        factRacesPerDay = std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[2].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[5].as_string().c_str());
                                    }
                                }

                                dayReport.push_back(dateOfRaces.toString("dd.MM.yyyy").toStdString());
                                dayReport.push_back(route);
                                dayReport.push_back(std::to_string(planRacesPerDay));
                                dayReport.push_back(std::to_string(factRacesPerDay));

                                int exploitationsReason = 0;
                                int technicalReason = 0;
                                int accidentReason = 0;
                                int deviationReason = 0;
                                int deviationTimeReason = 0;
                                int glonassError = 0;
                                int otherReason = 0;

                                while(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[0].as_string().c_str()) == dateOfRaces.toString("dd.MM.yyyy"))
                                {
                                    if(routesDataJson.as_object()[route].as_array()[i].as_array().size() > 12)
                                    {
                                        if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[12].as_string().c_str()) == QString("Э"))
                                        {
                                            if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty() && QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                exploitationsReason++;
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                                exploitationsReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str());
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                exploitationsReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                            else
                                                exploitationsReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                        }
                                        else if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[12].as_string().c_str()) == QString("Т"))
                                        {
                                            if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty() && QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                technicalReason++;
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                                technicalReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str());
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                technicalReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                            else
                                                technicalReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                        }
                                        else if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[12].as_string().c_str()) == QString("ДТП"))
                                        {
                                            if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty() && QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                accidentReason++;
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                                accidentReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str());
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                accidentReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                            else
                                                accidentReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                        }
                                        else if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[14].as_string().c_str()) == QString("Проезд ОП (>20%)") ||
                                                   QString(routesDataJson.as_object()[route].as_array()[i].as_array()[14].as_string().c_str()) == QString("Пропуск остановочных пунктов") ||
                                                   QString(routesDataJson.as_object()[route].as_array()[i].as_array()[14].as_string().c_str()) == QString("Несоблюдение трассы маршрута"))
                                        {
                                            if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty() && QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                deviationReason++;
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                                deviationReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str());
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                deviationReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                            else
                                                deviationReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                        }
                                        else if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[14].as_string().c_str()) == QString("Несоблюдение времени начала/окончания движения") ||
                                                 QString(routesDataJson.as_object()[route].as_array()[i].as_array()[14].as_string().c_str()) == QString("Несоблюдение времени отправления"))
                                        {
                                            if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty() && QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                deviationTimeReason++;
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                                deviationTimeReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str());
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                deviationTimeReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                            else
                                                deviationTimeReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                        }
                                        else if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[12].as_string().c_str()) == QString("БО"))
                                        {
                                            if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty() && QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                glonassError++;
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                                glonassError += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str());
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                glonassError += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                            else
                                                glonassError += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                        }
                                        else
                                        {
                                            if(QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty() && QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                otherReason++;
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()).isEmpty())
                                                otherReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str());
                                            else if(!QString(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str()).isEmpty())
                                                otherReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                            else
                                                otherReason += std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[15].as_string().c_str()) + std::stoi(routesDataJson.as_object()[route].as_array()[i].as_array()[16].as_string().c_str());
                                        }
                                    }

                                    ++i;

                                    if(routesDataJson.as_object()[route].as_array()[i].as_array().size() < 4)
                                        break;
                                }

                                --i;

                                dayReport.push_back(std::to_string(exploitationsReason));
                                dayReport.push_back(std::to_string(technicalReason));
                                dayReport.push_back(std::to_string(accidentReason));
                                dayReport.push_back(std::to_string(deviationReason));
                                dayReport.push_back(std::to_string(deviationTimeReason));
                                dayReport.push_back(std::to_string(glonassError));
                                dayReport.push_back(std::to_string(otherReason));

                            } catch(std::exception) {
                                dayReport.push_back(dateOfRaces.toString("dd.MM.yyyy").toStdString());
                                dayReport.push_back(route);
                                dayReport.push_back("-");
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
                        else
                            continue;
                    }
                }
            }
            else
            {
                for(int i = 0; i < dateOfRaces.daysInMonth(); ++i)
                {
                    std::vector<std::string> dayReport;
                    dayReport.push_back(dateOfRaces.toString("dd.MM.yyyy").toStdString());
                    dayReport.push_back(route);
                    dayReport.push_back("-");
                    dayReport.push_back("-");
                    dayReport.push_back("-");
                    dayReport.push_back("-");
                    dayReport.push_back("-");
                    dayReport.push_back("-");
                    dayReport.push_back("-");
                    dayReport.push_back("-");
                    dayReport.push_back("-");
                    reportTableModel.push_back(dayReport);
                    dateOfRaces = dateOfRaces.addDays(1);
                }
            }
        }

        return reportTableModel;

    } catch (const std::ifstream::failure& e) {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Ошибка!",
                                                                       tr("Рейсы не скачаны!\nПожалуйста, скачайте рейсы и попробуйте снова."),
                                                                       QMessageBox::Ok | QMessageBox::Cancel);
    }
}

void MakeAct::exit()
{
    MakeAct::~MakeAct();
}
