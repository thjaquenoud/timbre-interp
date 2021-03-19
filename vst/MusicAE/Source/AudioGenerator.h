/*
  ==============================================================================

    TimerImpl.h
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
    
    std::string model_name;
    
    std::vector<std::vector<float>> input;
    
    std::unique_ptr<tensorflow::Session> session;
    
    tensorflow::Status LoadGraph(const tensorflow::string& graph_file_name, std::unique_ptr<tensorflow::Session>* session);

public:
    std::vector<float> sliders;
    
    AudioGenerator(const int& b, double& sr, const int& c, std::vector<float> temp_sliders);
//    AudioGenerator();

//    void timerCallback() override;
    
    template <typename Real>
    Real* genAudio(const Real *audio);
    
    void modelToMem();
};
        
