#include "getacts.h"

GetActs::GetActs()
{
}

std::string inline GetActs::today_month()
{
    QDate today = QDate::currentDate();
    std::string month;

    switch (today.month()) {
    case 1:
        month = "Январь,Февраль,Март";
        break;
    case 2:
        month = "Январь,Февраль,Март";
        break;
    case 3:
        month = "Январь,Февраль,Март";
        break;
    case 4:
        month = "Апрель, Май, Июнь";
        break;
    case 5:
        month = "Апрель, Май, Июнь";
        break;
    case 6:
        month = "Апрель, Май, Июнь";
        break;
    case 7:
        month = "Июль, Август, Сентябрь";
        break;
    case 8:
        month = "Июль, Август, Сентябрь";
        break;
    case 9:
        month = "Июль, Август, Сентябрь";
        break;
    case 10:
        month = "Октябрь, Ноябрь, Декабрь";
        break;
    case 11:
        month = "Октябрь, Ноябрь, Декабрь";
        break;
    case 12:
        month = "Октябрь, Ноябрь, Декабрь";
        break;
    default:
        break;
    }

    return month;
}

std::string GetActs::name_of_spreadsheet(const std::string month, const int route)
{
    return "№" + std::to_string(route) + " " + month + " " + std::to_string(QDate::currentDate().year()).erase(0, 1);
}

void GetActs::download_spreadsheet()
{

}
