from django.urls import path
from apps.ML_models.views import ModelAPIView, RealTimeModelAPIView


urlpatterns = [
    path('predict/', ModelAPIView.as_view()),
    path('realtime/predict/', RealTimeModelAPIView.as_view()),
]
