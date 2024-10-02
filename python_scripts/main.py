import asyncio
from google_api_user import GoogleSheetsApiUser
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
        id_sheets, data = ExcelReader('routes.xlsx').get_data(), dict()
        api_user = GoogleSheetsApiUser()
        tasks = [api_user.get_values(id_sheets[route_numb], period) for route_numb in lst_routes]
        lst_reports = await asyncio.gather(*tasks)

        for i, report in enumerate(lst_reports):
            route_numb = lst_routes[i]
            data[route_numb] = report['values']

        with open('data.json', 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=4)

    @classmethod
    def run(cls):
        start = time()
        asyncio.run(cls.get_org_reports([271, 30, 4, 5, 176, 119, 371, 246,
                                     394, 392, 383, 58, 43, 66, 117, 239,
                                     380, 384, 487, 540, 247, 163, 23, 52, 247, 18, 164, 188, '343Э', 358], 'Июнь 24'))

        return f'Акты скачаны за {round(time() - start, 1)} сек'






