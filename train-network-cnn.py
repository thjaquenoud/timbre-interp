from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow.compat.v1 as tf
tf.disable_v2_behavior()
import tensorflow.keras
from tensorflow.keras.layers import Input, Dense, Lambda, Concatenate, Dropout, LeakyReLU, Multiply, Add, Conv2D, MaxPooling2D, Flatten, Reshape, UpSampling2D, Conv2DTranspose
from tensorflow.keras.models import Model, Sequential, load_model, clone_model
from tensorflow.keras.activations import linear

from tensorflow.keras.regularizers import l2
from tensorflow.keras.losses import mse
from tensorflow.keras.callbacks import LambdaCallback
from tensorflow.keras.optimizers import Adam

from tensorflow.python.keras import backend as K
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os
import librosa


global alpha
global beta
beta = K.variable(3e-7)
alpha = K.variable(0.3)
np.random.seed(90210)

def change_params(epoch, logs):
	if epoch<=5 and epoch%1==0:
		K.set_value(beta,K.get_value(beta)+2e-5)
	if epoch == 30:
		K.set_value(alpha,0.0)

def get_arguments():
	parser = argparse.ArgumentParser()
	parser.add_argument('--filename_in', type=str)
	parser.add_argument('--filename_out', type=str)
	parser.add_argument('--mode', type=str, default='train')
	parser.add_argument('--trained_model_name', type=str, default='')
	parser.add_argument('--n_epochs', type=int, default=5)
	return parser.parse_args()

