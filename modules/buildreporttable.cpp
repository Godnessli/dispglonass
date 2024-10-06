#include "buildreporttable.h"

BuildReportTable::BuildReportTable(const std::vector<std::vector<std::vector<std::string>>>& routesData, const std::string routes_names[])
{
    xlnt::workbook inputWorkBook;

    xlnt::worksheet inputWorkSheet = inputWorkBook.active_sheet();
    inputWorkSheet.title("Акты по Маршрутам");

    int count_rows = QDate(2024, 06, 1).daysInMonth();

    inputWorkSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(1, 1), xlnt::cell_reference(routesData[0][0].size() - 1, 1)));
    inputWorkSheet.cell(xlnt::cell_reference(1, 1)).value(routes_names[0]);
    inputWorkSheet.cell(1, 1).alignment(xlnt::alignment().horizontal(xlnt::horizontal_alignment::center));

    int row_modifier = 2;

    for(int route = 0; route < routesData.size(); ++route)
    {
        for(int row = (count_rows * route + row_modifier); row <= (count_rows * (route + 1) + row_modifier - 1); ++row)
        {
            for(int col = 1; col < routesData[route][0].size(); ++col)
            {
                inputWorkSheet.cell(xlnt::cell_reference(col, row)).value(routesData[route][row - (count_rows * route + row_modifier)][col - 1]);
            }

            if(row == (count_rows * (route + 1) + row_modifier - 1))
            {
                row++;
                row_modifier++;

                inputWorkSheet.merge_cells(xlnt::range_reference(xlnt::cell_reference(1, row), xlnt::cell_reference(routesData[route][0].size() - 1, row)));
                if(route + 1 == routesData.size())
                    inputWorkSheet.cell(xlnt::cell_reference(1, row)).value("");
                else
                    inputWorkSheet.cell(xlnt::cell_reference(1, row)).value(routes_names[route + 1]);
                inputWorkSheet.cell(1, row).alignment(xlnt::alignment().horizontal(xlnt::horizontal_alignment::center));
            }
        }
    }

    inputWorkBook.save("Актовый отчет.xlsx");
}
