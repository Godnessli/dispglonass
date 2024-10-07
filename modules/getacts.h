#ifndef GETACTS_H
#define GETACTS_H

#include <vector>
#include <QDate>
#include <vector>
#include <iostream>
#include <thread>
#include <boost/python.hpp>
#include <QMessageBox>

class GetActs
{
public:
    GetActs();
    ~GetActs();

    std::string inline today_month();
    std::string name_of_spreadsheet(const std::string);
    void download_spreadsheet(const std::vector<std::string> *arrayOfRoutes);

};

#endif // GETACTS_H
