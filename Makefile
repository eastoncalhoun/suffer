CORES := -j$(shell nproc)
CXX := g++
IFLAGS := -Iinclude
CXXFLAGS := $(IFLAGS) -O2
SRCcpp := $(shell find src/ -name "*.cpp")
SRCcc := $(shell find src/ -name "*.cc")
SRCcxx := $(shell find src/ -name "*.cxx") 
SRCpp := $(shell find src/ -name "*.c++")
SRCc := $(shell find src/ -name "*.c")
SRC := $(SRCcpp) $(SRCcc) $(SRCcxx) $(SRCpp) $(SRCc) 
OBJcpp := $(patsubst src/%.cpp, build/%.o, $(SRCcpp))
OBJcc := $(patsubst src/%.cc, build/%.o, $(SRCcc))
OBJcxx := $(patsubst src/%.cxx, build/%.o, $(SRCcxx))
OBJpp := $(patsubst src/%.c++, build/%.o, $(SRCpp))
OBJc := $(patsubst src/%.c, build/%.o, $(SRCc))
OBJ := $(OBJcpp) $(OBJcc) $(OBJcxx) $(OBJpp) $(OBJc)  

MAKEFLAGS += $(CORES)

build: out/suffer
	@echo Binary written to out/suffer

out/suffer: $(OBJ)
	@echo Linking $(OBJ)

	@mkdir -p out
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@

build/%.o: src/%.cpp
	@echo Compiling $<

	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

	@echo Compiled $<

install: out/suffer
	@if [ $$UID -eq 0 ]; then \
		echo You must run this install script as a normal user; \
		exit 1; \
	fi

	@echo Installing suffer... This will copy ./out/suffer to /usr/bin/suffer. It will use sudo, and immediately destroy sudos cache afterwards
	
	@echo - sudo cp ./out/suffer /usr/bin
	@sudo cp ./out/suffer /usr/bin

	@echo - sudo -k
	@sudo -k
	
	@mkdir -p $(HOME)/.suffer/cache
	@mkdir -p $(HOME)/.suffer/libs
	@mkdir -p $(HOME)/.suffer/scripts
	@cp ./data/known.json $(HOME)/.suffer/
	@cp ./data/config.json $(HOME)/.suffer/
	@cp -R ./scripts/ $(HOME)/.suffer/

	@echo Suffer has been installed!

clean:
	@rm -rf build/
	@rm -rf out/

	@echo Build Cleaned

config:
	@echo Make Configuration
	@echo
	@echo Cores: $(shell nproc)
	@echo Compiler: $(CXX)
	@echo Flags: $(CXXFLAGS)
	@echo Include Flags: $(IFLAGS)
	@echo Link Static Flags: $(LDFLAGS)
	@echo Link Library Flags: $(LDLIBS) 
	@echo
	@echo Current Compile Command: $(CXX) $(CXXFLAGS) -c src/foo/bar.cpp -o build/foo/bar.o
	@echo
	@echo Current Link Command: $(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o out/suffer
	@echo
	@echo Found Source: $(SRC)
	@echo
	@echo Needed Objects: $(OBJ)
	@echo

help:
	@echo Defined Targets: help, build, install, clean, config
