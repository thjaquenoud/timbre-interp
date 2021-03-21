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

#include "stft.h"
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
    
    std::unique_ptr<tensorflow::Session> session;
    
    tensorflow::Status LoadGraph(const tensorflow::string& graph_file_name, std::unique_ptr<tensorflow::Session>* session);

public:
    std::string model_name;
    std::vector<float> sliders;
    
    AudioGenerator(const int& b, double& sr, const int& c, std::vector<float> temp_sliders);
//    AudioGenerator();

//    void timerCallback() override;

    template <typename Real>
    Real* genAudio(const Real *audio)
    {
        int windowCount = batches + 3; // CHANGE FOR MIXER/EFFECTS
        int windowSizeHalf = len_window / 2 + 1;
        float **magnitudes = new float*[windowCount];
        float **phases = new float*[windowCount];
        for (int i = 0; i < windowCount; i++) {
            magnitudes[i] = new float[windowSizeHalf];
            phases[i] = new float[windowSizeHalf];
        }

        for (int i = 0; i < windowCount; i++)
            input.push_back(sliders);
        //tensorflow::Input::Initializer input_tensor(input, tensorflow::TensorShape({windowCount, 10}));
        tensorflow::Input::Initializer input_tensor({1.0});
        const tensorflow::Tensor& resized_tensor = input_tensor.tensor;

        // Actually run the image through the model.
        std::string input_layer = "Latent_Input", output_layer = "k2tfout_0"; // IMPORTANT: These are model dependent
        std::vector<tensorflow::Tensor> outputs;
        tensorflow::Status run_status = session->Run({{input_layer, resized_tensor}},
                                       {output_layer}, {}, &outputs);

        if (!run_status.ok()) {
            LOG(ERROR) << "Running model failed: " << run_status;
            exit(EXIT_FAILURE);
        }
                               
        for (int i = 0; i < windowCount; i++) {
            for (int j = 0; j < windowSizeHalf; j++) {
                magnitudes[i][j] = outputs[0].flat<float>()(windowSizeHalf * i + j);
                phases[i][j] = 0.0;
            }
        }

        Real dummy = 1.0;
        return istft(magnitudes, phases, len_window, windowCount, chunk*windowCount, chunk, dummy);
    }  

    void modelToMem();
};
        
