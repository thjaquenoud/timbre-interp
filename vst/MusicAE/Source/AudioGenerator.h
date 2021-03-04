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
#include <JuceHeader.h>

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

class AudioGenerator : public Timer
{
private:
    bool make_audio;
    const unsigned int batches = 20;
    const unsigned int num_chunks = batches - 1;
    const unsigned int samp_rate = 44100;
    unsigned int batch_ind;
    const unsigned int chunk = 1024;
    const unsigned int len_window = 4 * chunk;
    
    std::vector<juce::Slider*> sliders;
    std::vector<float> temp_sliders;
    std::string model_name;
    
    std::vector<std::vector<float>> input;
    
    std::unique_ptr<tensorflow::Session> session;
    
public:
    void timerCallback() override;
    
    void genAudio(int seg_length);
    void startNet();
    void modelToMem();
    tensorflow::Status LoadGraph(const tensorflow::string& graph_file_name, std::unique_ptr<tensorflow::Session>* session);
};
        