from django.conf import settings
from rest_framework import views
from rest_framework.response import Response
from apps.ML_models.serializers import ModelDataSerializer
from apps.ML_models.utility import get_firebase_data, int_to_char, columns, columns_to_scale
import pickle
import numpy as np
import pandas as pd
import time
import google.generativeai as genai

with open('apps/ML_models/saved_models/scaler.pkl', 'rb') as scaler_file:
    scaler = pickle.load(scaler_file)

with open('apps/ML_models/saved_models/model.pkl', 'rb') as file:
    model = pickle.load(file)

genai.configure(api_key=settings.GOOGLE_GENAI_API_KEY)
genai_model = genai.GenerativeModel(
    model_name=settings.GOOGLE_GENAI_MODEL_NAME,
    system_instruction=settings.GOOGLE_GENAI_SYSTEM_INSTRUCTION
)
genai_chat = genai_model.start_chat(history=[])


class RealTimeModelAPIView(views.APIView):
    permission_classes = []
    authentication_classes = []

    def get(self, request):
        predictions = []

        request_time_in_seconds = time.time()
        delta = settings.TIME_IN_SECONDS_BETWEEN_SIGNS
        time_to_sleep_before_firebase = settings.TIME_TO_SLEEP_IN_SECONDS

        while True:
            time.sleep(time_to_sleep_before_firebase)
            response = get_firebase_data()
            data = response[0]

            data_serializer = ModelDataSerializer(data=data)
            data_serializer.is_valid(raise_exception=True)

            data = [float(value) for value in list(data_serializer.validated_data.values())]
            x = np.array([data])

            input_df = pd.DataFrame(x, columns=columns)

            input_df[columns_to_scale] = scaler.transform(input_df[columns_to_scale])

            prediction = model.predict(input_df)

            if int_to_char[prediction[0]] == ' ':
                break

            predictions.append(prediction[0])

            time_to_sleep = request_time_in_seconds + delta - time.time()

            time.sleep(time_to_sleep)
            request_time_in_seconds += delta

        predictions = [int_to_char[p] for p in predictions]
        word = ''.join(predictions)

        try:
            response = genai_chat.send_message(word)
            word = response.text[0:-2]
        except Exception as e:
            pass

        return Response(data={'word': word})


class ModelAPIView(views.APIView):
    permission_classes = []
    authentication_classes = []

    def get(self, request):
        response = get_firebase_data()
        data = response[0]

        data_serializer = ModelDataSerializer(data=data)
        data_serializer.is_valid(raise_exception=True)

        data = [float(value) for value in list(data_serializer.validated_data.values())]
        x = np.array([data])
        input_df = pd.DataFrame(x, columns=columns)

        input_df[columns_to_scale] = scaler.transform(input_df[columns_to_scale])

        prediction = model.predict(input_df)

        return Response(data={'word': int_to_char[prediction[0]]})

    def post(self, request):
        data_serializer = ModelDataSerializer(data=request.data)
        data_serializer.is_valid(raise_exception=True)

        # np.array([[feature1, feature2, ..., featureN]])
        data = [float(value) for value in list(data_serializer.validated_data.values())]
        x = np.array([data])

        prediction = model.predict(x)

        return Response(prediction)
