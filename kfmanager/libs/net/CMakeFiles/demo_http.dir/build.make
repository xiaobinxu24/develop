# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_COMMAND = /home/xiaobin_xu/xqb/xqb_file/clion-1.2.4/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/xiaobin_xu/xqb/xqb_file/clion-1.2.4/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn

# Include any dependencies generated for this target.
include libs/net/CMakeFiles/demo_http.dir/depend.make

# Include the progress variables for this target.
include libs/net/CMakeFiles/demo_http.dir/progress.make

# Include the compile flags for this target's objects.
include libs/net/CMakeFiles/demo_http.dir/flags.make

libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o: libs/net/http/demo_http.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/demo_http.dir/http/demo_http.cpp.o -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/demo_http.cpp

libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo_http.dir/http/demo_http.cpp.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/demo_http.cpp > CMakeFiles/demo_http.dir/http/demo_http.cpp.i

libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo_http.dir/http/demo_http.cpp.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/demo_http.cpp -o CMakeFiles/demo_http.dir/http/demo_http.cpp.s

libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o.requires

libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o.provides: libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o.provides

libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o


libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o: libs/net/http/ghttp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo_http.dir/http/ghttp.c.o   -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/ghttp.c

libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo_http.dir/http/ghttp.c.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/ghttp.c > CMakeFiles/demo_http.dir/http/ghttp.c.i

libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo_http.dir/http/ghttp.c.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/ghttp.c -o CMakeFiles/demo_http.dir/http/ghttp.c.s

libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o.requires

libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o.provides: libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o.provides

libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o


libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o: libs/net/http/http_base64.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo_http.dir/http/http_base64.c.o   -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_base64.c

libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo_http.dir/http/http_base64.c.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_base64.c > CMakeFiles/demo_http.dir/http/http_base64.c.i

libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo_http.dir/http/http_base64.c.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_base64.c -o CMakeFiles/demo_http.dir/http/http_base64.c.s

libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o.requires

libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o.provides: libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o.provides

libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o


libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o: libs/net/http/http_date.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo_http.dir/http/http_date.c.o   -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_date.c

libs/net/CMakeFiles/demo_http.dir/http/http_date.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo_http.dir/http/http_date.c.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_date.c > CMakeFiles/demo_http.dir/http/http_date.c.i

libs/net/CMakeFiles/demo_http.dir/http/http_date.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo_http.dir/http/http_date.c.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_date.c -o CMakeFiles/demo_http.dir/http/http_date.c.s

libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o.requires

libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o.provides: libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o.provides

libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o


libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o: libs/net/http/http_hdrs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo_http.dir/http/http_hdrs.c.o   -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_hdrs.c

libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo_http.dir/http/http_hdrs.c.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_hdrs.c > CMakeFiles/demo_http.dir/http/http_hdrs.c.i

libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo_http.dir/http/http_hdrs.c.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_hdrs.c -o CMakeFiles/demo_http.dir/http/http_hdrs.c.s

libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o.requires

libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o.provides: libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o.provides

libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o


libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o: libs/net/http/http_req.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo_http.dir/http/http_req.c.o   -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_req.c

libs/net/CMakeFiles/demo_http.dir/http/http_req.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo_http.dir/http/http_req.c.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_req.c > CMakeFiles/demo_http.dir/http/http_req.c.i

libs/net/CMakeFiles/demo_http.dir/http/http_req.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo_http.dir/http/http_req.c.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_req.c -o CMakeFiles/demo_http.dir/http/http_req.c.s

libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o.requires

libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o.provides: libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o.provides

libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o


libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o: libs/net/http/http_resp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo_http.dir/http/http_resp.c.o   -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_resp.c

libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo_http.dir/http/http_resp.c.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_resp.c > CMakeFiles/demo_http.dir/http/http_resp.c.i

libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo_http.dir/http/http_resp.c.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_resp.c -o CMakeFiles/demo_http.dir/http/http_resp.c.s

libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o.requires

libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o.provides: libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o.provides

libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o


libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o: libs/net/http/http_trans.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo_http.dir/http/http_trans.c.o   -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_trans.c

libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo_http.dir/http/http_trans.c.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_trans.c > CMakeFiles/demo_http.dir/http/http_trans.c.i

libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo_http.dir/http/http_trans.c.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_trans.c -o CMakeFiles/demo_http.dir/http/http_trans.c.s

libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o.requires

libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o.provides: libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o.provides

libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o


libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o: libs/net/http/http_uri.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/demo_http.dir/http/http_uri.c.o   -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_uri.c

libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/demo_http.dir/http/http_uri.c.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_uri.c > CMakeFiles/demo_http.dir/http/http_uri.c.i

libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/demo_http.dir/http/http_uri.c.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/http_uri.c -o CMakeFiles/demo_http.dir/http/http_uri.c.s

libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o.requires

libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o.provides: libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o.provides

libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o


libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o: libs/net/CMakeFiles/demo_http.dir/flags.make
libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o: libs/net/http/KFHttp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/demo_http.dir/http/KFHttp.cpp.o -c /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/KFHttp.cpp

libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo_http.dir/http/KFHttp.cpp.i"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/KFHttp.cpp > CMakeFiles/demo_http.dir/http/KFHttp.cpp.i

libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo_http.dir/http/KFHttp.cpp.s"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/http/KFHttp.cpp -o CMakeFiles/demo_http.dir/http/KFHttp.cpp.s

libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o.requires:

.PHONY : libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o.requires

libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o.provides: libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o.requires
	$(MAKE) -f libs/net/CMakeFiles/demo_http.dir/build.make libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o.provides.build
.PHONY : libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o.provides

libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o.provides.build: libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o


# Object files for target demo_http
demo_http_OBJECTS = \
"CMakeFiles/demo_http.dir/http/demo_http.cpp.o" \
"CMakeFiles/demo_http.dir/http/ghttp.c.o" \
"CMakeFiles/demo_http.dir/http/http_base64.c.o" \
"CMakeFiles/demo_http.dir/http/http_date.c.o" \
"CMakeFiles/demo_http.dir/http/http_hdrs.c.o" \
"CMakeFiles/demo_http.dir/http/http_req.c.o" \
"CMakeFiles/demo_http.dir/http/http_resp.c.o" \
"CMakeFiles/demo_http.dir/http/http_trans.c.o" \
"CMakeFiles/demo_http.dir/http/http_uri.c.o" \
"CMakeFiles/demo_http.dir/http/KFHttp.cpp.o"

# External object files for target demo_http
demo_http_EXTERNAL_OBJECTS =

libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/build.make
libs/net/demo_http: common/libkfcommon.a
libs/net/demo_http: common/zlog/liblibzlog.a
libs/net/demo_http: libs/net/CMakeFiles/demo_http.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable demo_http"
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/demo_http.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/net/CMakeFiles/demo_http.dir/build: libs/net/demo_http

.PHONY : libs/net/CMakeFiles/demo_http.dir/build

libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/demo_http.cpp.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/ghttp.c.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/http_base64.c.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/http_date.c.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/http_hdrs.c.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/http_req.c.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/http_resp.c.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/http_trans.c.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/http_uri.c.o.requires
libs/net/CMakeFiles/demo_http.dir/requires: libs/net/CMakeFiles/demo_http.dir/http/KFHttp.cpp.o.requires

.PHONY : libs/net/CMakeFiles/demo_http.dir/requires

libs/net/CMakeFiles/demo_http.dir/clean:
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net && $(CMAKE_COMMAND) -P CMakeFiles/demo_http.dir/cmake_clean.cmake
.PHONY : libs/net/CMakeFiles/demo_http.dir/clean

libs/net/CMakeFiles/demo_http.dir/depend:
	cd /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net /home/xiaobin_xu/xqb/xqb_workspace/kfrouter-conn/libs/net/CMakeFiles/demo_http.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/net/CMakeFiles/demo_http.dir/depend

