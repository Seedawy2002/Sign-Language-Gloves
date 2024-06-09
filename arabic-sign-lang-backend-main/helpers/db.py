import pyrebase
import os
from dotenv import load_dotenv

load_dotenv()

firebaseConfig={
    "apiKey": os.getenv('FIREBASE_API_KEY', ""),
    "authDomain": os.getenv('FIREBASE_AUTH_DOMAIN' ,""),
    "databaseURL": os.getenv('FIREBASE_DATABASE_URL', ""),
    "projectId": os.getenv('FIREBASE_PROJECT_ID', ""),
    "storageBucket": os.getenv('FIREBASE_STORAGE_BUCKET', ""),
}

firebase = pyrebase.initialize_app(firebaseConfig)

db = firebase.database()
