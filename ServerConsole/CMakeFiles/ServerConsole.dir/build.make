# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole

# Include any dependencies generated for this target.
include CMakeFiles/ServerConsole.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ServerConsole.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ServerConsole.dir/flags.make

CMakeFiles/ServerConsole.dir/main.cpp.o: CMakeFiles/ServerConsole.dir/flags.make
CMakeFiles/ServerConsole.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ServerConsole.dir/main.cpp.o"
	clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerConsole.dir/main.cpp.o -c /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole/main.cpp

CMakeFiles/ServerConsole.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerConsole.dir/main.cpp.i"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole/main.cpp > CMakeFiles/ServerConsole.dir/main.cpp.i

CMakeFiles/ServerConsole.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerConsole.dir/main.cpp.s"
	clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole/main.cpp -o CMakeFiles/ServerConsole.dir/main.cpp.s

# Object files for target ServerConsole
ServerConsole_OBJECTS = \
"CMakeFiles/ServerConsole.dir/main.cpp.o"

# External object files for target ServerConsole
ServerConsole_EXTERNAL_OBJECTS =

ServerConsole: CMakeFiles/ServerConsole.dir/main.cpp.o
ServerConsole: CMakeFiles/ServerConsole.dir/build.make
ServerConsole: /mnt/c/Apoapse/DEV/BinLinux/libApoapseClientShared.so
ServerConsole: CMakeFiles/ServerConsole.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ServerConsole"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ServerConsole.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ServerConsole.dir/build: ServerConsole

.PHONY : CMakeFiles/ServerConsole.dir/build

CMakeFiles/ServerConsole.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ServerConsole.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ServerConsole.dir/clean

CMakeFiles/ServerConsole.dir/depend:
	cd /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole /mnt/c/Apoapse/DEV/ApoapseServer/ServerConsole/CMakeFiles/ServerConsole.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ServerConsole.dir/depend

