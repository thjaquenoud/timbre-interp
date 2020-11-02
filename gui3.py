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

class LoginScreen(GridLayout):

    def __init__(self, **kwargs):
        super(LoginScreen, self).__init__(**kwargs)
        with self.canvas.before:
            Color(0, 0.05, 0.15)
            Rectangle(pos=(0, 0), size=(2000, 2000))

        self.cols = 7

        self.sliders = []
        for i in range(0,7):
            print(i)
            self.sliders.append(Slider(min=0, max=100, value=50, orientation='vertical'))
            self.sliders[i].fbind('value', self.slide)
            self.add_widget(self.sliders[i])

        #Button to reset values by calling reset() when clicked
        self.padAnchor = AnchorLayout(size_hint_y=0.3)
        self.resetButton = Button(text='Reset', size_hint_y=None, height=75, size_hint_x=0.8, font_size=24)
        self.resetButton.bind(on_press=self.reset)
        self.padAnchor.add_widget(self.resetButton)
        self.add_widget(self.padAnchor)


        #Button to start music by calling useSliderVals(). This will be changed to a more relevant function soon
        self.padAnchor2 = AnchorLayout(size_hint_y=0.3)
        self.playButton = Button(text='Play', size_hint_y=None, height=75, size_hint_x=0.8, font_size=24, background_color=[0, 1, 0])
        self.playButton.bind(on_press=self.useSliderVals)
        self.padAnchor2.add_widget(self.playButton)
        self.add_widget(self.padAnchor2)

    
    def reset(self, instance):
        for slider in self.sliders:
            slider.value = 50
        self.useSliderVals()

    def slide(self, instance, val):
        self.useSliderVals()

    def useSliderVals(self, *args, **kwargs):
        for slider in self.sliders:
            print(slider.value)

class MusicAE(App):

    def build(self):
        return LoginScreen()

if __name__ == '__main__':
    MusicAE().run()

