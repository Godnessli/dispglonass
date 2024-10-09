#include "readtransportworktable.h"

ReadTransportWorkTable::ReadTransportWorkTable()
{
}

std::string ReadTransportWorkTable::get_filepath_from_user()
{
    QWidget* fileDialogWidget   = new QWidget;

    std::string filepath        =  QFileDialog::getOpenFileName(fileDialogWidget, "Выберите файл", QCoreApplication::applicationDirPath(), "Файл Excel(*.xlsx)").toStdString();

    delete fileDialogWidget;
    fileDialogWidget = nullptr;

    return filepath;
}

std::vector<std::vector<std::string>> ReadTransportWorkTable::table_data(const std::string &path, const std::vector<std::string>& routesList)
{
    std::vector<std::vector<std::string>> tableData;
    xlnt::workbook transportWorkBook;
    transportWorkBook.load(path);

    xlnt::worksheet transportWorkSheet = transportWorkBook.active_sheet();

    const unsigned dateCol          = 0;
    const unsigned routeCol         = 6;
    const unsigned planCol          = 10;
    const unsigned factCol          = 11;
    const unsigned unexutedRacesCol = 13;
    int rowNum = 0;

    for(auto row : transportWorkSheet.rows(false))
    {
        rowNum++;
        std::vector<std::string> transportWorkData;

        for(int i = 0; i < routesList.size(); ++i)
        {
            if(row[routeCol].to_string() == routesList[i])
            {
                std::string date = QDate(1900, 1, 1).addDays(std::stoi(row[dateCol].to_string()) - 2).toString("dd.MM.yyyy").toStdString();
                std::cout << date << "\t";
                std::cout << row[routeCol].to_string() << "\t";
                std::cout << row[planCol].to_string() << "\t";
                std::cout << row[factCol].to_string() << "\t";
                std::cout << row[unexutedRacesCol].to_string() << std::endl;

                transportWorkData.push_back(date);
                transportWorkData.push_back(row[routeCol].to_string());
                transportWorkData.push_back(row[planCol].to_string() );
                transportWorkData.push_back(row[factCol].to_string());
                transportWorkData.push_back(row[unexutedRacesCol].to_string());
                tableData.push_back(transportWorkData);
            }
        }
    }

    return tableData;
}