class Manne:
	def __init__(self, args):
		self.frames = []
		self.X_train = []
		self.X_val = []
		self.X_test = []
		self.encoder = []
		self.decoder = []
		self.network = []
		self.dnb_net = []
		self.encoder_widths = []
		self.decoder_widths = []
		self.beta_changer = []

		self.n_epochs = args.n_epochs
		self.filename_in = args.filename_in
		self.filename_out = args.filename_out
		self.trained_model_name = args.trained_model_name

	def do_everything(self):
		self.load_dataset()
		self.define_net()
		self.train_net()
		self.evaluate_net()

	def just_plot(self):
		self.load_dataset()
		self.load_net()
		self.make_net()
		adam_rate = 5e-4
		self.network.compile(optimizer=Adam(lr=adam_rate), loss=self.my_mse, metrics=[self.my_mse])
		self.evaluate_net()
		self.save_latents()

	def my_mse(self, inputs, outputs):
		return mse(inputs[:,:4098],outputs)

	def my_mse2(self, inputs, outputs):
		inspec1, outspec1 = inputs[0], outputs[0]
		inspec2, outspec2 = inputs[1], outputs[1]
		outloss = K.sum(mse(inspec1,outspec1)+mse(inspec2,outspec2))
		return outloss

	def load_net(self):
		enc_filename = os.path.join(os.getcwd(),'models/'+self.trained_model_name+'_trained_encoder.h5')
		print(enc_filename)
		self.encoder = load_model(enc_filename,custom_objects={'sampling': self.sampling}, compile=False)
		dec_filename = os.path.join(os.getcwd(),'models/'+self.trained_model_name+'_trained_decoder.h5')
		self.decoder = load_model(dec_filename,custom_objects={'sampling': self.sampling}, compile=False)

	def load_dataset(self):
		global sample_length

		filename = 'frames/'+self.filename_in+'_frames.npy'	#Static Data used for training net
		filepath = os.path.join(os.getcwd(),filename)
		orig_frames = np.load(filepath)
		orig_frames_1 = np.asarray(orig_frames)
		
		sample_length = 24
		len_frames = orig_frames_1.shape[0]
		num_samples = len_frames // sample_length
		num_frames = num_samples * sample_length
		orig_frames_cnn = np.reshape(orig_frames_1[:num_frames,:], (num_samples,sample_length,-1,1))
		
		np.random.shuffle(orig_frames_cnn)

		self.frames = orig_frames_cnn
		orig_frames = None
		orig_frames_1 = None

		self.X_train =  self.frames[:int(0.95*num_samples),:,:] 
		self.X_val =  self.frames[int(0.95*num_samples):int(0.975*num_samples),:,:]
		self.X_test =  self.frames[int(0.975*num_samples):,:,:]



	def define_net(self):
		global decoder_outdim
		global sample_length

		l2_penalty = 1e-7

		decoder_outdim = 2049
		self.encoder_widths = [256,128,64,32,16,10]
		self.decoder_widths = [16,32,64,128,256,454]
		self.cnn_filters = [32,16]

		drop = 0.0
		alpha_val=0.1
		input_spec1 = Input(shape=(sample_length,decoder_outdim,1))

		encoded1 = Conv2D(self.cnn_filters[0], 3, padding='same', kernel_regularizer=l2(l2_penalty))(input_spec1)
		encoded1 = LeakyReLU(alpha=alpha_val)(encoded1)

		encoded1 = Conv2D(self.cnn_filters[0], 3, padding='same', kernel_regularizer=l2(l2_penalty))(encoded1)
		encoded1 = LeakyReLU(alpha=alpha_val)(encoded1)
		encoded1 = MaxPooling2D(3)(encoded1)

		for i in range(2):
			encoded1 = Conv2D(self.cnn_filters[1], 3, padding='same', kernel_regularizer=l2(l2_penalty))(encoded1)
			encoded1 = LeakyReLU(alpha=alpha_val)(encoded1)

		encoded1 = MaxPooling2D(3)(encoded1)

		encoded1 = Flatten()(encoded1)

		for width in self.encoder_widths[0:-1]:
			encoded1 = Dense(units=width,
				activation=None,
				kernel_regularizer=l2(l2_penalty))(encoded1)
			encoded1 = LeakyReLU(alpha=alpha_val)(encoded1)

		encoded1 = Dense(units=self.encoder_widths[-1], activation='sigmoid', kernel_regularizer=l2(l2_penalty))(encoded1)
		self.encoder = Model(inputs=[input_spec1], outputs=[encoded1])

		input_latent1 = Input(shape=(self.encoder_widths[-1],))
		decoded1 = Dense(units=self.decoder_widths[0],
			activation=None,
			kernel_regularizer=l2(l2_penalty))(input_latent1)
		decoded1 = LeakyReLU(alpha=alpha_val)(decoded1)

		for width in self.decoder_widths[1:-1]:
			decoded1 = Dense(units=width,
				activation=None,
				kernel_regularizer=l2(l2_penalty))(decoded1)
			decoded1 = LeakyReLU(alpha=alpha_val)(decoded1)

		decoded1 = Dense(units=self.decoder_widths[-1],
			kernel_regularizer=l2(l2_penalty))(decoded1)
		decoded1 = LeakyReLU(alpha=alpha_val)(decoded1)

		###

		decoded1 = Reshape((2, 227, 1))(decoded1)
		decoded1 = UpSampling2D(3)(decoded1)
		
		decoded1 = Conv2DTranspose(self.cnn_filters[1], 3, padding='same', kernel_regularizer=l2(l2_penalty))(decoded1)
		decoded1 = LeakyReLU(alpha=alpha_val)(decoded1)

		decoded1 = Conv2DTranspose(self.cnn_filters[1], 3, kernel_regularizer=l2(l2_penalty))(decoded1)
		decoded1 = LeakyReLU(alpha=alpha_val)(decoded1)
		decoded1 = UpSampling2D(3)(decoded1)

		decoded1 = Conv2DTranspose(self.cnn_filters[0], 3, padding='same', kernel_regularizer=l2(l2_penalty))(decoded1)
		decoded1 = LeakyReLU(alpha=alpha_val)(decoded1)

		decoded1 = Conv2DTranspose(1, 3, padding='same', activation='relu', kernel_regularizer=l2(l2_penalty))(decoded1)

		self.decoder = Model(inputs=[input_latent1], outputs=[decoded1])

		my_batch = [input_spec1]
		my_encoded = self.encoder(my_batch)
		my_decoded = self.decoder(my_encoded)
		self.network = Model(inputs=my_batch, outputs=my_decoded)

		print('\n net summary \n')
		self.network.summary()
		print('\n enc summary \n')
		self.encoder.summary()
		print('\n dec summary \n')
		self.decoder.summary()



	def train_net(self):
		global decoder_outdim
		global sample_length
		adam_rate = 1e-4
		train_data = [self.X_train[:,:sample_length,:decoder_outdim]]
		train_target = [self.X_train[:,:sample_length,:decoder_outdim]]
		val_data = [self.X_val[:,:sample_length,:decoder_outdim]]
		val_target = [self.X_val[:,:sample_length,:decoder_outdim]]

		self.network.compile(optimizer=Adam(lr=adam_rate), loss=mse)
		self.network.fit(x=train_data, y=train_target,
				epochs=self.n_epochs,
				batch_size=200,
				shuffle=True,
				validation_data=(val_data, val_target)
				)


		mode = Input(shape=(1,))
		modalpha1 = Input(shape=(self.encoder_widths[-1],))
		modnegalpha1 = Input(shape=(self.encoder_widths[-1],))
		final_spec_1 = Input(shape=(sample_length, decoder_outdim,))
		final_spec_2 = Input(shape=(sample_length, decoder_outdim,))
		my_batch0 = [final_spec_1]
		my_batch1 = [final_spec_2]
		
		# synthesis
		if mode == 0:
			latent = [modalpha1]
			final_decoded = self.decoder(latent)
		# effects
		elif mode == 1:
			my_encoded = self.encoder(my_batch0)
			mod_latent = Multiply()([my_encoded,modalpha1])
			final_decoded = self.decoder([mod_latent])
		# mixing
		else:
			my_encoded0 = self.encoder(my_batch0)
			my_encoded1 = self.encoder(my_batch1)
			blarg0 = Multiply()([my_encoded0,modalpha1])
			blarg1 = Multiply()([my_encoded1,modnegalpha1])
			mod_latent1 = Add()([blarg0,blarg1])
			final_decoded = self.decoder([mod_latent1])
		
		self.dnb_net = Model(inputs=[mode,final_spec_1,final_spec_2,modalpha1,modnegalpha1],
			outputs=final_decoded)
		self.dnb_net.save('models/'+self.filename_out+'_trained_network.h5')

	def save_latents(self):
		indat = self.frames
		enc_mag = self.encoder.predict(indat,verbose=1)
		df = pd.DataFrame(enc_mag)
		df.to_csv('encoded_mags.csv')


	def evaluate_net(self):
		global decoder_outdim
		global sample_length

		test_data = self.X_test[:,:sample_length,:decoder_outdim]
		test_target = self.X_test[:,:sample_length,:decoder_outdim]
		val_data = self.X_val[:,:sample_length,:decoder_outdim]
		val_target = self.X_val[:,:sample_length,:decoder_outdim]


		if args.filename_in == 'one_octave':
			mod = 1
		elif args.filename_in == 'five_octave' or args.filename_in == 'violin':
			mod = 10
		elif args.filename_in == 'guitar':
			mod = 3
		else:
			mod = 1

		print('\n')
		print('Evaluating performance on validation and test sets')
		a=self.network.evaluate(x=val_data,y=val_target,verbose=1)
		b=self.network.evaluate(x=test_data,y=test_target,verbose=1)
		print('\n')
		print('Plotting network reconstructions')
		valset_eval = self.network.predict(val_data,verbose=1)
		testset_eval = self.network.predict(test_data,verbose=1)
		frame_check = [] #[10, 15, 20, 25, 30]#, 350, 400, 450, 500]

		for frame in frame_check:
			frame *= mod
			xx = np.arange(2049)*(22050/2049)
			val_yy = val_target[frame,0:2049]
			val_zz = valset_eval[frame,0:2049]
			test_yy = test_target[frame,0:2049]
			test_zz = testset_eval[frame,0:2049]
			plt.figure(1)
			plt.subplot(211)
			plt.plot(xx,val_yy)
			plt.ylim([0,1.2])
			plt.ylabel('Spectral Magnitude')
			plt.xscale('log')
			plt.xlabel('Frequency (Hz)')
			plt.title('Input Spectrum')
			plt.subplot(212)
			plt.plot(xx,val_zz,color='r')
			plt.ylim([0,1.2])
			plt.ylabel('Spectral Magnitude')
			plt.xscale('log')
			plt.xlabel('Frequency (Hz)')
			plt.title('Output Spectrum')
			plt.tight_layout()
			plotname = '_drums_'+str(frame)+'.pdf'
			plt.savefig(plotname, format = 'pdf', bbox_inches='tight')
			plt.clf()

			plt.figure(1)
			plt.subplot(211)
			plt.plot(xx,test_yy)
			plt.ylim([0,1.2])
			plt.ylabel('Spectral Magnitude')
			plt.xscale('log')
			plt.xlabel('Frequency (Hz)')
			plt.title('Input Spectrum')
			plt.subplot(212)
			plt.plot(xx,test_zz,color='r')
			plt.ylim([0,1.2])
			plt.ylabel('Spectral Magnitude')
			plt.xscale('log')
			plt.xlabel('Frequency (Hz)')
			plt.title('Output Spectrum')
			plt.tight_layout()
			plotname = '_bass_'+str(frame)+'.pdf'
			plt.savefig(plotname, format = 'pdf', bbox_inches='tight')
			plt.clf()


if __name__ == '__main__':
	args = get_arguments()
	my_manne = Manne(args)
	if args.mode == 'train':
		my_manne.do_everything()
	else:
		my_manne.just_plot()
