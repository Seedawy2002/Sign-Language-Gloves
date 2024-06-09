#!/bin/python3
from crypt import methods
from flask import Flask, request, jsonify, send_file
from helpers.helpers import predict_img_main, say, predict_hand
from PIL import Image
import io
from flask_cors import CORS

app = Flask(__name__)
CORS(app, origins='*')

@app.route('/' , methods=['GET'], strict_slashes=False)
def home():
    return "Hello World"

@app.route('/predict_img', methods=['POST'], strict_slashes=False)
def predict_img():
    if request.method == 'POST':
        if 'image' not in request.files:
            return jsonify({'error': 'No image provided'}), 400
        file = request.files['image']
        if not file.filename.lower().endswith('.jpg'):
            return jsonify({'error': 'Invalid file type. Please upload a JPG image.'}), 400

        image_bytes = file.read()
        img = Image.open(io.BytesIO(image_bytes))

        # save image
        filename = './imgs/new_photo.jpg'
        img.save(filename)

        res = predict_img_main(filename)

        # return anything for now
        return jsonify({"result": res}), 200

@app.route("/say", methods=['POST'], strict_slashes=False)
def say_text():
    if request.method == 'POST':
        data = request.get_json()
        text = data.get('text')
        if text is None:
            return jsonify({'error': 'No text provided'}), 400

        speech_file_path = './speech.mp3'
        say(text, speech_file_path)

        return send_file(speech_file_path, as_attachment=True), 200


@app.route('/predict_hand', methods=['POST'], strict_slashes=False)
def predict_hand_func():
    if request.method == 'POST':
        res = predict_hand()
        return jsonify({"result": res}), 200


if __name__ == '__main__':
    app.run('0.0.0.0', port=5000, debug=True)
