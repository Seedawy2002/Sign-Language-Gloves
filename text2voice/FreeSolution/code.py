# !pip install gtts

from gtts import gTTS

#function
def say(text, speech_file_path):
    tts = gTTS(text=text, lang='ar')

    # Save the audio to a file
    tts.save(speech_file_path)

# test
say('مرحبا', 'response.mp3')
