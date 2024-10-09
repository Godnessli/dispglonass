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
        month = "Сентябрь"; // Важно!!! Поменять обратно на Октябрь, сделано только для теста!
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
    std::string run_script = "import sys\n";

    #ifdef __MINGW32__
        run_script +=   "sys.path.insert(1, 'A:/projects/dispglonass/python_scripts')\n"
                        "sys.path.insert(1, 'A:/usr/Python312/Lib/site-packages')\n";
    #endif

    #ifdef __linux__
        run_script +=   "sys.path.insert(1, '/home/godnessli/projects/dispglonass/python_scripts')\n"
                        "sys.path.insert(1, '/home/godnessli/.local/lib/python3.12/site-packages')\n";
    #endif

    run_script +=   "import main\n"
                    "obj = main.ReportReader\n"
                    "def run():\n"
                    "  return obj.run([";

    std::string routes;

    for(int i = 0; i < arrayOfRoutes -> size(); ++i)
    {
        if(i == arrayOfRoutes -> size() - 1)
        {
            bool digit = true;
            for(int j = 0; j < arrayOfRoutes -> at(i).size(); ++j)
            {
                if(!std::isdigit(arrayOfRoutes -> at(i)[j]))
                {
                    digit = false;
                    break;
                }
            }

            if(digit)
                routes += arrayOfRoutes -> at(i);
            else
                routes += "'" + arrayOfRoutes -> at(i) + "'";
        }
        else
        {
            bool digit = true;
            qDebug() << arrayOfRoutes -> at(i).size();
            for(int j = 0; j < arrayOfRoutes -> at(i).size(); ++j)
            {
                if(!std::isdigit(arrayOfRoutes -> at(i)[j]))
                {
                    digit = false;
                    break;
                }
            }

            if(digit)
                routes += arrayOfRoutes -> at(i) + ",";
            else
                routes += "'" + arrayOfRoutes -> at(i) + "'" + ",";
        }
    }

    run_script += routes + "], '" + name_of_spreadsheet(today_month()) + "')";

    qDebug() << QString::fromStdString(run_script);

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
