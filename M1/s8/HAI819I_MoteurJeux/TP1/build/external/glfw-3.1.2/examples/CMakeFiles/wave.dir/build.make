# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build

# Include any dependencies generated for this target.
include external/glfw-3.1.2/examples/CMakeFiles/wave.dir/depend.make

# Include the progress variables for this target.
include external/glfw-3.1.2/examples/CMakeFiles/wave.dir/progress.make

# Include the compile flags for this target's objects.
include external/glfw-3.1.2/examples/CMakeFiles/wave.dir/flags.make

external/glfw-3.1.2/examples/CMakeFiles/wave.dir/wave.c.o: external/glfw-3.1.2/examples/CMakeFiles/wave.dir/flags.make
external/glfw-3.1.2/examples/CMakeFiles/wave.dir/wave.c.o: ../external/glfw-3.1.2/examples/wave.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glfw-3.1.2/examples/CMakeFiles/wave.dir/wave.c.o"
	cd /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/external/glfw-3.1.2/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/wave.dir/wave.c.o   -c /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/external/glfw-3.1.2/examples/wave.c

external/glfw-3.1.2/examples/CMakeFiles/wave.dir/wave.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/wave.dir/wave.c.i"
	cd /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/external/glfw-3.1.2/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/external/glfw-3.1.2/examples/wave.c > CMakeFiles/wave.dir/wave.c.i

external/glfw-3.1.2/examples/CMakeFiles/wave.dir/wave.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/wave.dir/wave.c.s"
	cd /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/external/glfw-3.1.2/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/external/glfw-3.1.2/examples/wave.c -o CMakeFiles/wave.dir/wave.c.s

# Object files for target wave
wave_OBJECTS = \
"CMakeFiles/wave.dir/wave.c.o"

# External object files for target wave
wave_EXTERNAL_OBJECTS =

external/glfw-3.1.2/examples/wave: external/glfw-3.1.2/examples/CMakeFiles/wave.dir/wave.c.o
external/glfw-3.1.2/examples/wave: external/glfw-3.1.2/examples/CMakeFiles/wave.dir/build.make
external/glfw-3.1.2/examples/wave: external/glfw-3.1.2/src/libglfw3.a
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/librt.so
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/libm.so
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/libX11.so
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/libXrandr.so
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/libXinerama.so
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/libXi.so
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/libXcursor.so
external/glfw-3.1.2/examples/wave: /usr/lib/x86_64-linux-gnu/libGL.so
external/glfw-3.1.2/examples/wave: external/glfw-3.1.2/examples/CMakeFiles/wave.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable wave"
	cd /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/external/glfw-3.1.2/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wave.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/glfw-3.1.2/examples/CMakeFiles/wave.dir/build: external/glfw-3.1.2/examples/wave

.PHONY : external/glfw-3.1.2/examples/CMakeFiles/wave.dir/build

external/glfw-3.1.2/examples/CMakeFiles/wave.dir/clean:
	cd /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/external/glfw-3.1.2/examples && $(CMAKE_COMMAND) -P CMakeFiles/wave.dir/cmake_clean.cmake
.PHONY : external/glfw-3.1.2/examples/CMakeFiles/wave.dir/clean

external/glfw-3.1.2/examples/CMakeFiles/wave.dir/depend:
	cd /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1 /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/external/glfw-3.1.2/examples /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/external/glfw-3.1.2/examples /home/e20180001518/Master/M1/s8/HAI819I_MoteurJeux/TP1/build/external/glfw-3.1.2/examples/CMakeFiles/wave.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/glfw-3.1.2/examples/CMakeFiles/wave.dir/depend

