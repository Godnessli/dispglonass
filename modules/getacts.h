#ifndef GETACTS_H
#define GETACTS_H

#include <vector>
#include <QDate>

class GetActs
{
public:
    GetActs();
    ~GetActs();

    std::string inline today_month();
    std::string name_of_spreadsheet(const std::string, const int);
    void download_spreadsheet();
};

#endif // GETACTS_H
