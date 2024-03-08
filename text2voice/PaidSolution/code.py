# !pip install openai

import openai

openai.api_key  = #floooth

# function
def say(text, voice, speech_file_path):
    response = openai.audio.speech.create(
    model="tts-1",
    voice=voice,
    input=text
    )
    response.stream_to_file(speech_file_path)

# test
say('مرحبا', 'alloy', 'response_openai.mp3') # voices: alloy, echo, fable, onyx, nova, and shimmer
