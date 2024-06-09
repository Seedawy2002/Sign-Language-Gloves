#!/bin/python3
import cv2
import os
from dotenv import load_dotenv
from gtts import gTTS
from helpers.db import db
import pandas as pd
import pickle
from ultralytics import YOLO

load_dotenv()
model = YOLO(os.getenv('MODEL_PATH'))

def say(text, speech_file_path):
    tts = gTTS(text=text, lang='ar')

    # Save the audio to a file
    tts.save(speech_file_path)


mapping = {'ain':'ع',
           'al':'ال',
           'aleff':'ا',
           'bb':'ب',
           'dal':'د',
           'dha':'ظ',
           'dhad':'ض',
           'fa':'ف',
           'gaaf':'ق',
           'ghain':'غ',
           'ha':'ه',
           'haa':'ه',
           'jeem':'ج',
           'kaaf':'ك',
           'khaa':'خ',
           'la':'لا',
           'laam':'ل',
           'meem':'م',
           'nun':'ن',
           'ra':'ر',
           'saad':'ص',
           'seen':'س',
           'sheen':'ش',
           'ta':'ت',
           'taa':'ط',
           'thaa':'ث',
           'thal':'ذ',
           'toot':'ة',
           'waw':'و',
           'ya':'ي',
           'yaa':'ي',
           'zay':'ز'}

###############
# Hand
###############
int_to_char ={0: 'ا',
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
 28: ' '}

# Load the scaler and the model
with open(os.getenv('SCALER_PATH'), 'rb') as scaler_file:
    scaler = pickle.load(scaler_file)

with open(os.getenv('MODEL2_PATH'), 'rb') as model_file:
    loaded_model = pickle.load(model_file)

# image_file = './imgs/photo.jpg'

# Main function
def predict_img_main(image_file="./imgs/new_photo.jpg"):
    if image_file != None and image_file != ' ':
        # Convert to OpenCV format
        frame = cv2.imread(image_file)
        if frame is None:
            return "NFRM"
        try:
            results = model.track(frame)
        except Exception as e:
            # Reload
            model2 = YOLO(os.getenv('MODEL_PATH'))
            frame = cv2.imread(image_file)
            results = model2.track(frame)
            # return "ERR"
        stri = results[0].verbose()
        if (stri != "(no detections), "):
            words = stri.split()
            predicted_class = mapping[words[1][:-1]]
            return predicted_class
        else:
            return ""


def predict_hand():
    db_dict = db.child('esp-stream').child("gyros").get().val()[0]
    input_dict = {
        # 'gyro_x': db_dict['x'],
        # 'gyro_y': db_dict['y'],
        # 'gyro_z': db_dict['z'],
        'flex1': db_dict['flex1'],
        'flex2': db_dict['flex2'],
        'flex3': db_dict['flex3'],
        'flex4': db_dict['flex4'],
        'flex5': db_dict['flex5'],
        'contact1': db_dict['contact1'],
        'contact2': db_dict['contact2'],
        'contact3': db_dict['contact3']
    }

    # Convert the dictionary to a DataFrame
    input_df = pd.DataFrame([input_dict])
    # Select features
    important_features = ['flex1',  'flex2',    'flex3',    'flex4',
    'flex5',    'contact1', 'contact2', 'contact3']
    input_df = input_df[important_features]

    # Scale the flex sensors readings
    columns_to_scale = [col for col in input_df.columns if not
    col.startswith('contact')]
    input_df[columns_to_scale] = scaler.transform(input_df[columns_to_scale])

    prediction = loaded_model.predict(input_df)

    return int_to_char[prediction[0]]
