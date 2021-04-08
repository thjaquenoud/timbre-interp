/*
  ==============================================================================

    AudioGenerator.cpp
    Created: 23 Feb 2021 3:43:23pm
    Author:  sam

  ==============================================================================
*/

#include "AudioGenerator.h"

AudioGenerator::AudioGenerator(const int& b, double& sr, const int& c, std::vector<float> temp_sliders)
    : //Timer(),
    batches(b),
    num_chunks(b - 1),
    samp_rate(sr),
    chunk(c),
    len_window(4 * c),
    first(true),
    sliders(temp_sliders.begin(), temp_sliders.end())
{
    //std::cerr << "audio generator constructor\n";
    /*doublePrecision = dp;
    make_audio = false;
    batches = b;
    num_chunks = batches - 1;
    samp_rate = sr;
    chunk = c;
    len_window = 4 * chunk;
    
    startTimer(1);*/
}

/*AudioGenerator::AudioGenerator()
    : Timer()
{
    doublePrecision = true;
}*/

/*void AudioGenerator::timerCallback() override
{
    if(make_audio)
        gen_audio(num_chunks);
        
    for(int i = 0; i < sliders.size(); i++)
        temp_sliders[i] = (sliders[i]->getValue()) / 100;
}*/

void AudioGenerator::modelToMem()
{
    //std::cerr << "model to mem\n";
    // We need to call this to set up global state for TensorFlow.
    int argc;
    char** argv;
    tensorflow::port::InitMain("musicae", &argc, &argv);
    if (argc > 1) {
        LOG(ERROR) << "Unknown argument " << argv[1] << "\n";
        exit(EXIT_FAILURE);
    }

    // First we load and initialize the model.
    tensorflow::string graph_path = tensorflow::io::JoinPath("/home/sam/ece396/timbre-interp/keras-to-tensorflow-master", model_name);
    tensorflow::Status load_graph_status = LoadGraph(graph_path, &session);
    if (!load_graph_status.ok()) {
        LOG(ERROR) << load_graph_status;
        exit(EXIT_FAILURE);
    }
}

// Reads a model graph definition from disk, and creates a session object you
// can use to run it.
tensorflow::Status AudioGenerator::LoadGraph(const tensorflow::string& graph_file_name,
                 std::unique_ptr<tensorflow::Session>* session) {
    //std::cerr << "load graph\n";
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
