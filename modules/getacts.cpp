#include "getacts.h"

GetActs::GetActs()
{
}

GetActs::~GetActs()
{
}

std::string inline GetActs::today_month()
{
    QDate today = QDate::currentDate();
    std::string month;

    switch (today.month()) {
    case 1:
        month = "Январь";
        break;
    case 2:
        month = "Февраль";
        break;
    case 3:
        month = "Март";
        break;
    case 4:
        month = "Апрель";
        break;
    case 5:
        month = "Май";
        break;
    case 6:
        month = "Июнь";
        break;
    case 7:
        month = "Июль";
        break;
    case 8:
        month = "Август";
        break;
    case 9:
        month = "Сентябрь";
        break;
    case 10:
        month = "Июнь"; // Важно!!! Поменять обратно на Октябрь, сделано только для теста!
        break;
    case 11:
        month = "Ноябрь";
        break;
    case 12:
        month = "Декабрь";
        break;
    default:
        break;
    }

    return month;
}

std::string GetActs::name_of_spreadsheet(const std::string month)
{
    return month + " " + std::to_string(QDate::currentDate().year()).erase(0, 2);
}

void GetActs::download_spreadsheet(const std::vector<std::string> *arrayOfRoutes)
{
    std::string run_script = "import sys\n"
                             "sys.path.insert(1, 'A:/projects/dispglonass/python_scripts')\n"
                             "sys.path.insert(1, 'A:/usr/Python312/Lib/site-packages')\n"
                             "import main\n"
                             "obj = main.ReportReader\n"
                             "def run():\n"
                             "  return obj.run([";

    std::string routes;

    for(int i = 0; i < arrayOfRoutes -> size(); ++i)
    {
        if(i == arrayOfRoutes -> size() - 1)
            routes += arrayOfRoutes -> at(i);
        else
            routes += arrayOfRoutes -> at(i) + ", ";
    }

    run_script += routes + "], '" + name_of_spreadsheet(today_month()) + "')";

    Py_Initialize();

    try {
        boost::python::object main          = boost::python::import("__main__");
        boost::python::object globals       = main.attr("__dict__");

        boost::python::object reportReader  = boost::python::exec(run_script.c_str(), globals, globals);
        boost::python::object run           = globals["run"];

        float message                 = boost::python::extract<float>(run());

        qDebug() << "Акты скачаны за " << message << " секунд\n";

    } catch (boost::python::error_already_set) {
        PyErr_Print();
    }
}
