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

std::vector<std::vector<std::string>> ReadTransportWorkTable::table_data(const std::string &path, const std::vector<std::string> &routesVector)
{
    std::vector<std::vector<std::string>> tableData;
    xlnt::workbook transportWorkBook;
    transportWorkBook.load(path);

    xlnt::worksheet transportWorkSheet = transportWorkBook.active_sheet();

    unsigned dateCol          = 0;
    unsigned routeCol         = 0;
    unsigned planCol          = 0;
    unsigned factCol          = 0;
    unsigned unexutedRacesCol = 0;

    for(auto col : transportWorkSheet.rows(false)[0])
    {
        if(col.to_string() == "Дата выпуска")
            dateCol = col.column().index - 1;
        else if(col.to_string() == "№ Маршрута договор")
            routeCol = col.column().index - 1;
        else if(col.to_string() == "План рейсов")
            planCol = col.column().index - 1;
        else if(col.to_string() == "Факт рейсов")
            factCol = col.column().index - 1;
        else if(col.to_string() == "Сорванные рейсы")
            unexutedRacesCol = col.column().index - 1;
    }

    int rowNum = 0;

    for(auto row : transportWorkSheet.rows(false))
    {
        rowNum++;
        std::vector<std::string> transportWorkData;

        for(int i = 0; i < routesVector.size(); ++i)
        {
            if(row[routeCol].to_string() == routesVector[i])
            {
                std::string date = QDate(1900, 1, 1).addDays(std::stoi(row[dateCol].to_string()) - 2).toString("dd.MM.yyyy").toStdString();

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
