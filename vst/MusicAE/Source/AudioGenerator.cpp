/*
  ==============================================================================

    AudioGenerator.cpp
    Created: 23 Feb 2021 3:43:23pm
    Author:  MusicAE

  ==============================================================================
*/

#include "AudioGenerator.h"

AudioGenerator::AudioGenerator(const int& b, double& sr, const int& c, std::vector<float> temp_sliders)
    :
    batches(b),
    num_chunks(b - 1),
    samp_rate(sr),
    chunk(c),
    len_window(4 * c),
    first(true),
    sliders(temp_sliders.begin(), temp_sliders.end())
{
}

void AudioGenerator::modelToMem()
{
    // We need to call this to set up global state for TensorFlow.
    int argc = 1;
    char** argv;
    tensorflow::port::InitMain("musicae", &argc, &argv);
    if (argc > 1) {
        LOG(ERROR) << "Unknown error\n";
        exit(EXIT_FAILURE);
    }

    // First we load and initialize the model.
    tensorflow::string graph_path = model_name;
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
