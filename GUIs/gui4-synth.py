from tkinter import *
import os
import librosa
import soundfile as sf
import argparse
import pyaudio
import numpy as np
from tensorflow.keras.models import Model, load_model
import tensorflow.compat.v1 as tf
tf.disable_v2_behavior() 
from scipy import signal
import time
import sys
import scipy, pylab
from kivy.core.window import Window
from kivy.app import App
from kivy.uix.gridlayout import GridLayout
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.anchorlayout import AnchorLayout
from kivy.properties import ListProperty
from kivy.uix.slider import Slider
from kivy.graphics import *
from kivy.clock import Clock

POLL_TIME = 0.001       #refresh rate of slider update
CHUNK = 1024            #samples per chunk
BATCHES = 20            #chunks per snip
NUM_CHUNKS = BATCHES-1  #
SAMP_RATE = 44100       #sample rate
CHANNELS = 1            #
LEN_WINDOW = 4*CHUNK    #Specified length of analysis window

xfade_in = np.linspace(0,1,num=CHUNK)
xfade_out = np.flip(xfade_in)

class LoginScreen(BoxLayout):

    def __init__(self, **kwargs):

        upper = GridLayout(cols=10)
        lower = BoxLayout(orientation='vertical', spacing=2)
        self.orientation = 'horizontal'
        Window.size = (1000, 500)

        self.alpha = 1
        self.num_latents = 10
        self.temp_sliders = np.ones((1,self.num_latents))
        self.make_audio = False
        self.all_data = np.zeros((BATCHES,CHUNK))


        super(LoginScreen, self).__init__(**kwargs)
        with self.canvas.before:
            Color(0, 0.05, 0.15)
            Rectangle(pos=(0, 0), size=(2000, 2000))


        self.sliders = []
        for i in range(0,10):
            print(i)
            self.sliders.append(Slider(min=0, max=100, value=50, orientation='vertical'))
            self.sliders[i].fbind('value', self.slide)
            upper.add_widget(self.sliders[i])

        #Button to start music by calling useSliderVals(). This will be changed to a more relevant function soon
        self.padAnchor6 = AnchorLayout(size_hint_y=0.3)
        self.playButton = Button(text='Play', size_hint_y=None, height=50, size_hint_x=0.3, font_size=24, background_color=[0, 1, 0])
        self.playButton.bind(on_press=self.start_net)
        self.padAnchor6.add_widget(self.playButton)
        lower.add_widget(self.padAnchor6)


        #Button to reset values by calling reset() when clicked
        self.padAnchor = AnchorLayout(size_hint_y=0.3)
        self.resetButton = Button(text='Reset', size_hint_y=None, height=50, size_hint_x=0.3, font_size=24)
        self.resetButton.bind(on_press=self.reset)
        self.padAnchor.add_widget(self.resetButton)
        lower.add_widget(self.padAnchor)


        self.padAnchor6 = AnchorLayout(size_hint_y=0.3)
        self.textinput3 = TextInput(hint_text='Model', size_hint_y=None, height=45, font_size=24, size_hint_x=None, width=300, multiline='false')
        self.padAnchor6.add_widget(self.textinput3)
        lower.add_widget(self.padAnchor6)


        self.add_widget(upper)
        self.add_widget(lower)
        Clock.schedule_interval(self.loop, POLL_TIME)

    def reset(self, instance):
        for slider in self.sliders:
            slider.value = 50
        self.useSliderVals()

    def slide(self, instance, val):
        self.useSliderVals()

    def useSliderVals(self, *args, **kwargs):
        for slider in self.sliders:
            print(slider.value)

    def gen_audio(self, seg_length):
        num_samps = seg_length*CHUNK
        self.make_audio = False


        # temp_alpha = np.tile(self.alpha*self.temp_sliders,(NUM_CHUNKS+5,1))      #NUM_CHUNKS+5 is really sus, might have
        # temp_negalpha = np.tile((1-self.alpha)*self.temp_sliders,(NUM_CHUNKS+5,1))    #
        # temp_phase = self.alpha*phaseA+(1-self.alpha)*phaseB #Unstack and Interpolate Phase
        # temp_max = self.alpha*max_A+(1-self.alpha)*max_B #Unstack and Interpolate Normalizing gains
        # temp_out_mag = self.full_net.predict([ magA, magB, temp_alpha ,temp_negalpha ])
        #np.set_printoptions(threshold=sys.maxsize)
        print(self.temp_sliders)
        temp_out_mag = self.full_net.predict([np.tile(self.temp_sliders, (BATCHES + 4, 1))])
        #print(temp_out_mag[0,:])
        out_mag = temp_out_mag.T
        E = out_mag
        _, temp_out = np.float32(signal.istft(0.24*E, fs=SAMP_RATE, noverlap=3*CHUNK))  #0.24 sus
        out = temp_out[CHUNK:-2*CHUNK]
        print(out)
        #for i in range(len(out)):
        #    print(out[i])
        #    print("\n")
        newdim = len(out)//CHUNK
        self.new_data = out.reshape((newdim,CHUNK))

    def loop(self, *args, **kwargs):
        #print("LOOP")
        
        if self.make_audio:
            t = time.time() ################### we could get rid of it?
            self.gen_audio(NUM_CHUNKS)

        for w in range(self.num_latents):
            self.temp_sliders[0,w]=self.sliders[w].value/100


    def start_net(self, *args, **kwargs):

        self.model_to_mem()
        self.batch_ind = 0

        self.make_audio = True
        self.gen_audio(NUM_CHUNKS)

        self.p = pyaudio.PyAudio()
        self.stream = self.p.open(format=pyaudio.paFloat32,
                                    channels=CHANNELS,
                                    frames_per_buffer=CHUNK,
                                    rate=SAMP_RATE,
                                    output=True,
                                    stream_callback=self.callback)

        self.stream.start_stream()
        #time.sleep(0.1) ##### I DON"T KNOW IF WE NEED THIS

    def model_to_mem(self):
        #model_name = "long_synth"
        data_path_net = os.path.join(os.getcwd(),'models/'+self.textinput3.text+'_trained_network_synth.h5')
        #for now that path is hard coded
        #data_path_net = os.path.join(os.getcwd(),'models/'+model_name+'_trained_network_synth.h5')
        self.full_net = load_model(data_path_net, compile=False)
        self.full_net._make_predict_function()
        self.full_net_graph = tf.get_default_graph()

    def callback(self, in_data, frame_count, time_info, status, *args, **kwargs):
        if self.batch_ind >= NUM_CHUNKS:
            self.batch_ind = 0
        if self.batch_ind == 0:
            xfade = xfade_in*self.new_data[0,:] + xfade_out*self.all_data[-1,:]
            self.all_data = self.new_data
            self.all_data[0,:] = xfade
        if self.batch_ind == 1:
            self.make_audio = True

        data = self.all_data[self.batch_ind,:] #send chunk to audio buffer when requested
        self.batch_ind += 1
        return(data, pyaudio.paContinue)


    def stop_sound(self):
        self.stream.stop_stream()
        print('audio paused')
        self.stream.close()
        self.p.terminate()
        self.batch_ind = BATCHES
        self.make_audio = False

    def quit(self):
        root.destroy()


class MusicAE(App):

    def build(self):

        return LoginScreen()

if __name__ == '__main__':
    MusicAE().run()

