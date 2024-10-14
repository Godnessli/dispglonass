#include "buildreporttable.h"

BuildReportTable::BuildReportTable(const std::vector<std::vector<std::vector<std::string>>>& routesData, const std::vector<std::vector<std::string>>& routesVector)
{
    xlnt::workbook inputWorkBook;
    xlnt::worksheet inputWorkSheet = inputWorkBook.active_sheet();
    inputWorkSheet.title("Акты по Маршрутам");

    int count_rows = QDate(2024, 9, 1).daysInMonth();

    inputWorkSheet.cell(xlnt::cell_reference(1, 1)).value("Дата");
    inputWorkSheet.cell(xlnt::cell_reference(2, 1)).value("Маршрут");
    inputWorkSheet.cell(xlnt::cell_reference(3, 1)).value("План ОРГП");
    inputWorkSheet.cell(xlnt::cell_reference(4, 1)).value("План Вест-Сервис");
    inputWorkSheet.cell(xlnt::cell_reference(5, 1)).value("Факт ОРГП");
    inputWorkSheet.cell(xlnt::cell_reference(6, 1)).value("Факт Вест-Сервис");
    inputWorkSheet.cell(xlnt::cell_reference(7, 1)).value("Срывы Вест-Сервис");
    inputWorkSheet.cell(xlnt::cell_reference(8, 1)).value("Срывы Эксп.");
    inputWorkSheet.cell(xlnt::cell_reference(9, 1)).value("Срывы Тех.");
    inputWorkSheet.cell(xlnt::cell_reference(10, 1)).value("ДТП");
    inputWorkSheet.cell(xlnt::cell_reference(11, 1)).value("Отклонения");
    inputWorkSheet.cell(xlnt::cell_reference(12, 1)).value("Несоблюдение времени");
    inputWorkSheet.cell(xlnt::cell_reference(13, 1)).value("БО");
    inputWorkSheet.cell(xlnt::cell_reference(14, 1)).value("Другие срывы");

    for(int route = 0; route < routesData.size(); ++route)
    {
        for(int row = (routesData[route].size() * route + 2); row <= (routesData[route].size() * (route + 1) + 1); ++row)
        {
            int colModifier = 0;
            for(int col = 1; col <= routesData[route][0].size() + 3; ++col)
            {
                if((col != 4) && (col != 6) && (col != 7))
                {
                    inputWorkSheet.cell(xlnt::cell_reference(col, row)).value(routesData[route][row - (count_rows * route + 2)][col - 1 - colModifier]);
                    inputWorkSheet.cell(col, row).alignment(xlnt::alignment().horizontal(xlnt::horizontal_alignment::center));
                }
                else if(col == 4)
                {
                    colModifier++;
                    for(int i = 0; i < routesVector.size(); ++i)
                    {
                        if((routesVector[i][1] == inputWorkSheet.cell(2, row).to_string()) && (routesVector[i][0] == inputWorkSheet.cell(1, row).to_string()))
                        {
                            inputWorkSheet.cell(xlnt::cell_reference(4, row)).value(routesVector[i][2]);
                            inputWorkSheet.cell(xlnt::cell_reference(6, row)).value(routesVector[i][3]);
                            inputWorkSheet.cell(xlnt::cell_reference(7, row)).value(routesVector[i][4]);
                        }
                    }
                }
                else if(col == 6)
                    colModifier++;
                else if(col == 7)
                    colModifier++;

                if(col > 9)
                {
                    try {
                        if(std::stoi(inputWorkSheet.cell(col, row).to_string()) > 10)
                        {
                            inputWorkSheet.cell(col, row).fill(xlnt::fill::solid(xlnt::rgb_color("b50808")));
                            inputWorkSheet.cell(col, row).font(xlnt::font().color(xlnt::color::white()));
                            inputWorkSheet.cell(col, row).font(xlnt::font().name("Calibri"));
                        }
                        else if(std::stoi(inputWorkSheet.cell(col, row).to_string()) > 5)
                            inputWorkSheet.cell(col, row).fill(xlnt::fill::solid(xlnt::rgb_color("f1b64e")));
                    } catch (std::exception) {
                        continue;
                    }
                }
            }
        }
    }

    inputWorkBook.save("Актовый отчет.xlsx");
}
