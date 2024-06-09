import requests
from django.conf import settings


def get_firebase_data():
    url = f"{settings.FIREBASE_DATABASE_URL}/{settings.FIREBASE_PATH_TO_DATA}.json?auth={settings.FIREBASE_API_KEY}"
    response = requests.get(url)
    if response.status_code == 200:
        return response.json()
    else:
        response.raise_for_status()


columns = ['flex1', 'flex2', 'flex3', 'flex4', 'flex5',
           'contact1', 'contact2', 'contact3']

columns_to_scale = [col for col in columns if not col.startswith('contact')]

int_to_char = {
    0: 'ا',
    1: 'ب',
    2: 'ت',
    3: 'ث',
    4: 'ج',
    5: 'ح',
    6: 'خ',
    7: 'د',
    8: 'ذ',
    9: 'ر',
    10: 'ز',
    11: 'س',
    12: 'ش',
    13: 'ص',
    14: 'ض',
    15: 'ط',
    16: 'ظ',
    17: 'ع',
    18: 'غ',
    19: 'ف',
    20: 'ق',
    21: 'ك',
    22: 'ل',
    23: 'م',
    24: 'ن',
    25: 'ه',
    26: 'و',
    27: 'ي',
    28: ' ',
}
