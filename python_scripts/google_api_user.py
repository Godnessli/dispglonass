import os.path
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
import aiohttp
from excel_reader import ExcelReader


class GoogleSheetsApiUser:
    SCOPES = ['https://www.googleapis.com/auth/spreadsheets']
    URL = 'https://sheets.googleapis.com/v4/spreadsheets'

    def __init__(self):
        self.token: str = self.get_token()
        self.id_sheets: dict = ExcelReader('A:/Projects/dispglonass/python_scripts/routes.xlsx').get_data()

    def get_token(self):
        """ Получет API токен."""

        creds = None
        if os.path.exists('A:/Projects/dispglonass/python_scripts/token.json'):
            creds = Credentials.from_authorized_user_file('A:/Projects/dispglonass/python_scripts/token.json', self.SCOPES)
        if not creds or not creds.valid:
            if creds and creds.expired and creds.refresh_token:
                creds.refresh(Request())
            else:
                flow = InstalledAppFlow.from_client_secrets_file('A:/Projects/dispglonass/python_scripts/credentials.json', self.SCOPES)
                creds = flow.run_local_server(port=0)
            with open('A:/Projects/dispglonass/python_scripts/token.json', 'w') as token:
                token.write(creds.to_json())

        return creds.token

    async def get_values(self, spreadsheet_id, ranges: str):
        """Делает get-запрос и получает двумерный список значений ячеек таблицы.

            Аргументы:
                spreadsheet_id(str): идентификатор таблицы,
                ranges(str): нужный диапазон формата <Название листа>!<Ячейка 1><Ячейка 2>
        """

        req_url = self.URL + f'/{spreadsheet_id}/values/{ranges}'
        async with aiohttp.ClientSession() as session:
            async with session.get(req_url, headers={'Authorization': 'Bearer ' + self.token}) as response:
                return await response.json()


