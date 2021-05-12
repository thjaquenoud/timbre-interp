# MusicAE
Autoencoder Based Real-Time Mixing, Effects Modelling, and Synthesizing

This project is an extension of [1] and contains both a VST plugin and a standalone Python GUI for applying an autoencoder for the three applications of mixing, effects modelling, and synthesizing.

## Python
Training and standalone GUIs are performed in Python. Testing was done on Python 3.7.4. Training files are found in the ```training``` folder. Standalone GUIs are found in the ```GUIs``` folder. Those scripts require Ffmpeg and Port Audio 

```
sudo apt-get install libasound-dev portaudio19-dev libportaudio2 libportaudiocpp0
sudo apt-get install ffmpeg libav-tools
```

A virtual environment is recommended to ensure that all Python packages are correct. The Python packages are found in 4req.txt.

```
mkdir venv
python -m venv venv/
source venv/bin/activate
pip install --upgrade pip setuptools
pip install -r 4req.txt
```

In order to generate a corpus to train your autoencoder, run wav2frames.py on the wav file of your choice. Please ensure the audio file is placed in the ```audio``` directory.
```
python wav2frames.py --filename_in=my_audio.wav --filename_out=my_corpus
```

Then, to train an autoencoder, run 
```
python <training file> --filename_in=my_corpus --filename_out=my_embedding --n_epochs=100
```

To start a GUI, run 

```
python <GUI file>
```

For the mixing or effects GUIs, type the name of the tracks you would like to apply into the "Audio File" boxes. Be sure these audio files are placed in the ```audio``` directory.

For all 3 GUIs, type the prefix of the trained model you would like to run (in this case just ```my_embedding```) into the "Model" box.

Clicking "Play" will start the audio processing through the neural network and play out audio in real time. Change the value of the left group of sliders to change the latent representation of the audio. For the mixing GUI, change the value of slider between the "Audio File" boxes to change the weighting of the two tracks.

Clicking "Reset" will reset all slider values.

## VST Plugin
The VST plugin is "MusicAE.vst3". To use the plugin, FFTW must be installed which can be found [here](http://www.fftw.org/download.html). The configure script should be run as shown below.

```
./configure --enable-float --enable-shared --with-fpic
```

This is to implement an STFT and ISTFT which was implemented as in [2].

Additionally, the C++ TensorFlow libraries available after unzipping the 7zip archive "tflibs.7z.001" must be put in a location where they will be seen by the linker at runtime.

The plugin can then be scanned as any other plugin. The plugin contains all 3 components in one and can be toggled using the buttons on the top of the user interface. The effects and synthesizer components can be used by applying the plugin to a track and the mixer component can be used by applying the plugin to a track and adding two other input tracks as sidechains.

All models used in the VST plugin must be TensorFlow .pd files. .h5 files created from the training scripts can be converted to .pb files using the "k2tf_convert.py" file from the work of [3]. The model can be loaded in the plugin by supplying the model file path and clicking "Load". If the model loads correctly, the plugin can be started by clicking "Start". The sliders can be reset by clicking "Reset". The plugin can be stopped by clicking "Stop".

## References
1. Colonel J. and Keene S., “Low Latency Timbre Interpolation and Warping Using Autoencoding Neural Networks,” J. Audio Eng. Soc., Oct. 2020.
2. brachna, “How to reconstruct result of Signal->STFT->ISTFT->Signal using overlapping windows?,” stack overflow, Jul. 26, 2016. https://stackoverflow.com/questions/38580472/how-to-reconstruct-result-of-signal-stft-istft-signal-using-overlapping-windo?fbclid=IwAR2JUUH2UQwZd8_Wk8Hwe0yhgC9NVb3CEvZw9bMuI-EqaiNAvmaITkBDrxU
3. Brandon, “Run your Keras models in C++ Tensorflow,” bitbionic, Aug. 18, 2017. http://www.bitbionic.com/2017/08/18/run-your-keras-models-in-c-tensorflow/
