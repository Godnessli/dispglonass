import asyncio
from google_api_user import GoogleSheetsApiUser, DIR_PATH
import json
from excel_reader import ExcelReader
from time import time


class ReportReader:

    @staticmethod
    async def get_org_reports(lst_routes: list, period: str):
        """ Асинхронно получает список отчётов организатора и записывает в файл 'data.json'

            Аргументы:
                lst_routes(list): список номер маршрутов по договору,
                period(str): нужный период форамата <Месяц> <Последние две цифры года>

        """
        id_sheets, data = ExcelReader(DIR_PATH + '/' + 'routes.xlsx').get_data(), dict()
        api_user = GoogleSheetsApiUser()
        tasks = [api_user.get_values(id_sheets[route_numb], period) for route_numb in lst_routes]
        lst_reports = await asyncio.gather(*tasks)

        for i, report in enumerate(lst_reports):
            route_numb = lst_routes[i]
            data[route_numb] = report['values']

        with open(DIR_PATH + '/' + 'data.json', 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=4)

    @classmethod
    def run(cls, routes, month):
        start = time()
        asyncio.run(cls.get_org_reports(routes, month))

        return round(time() - start, 1)





