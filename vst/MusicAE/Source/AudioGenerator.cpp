/*
  ==============================================================================

    MyTimer.cpp
    Created: 23 Feb 2021 3:43:23pm
    Author:  sam

  ==============================================================================
*/

#include "AudioGenerator.h"

void AudioGenerator::timerCallback() override
{
    if(make_audio)
        gen_audio(num_chunks);
        
    for(int i = 0; i < sliders.size(); i++)
        temp_sliders[i] = (sliders[i]->getValue()) / 100;
}

void AudioGenerator::startNet()
{
    modelToMem();
    batch_ind = 0;
    
    make_audio = true;
    gen_audio(num_chunks);
    
    /* PyAudio stuff */
}

void AudioGenerator::genAudio(int seg_length)
{
    make_audio = false;
    
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
