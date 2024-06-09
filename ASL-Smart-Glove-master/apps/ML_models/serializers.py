from rest_framework import serializers


class ModelDataSerializer(serializers.Serializer):
    # gyro_x  gyro_y  gyro_z  flex1  flex2  flex3  flex4  flex5  contact1  contact2  contact3
    # x = serializers.DecimalField(max_digits=15, decimal_places=10)
    # y = serializers.DecimalField(max_digits=15, decimal_places=10)
    # z = serializers.DecimalField(max_digits=15, decimal_places=10)
    flex1 = serializers.DecimalField(max_digits=15, decimal_places=10)
    flex2 = serializers.DecimalField(max_digits=15, decimal_places=10)
    flex3 = serializers.DecimalField(max_digits=15, decimal_places=10)
    flex4 = serializers.DecimalField(max_digits=15, decimal_places=10)
    flex5 = serializers.DecimalField(max_digits=15, decimal_places=10)
    contact1 = serializers.DecimalField(max_digits=15, decimal_places=10)
    contact2 = serializers.DecimalField(max_digits=15, decimal_places=10)
    contact3 = serializers.DecimalField(max_digits=15, decimal_places=10)
