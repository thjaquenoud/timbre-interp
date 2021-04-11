/*
  ==============================================================================

    AudioGenerator.h
    Created: 23 Feb 2021 3:17:42pm
    Author:  sam

  ==============================================================================
*/

#pragma once

#include <fstream>
#include <utility>
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"

#include "tensorflow/cc/framework/ops.h"
#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"
#include "stft.h"

enum MusicAE_state {
        STATE_SYNTH,
        STATE_EFFECTS,
        STATE_MIXER
}; 

class AudioGenerator //: public juce::Timer
{
private:
    bool make_audio;
    const int batches {20};
    const int num_chunks {batches - 1};
    double samp_rate {44100};
    const int chunk {1024};
    const int len_window {4 * chunk};
    
    std::vector<std::vector<float>> input;
    bool first {true};
    
    std::unique_ptr<tensorflow::Session> session;
    
    tensorflow::Status LoadGraph(const tensorflow::string& graph_file_name, std::unique_ptr<tensorflow::Session>* session);

public:
    std::string model_name;
    float alpha;
    std::vector<float> sliders;
    
    AudioGenerator(const int& b, double& sr, const int& c, std::vector<float> temp_sliders);
//    AudioGenerator();

//    void timerCallback() override;

    template <typename Real>
    Real* genAudio(const Real *audio1, const Real *excessAudio1, const Real *audio2, const Real *excessAudio2, int excess, const Real *fade, enum MusicAE_state state)
    {
        std::cerr << "genaudio" << state << "\n";
        int windowCount = state == STATE_SYNTH ? batches + 3 : batches + 4; // CHANGE FOR MIXER/EFFECTS
        int windowSizeHalf = len_window / 2 + 1;
        float **magnitudes = new float*[windowCount];
        float **phases = new float*[windowCount];
        for (int i = 0; i < windowCount; i++) {
            magnitudes[i] = new float[windowSizeHalf];
            phases[i] = new float[windowSizeHalf];
        }
        
        if (state == STATE_SYNTH) {
            tensorflow::Tensor input_sliders(tensorflow::DT_FLOAT, tensorflow::TensorShape({windowCount, 10}));

            for (int i = 0; i < windowCount; i++){
                for (int j = 0; j < 10; j++){
                    input_sliders.flat<float>()(10 * i + j) = sliders[j];
                }
            }

            /*for (int i = 0; i < windowCount; i++){
                for (int j = 0; j < 10; j++){
                    std::cerr << input_sliders.flat<float>()(10 * i + j) << " ";
                }
                std::cerr << "\n";
            }*/

            //std::cerr << "\n";

            // Actually run the image through the model.
            std::string input_layer = "Latent_Input", output_layer = "k2tfout_0"; // IMPORTANT: These are model dependent
            std::vector<tensorflow::Tensor> outputs;
            tensorflow::Status run_status = session->Run({{input_layer, input_sliders}},
                                        {output_layer}, {}, &outputs);

            if (!run_status.ok()) {
                LOG(ERROR) << "Running model failed: " << run_status;
                exit(EXIT_FAILURE);
            }
            
            for (int i = 0; i < windowCount; i++) {
                for (int j = 0; j < windowSizeHalf; j++) {
                    //std::cerr << outputs[0].flat<float>()(windowSizeHalf * i + j) << " ";
                    magnitudes[i][j] = 0.24 * outputs[0].flat<float>()(windowSizeHalf * i + j);
                    phases[i][j] = 0.0;
                }
                //std::cerr << "\n";
            }
        }
        else {
            struct stftReturn input1 = stft(audio1, excessAudio1, excess, len_window, windowCount, chunk*windowCount, chunk);
            if (state == STATE_EFFECTS){
                std::cerr << "finished stft\n";
                tensorflow::Tensor input_effects(tensorflow::DT_FLOAT, tensorflow::TensorShape({windowCount, 10}));
                for (int i = 0; i < windowCount; i++){
                    for (int j = 0; j < 10; j++){
                        input_effects.flat<float>()(10 * i + j) = sliders[j];
                        //std::cerr << input_effects.flat<float>()(10 * i + j) << " ";
                    }
                    //std::cerr << "\n";
                }
                std::cerr << "finished setting effects\n";

            
                std::string track1_layer = "Track1_Input", latent_layer = "Latent_Input", output_layer = "k2tfout_0"; // IMPORTANT: These are model dependent
                std::vector<tensorflow::Tensor> outputs;
                tensorflow::Status run_status = session->Run({{track1_layer, input1.magnitudes}, {latent_layer, input_effects}},
                                            {output_layer}, {}, &outputs);

                if (!run_status.ok()) {
                    LOG(ERROR) << "Running model failed: " << run_status;
                    exit(EXIT_FAILURE);
                }
                
                std::cerr << "finished model\n";
                for (int i = 0; i < windowCount; i++) {
                    for (int j = 0; j < windowSizeHalf; j++) {
                        //std::cerr << outputs[0].flat<float>()(windowSizeHalf * i + j) << " ";
                        magnitudes[i][j] = 0.24 * outputs[0].flat<float>()(windowSizeHalf * i + j) * input1.max[i];
                        phases[i][j] = input1.phases[i][j];
                    }
                    //std::cerr << "finished setting mag and phases:" << i << "\n";
                }
                //std::cerr << "finished setting mag and phases\n";

            }
            else {           
                struct stftReturn input2 = stft(audio2, excessAudio2, excess, len_window, windowCount, chunk*windowCount, chunk);
                
                tensorflow::Tensor input_alpha(tensorflow::DT_FLOAT, tensorflow::TensorShape({windowCount, 10}));
                tensorflow::Tensor input_negalpha(tensorflow::DT_FLOAT, tensorflow::TensorShape({windowCount, 10}));
                for (int i = 0; i < windowCount; i++){
                    for (int j = 0; j < 10; j++){
                        input_alpha.flat<float>()(10 * i + j) = alpha * sliders[j];
                        input_negalpha.flat<float>()(10 * i + j) = (1-alpha) * sliders[j];
                    }
                }
                
                std::string track1_layer = "Track1_Input", track2_layer = "Track2_Input", alpha_layer = "Alpha_Input", negalpha_layer = "Negalpha_Input", output_layer = "k2tfout_0"; // IMPORTANT: These are model dependent
                std::vector<tensorflow::Tensor> outputs;
                tensorflow::Status run_status = session->Run({{track1_layer, input1.magnitudes}, {track2_layer, input2.magnitudes}, {alpha_layer, input_alpha}, {negalpha_layer, input_negalpha}},
                                            {output_layer}, {}, &outputs);

                if (!run_status.ok()) {
                    LOG(ERROR) << "Running model failed: " << run_status;
                    exit(EXIT_FAILURE);
                }

                for (int i = 0; i < windowCount; i++) {
                    for (int j = 0; j < windowSizeHalf; j++) {
                        //std::cerr << outputs[0].flat<float>()(windowSizeHalf * i + j) << " ";
                        magnitudes[i][j] = 0.24 * outputs[0].flat<float>()(windowSizeHalf * i + j) * (alpha * input1.max[i] + (1-alpha) * input2.max[i]);
                        phases[i][j] = alpha * input1.phases[i][j] + (1-alpha) * input2.phases[i][j];
                    }
                    //std::cerr << "\n";
                }
            }
        }

        std::cerr << "genaudio done\n";
        Real *ret = istft(magnitudes, phases, len_window, windowCount, chunk*windowCount, chunk, fade, first, state == STATE_SYNTH);
        
        first = false;
        return ret;

    }  

    void modelToMem();
};
        
