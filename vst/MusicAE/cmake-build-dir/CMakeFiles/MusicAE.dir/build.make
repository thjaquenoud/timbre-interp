# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/cmake-3.19.4-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.19.4-Linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sam/Downloads/ece396/timbre-interp/MusicAE

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir

# Include any dependencies generated for this target.
include CMakeFiles/MusicAE.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MusicAE.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MusicAE.dir/flags.make

CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.o: ../Source/PluginEditor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.o -c /home/sam/Downloads/ece396/timbre-interp/MusicAE/Source/PluginEditor.cpp

CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/Downloads/ece396/timbre-interp/MusicAE/Source/PluginEditor.cpp > CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.i

CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/Downloads/ece396/timbre-interp/MusicAE/Source/PluginEditor.cpp -o CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.s

CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.o: ../Source/PluginProcessor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.o -c /home/sam/Downloads/ece396/timbre-interp/MusicAE/Source/PluginProcessor.cpp

CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/Downloads/ece396/timbre-interp/MusicAE/Source/PluginProcessor.cpp > CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.i

CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/Downloads/ece396/timbre-interp/MusicAE/Source/PluginProcessor.cpp -o CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.o: /home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.o -c /home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o: /home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o -c /home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o: /home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o -c /home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o: /home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o -c /home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o: /home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o -c /home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.o: /home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.o -c /home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.o: /home/sam/JUCE/modules/juce_events/juce_events.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.o -c /home/sam/JUCE/modules/juce_events/juce_events.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_events/juce_events.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_events/juce_events.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.o: /home/sam/JUCE/modules/juce_core/juce_core.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.o -c /home/sam/JUCE/modules/juce_core/juce_core.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_core/juce_core.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_core/juce_core.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o: /home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o -c /home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o: /home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o -c /home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o: /home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o -c /home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.s

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o: CMakeFiles/MusicAE.dir/flags.make
CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o: /home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o -c /home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp > CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.i

CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp -o CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.s

# Object files for target MusicAE
MusicAE_OBJECTS = \
"CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.o" \
"CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o" \
"CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o"

# External object files for target MusicAE
MusicAE_EXTERNAL_OBJECTS =

MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/Source/PluginEditor.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/Source/PluginProcessor.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client_utils.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_graphics/juce_graphics.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_events/juce_events.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_core/juce_core.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/home/sam/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/build.make
MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a: CMakeFiles/MusicAE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX static library \"MusicAE_artefacts/libAudio Plugin Example_SharedCode.a\""
	$(CMAKE_COMMAND) -P CMakeFiles/MusicAE.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MusicAE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MusicAE.dir/build: MusicAE_artefacts/libAudio\ Plugin\ Example_SharedCode.a

.PHONY : CMakeFiles/MusicAE.dir/build

CMakeFiles/MusicAE.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MusicAE.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MusicAE.dir/clean

CMakeFiles/MusicAE.dir/depend:
	cd /home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sam/Downloads/ece396/timbre-interp/MusicAE /home/sam/Downloads/ece396/timbre-interp/MusicAE /home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir /home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir /home/sam/Downloads/ece396/timbre-interp/MusicAE/cmake-build-dir/CMakeFiles/MusicAE.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MusicAE.dir/depend

