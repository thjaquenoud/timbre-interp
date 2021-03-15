/*
  ==============================================================================

    MyTimer.cpp
    Created: 23 Feb 2021 3:43:23pm
    Author:  sam

  ==============================================================================
*/

#include "AudioGenerator.h"

AudioGenerator::AudioGenerator(unsigned int b, unsigned int sr, unsigned int c)
    : Timer()
{   
    make_audio = false;
    batches = b;
    num_chunks = batches - 1;
    samp_rate = r;
    chunk = c;
    len_window = 4 * chunk;
    
    startTimer(1);
}

void AudioGenerator::timerCallback() override
{
    if(make_audio)
        gen_audio(num_chunks);
        
    for(int i = 0; i < sliders.size(); i++)
        temp_sliders[i] = (sliders[i]->getValue()) / 100;
}

double* AudioGenerator::genAudio(double *audio)
{
    unsigned int windowCount = batches + 3; // CHANGE FOR MIXER/EFFECTS
    unsigned int windowSizeHalf = len_window / 2 + 1;
    double **magnitudes = new double*[windowCount];
    double **phases = new double*[windowCount];
    for (int i = 0; i < windowCount; i++) {
        magnitudes[i] = new double[windowSizeHalf];
        phases[i] = new double[windowSizeHalf];
    }
    
    for (int i = 0; i < windowCount; i++)
        input.push_back(temp_sliders);
    tensorflow::Input::Initializer input_tensor(input);
    const Tensor& resized_tensor = input_tensor.tensor;
    
    // Actually run the image through the model.
    std::vector<Tensor> outputs;
    tensorflow::Status run_status = session->Run({{input_layer, resized_tensor}},
                                   {output_layer}, {}, &outputs);
                                   
    if (!run_status.ok()) {
        LOG(ERROR) << "Running model failed: " << run_status;
        return -1;
    }
                                   
    for (int i = 0; i < windowCount; i++) {
        for (int j = 0; j < windowSizeHalf; j++) {
            magnitudes[i][j] = outputs[0].flat<double>()(windowSizeHalf * i + j);
            phases[i][j] = 0.0;
        }
    }
    
    return istft(magnitudes, phases, len_window, windowCount, chunk*windowCount, chunk);
}

void AudioGenerator::modelToMem()
{
    // We need to call this to set up global state for TensorFlow.
    int argc;
    char** argv;
    tensorflow::port::InitMain("musicae", &argc, &argv);
    if (argc > 1) {
        LOG(ERROR) << "Unknown argument " << argv[1] << "\n" << usage;
        return -1;
    }

    // First we load and initialize the model.
    tensorflow::string graph_path = tensorflow::io::JoinPath(".", model_name);
    tensorflow::Status load_graph_status = LoadGraph(graph_path, &session);
    if (!load_graph_status.ok()) {
        LOG(ERROR) << load_graph_status;
        return -1;
    }
}

// Reads a model graph definition from disk, and creates a session object you
// can use to run it.
tensorflow::Status LoadGraph(const tensorflow::string& graph_file_name,
                 std::unique_ptr<tensorflow::Session>* session) {
    tensorflow::GraphDef graph_def;
    tensorflow::Status load_graph_status = ReadBinaryProto(tensorflow::Env::Default(), graph_file_name, &graph_def);
    if (!load_graph_status.ok())
        return tensorflow::errors::NotFound("Failed to load compute graph at '", graph_file_name, "'");
   
    session->reset(tensorflow::NewSession(tensorflow::SessionOptions()));
    tensorflow::Status session_create_status = (*session)->Create(graph_def);
    if (!session_create_status.ok())
        return session_create_status;
  
    return tensorflow::Status::OK();
}
