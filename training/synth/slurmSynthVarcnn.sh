#!/bin/bash
#SBATCH --job-name=MUSIC_SYNTH_TEST
#SBATCH --output=/zooper2/musicae/synth/outputVarcnn.txt
#SBATCH --gres=gpu:2
#SBATCH --ntasks=1
#SBATCH --gres-flags=enforce-binding
#SBATCH --nodes=1-1
#SBATCH --mem=10gb

#This is a sample SLURM job script
#It has a job name called JOB_NAME
#It will save it's stdout to /zooper2/USERNAME/output.txt
#It will have access to 2 GPUs and 1GB of RAM for a maximum of 1 minute
#The minimum allocatable time is 1 minute

echo "CUDA DEVICES: $CUDA_VISIBLE_DEVICES"
export CUDA_HOME="/usr/local/cuda-10.1"
export PATH="$PATH:/usr/local/cuda-10.1/bin"
export LD_LIBRARY_PATH="/usr/local/cuda-10.1/lib64:/usr/local/cuda/extras/CUPTI/lib64"
source ../venv/bin/activate
python train-network-synth-varcnn.py --filename_in=long_frames --filename_out=long_synth --n_epochs=100

