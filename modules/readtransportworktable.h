#ifndef READTRANSPORTWORKTABLE_H
#define READTRANSPORTWORKTABLE_H

#include <iostream>
#include <QWidget>
#include <QFileDialog>
#include <QCoreApplication>
#include <xlnt/xlnt.hpp>

class ReadTransportWorkTable
{
public:
    ReadTransportWorkTable();

    std::string                             get_filepath_from_user();
    std::vector<std::vector<std::string>>   table_data(const std::string &, const std::vector<std::string> &);

};

#endif // READTRANSPORTWORKTABLE_H
