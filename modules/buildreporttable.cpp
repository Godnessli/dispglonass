#include "buildreporttable.h"

BuildReportTable::BuildReportTable(const std::vector<std::vector<std::vector<std::string>>>& routesData)
{
    xlnt::workbook inputWorkBook;
    std::string dest_filename = "Актовый отчет.xlsx";

    xlnt::worksheet inputWorkSheet = inputWorkBook.active_sheet();
    inputWorkSheet.title("Акты по Маршрутам");

    int buffer_count_rows;
    for(int route = 0; route < routesData.size(); ++route)
    {
        for(int row = 0; row < routesData[route].size(); ++row)
        {
            for(int col = 0; col < routesData[route][row].size(); ++col)
            {
                inputWorkSheet.cell(xlnt::cell_reference(col, row)).value(routesData[route][row][col]);
            }
        }
    }

    inputWorkBook.save(dest_filename);
}
