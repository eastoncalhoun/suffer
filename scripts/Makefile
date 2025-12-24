# SUFFER GENERATED

CORES := -j$(shell nproc)
CXX := ^c
IFLAGS := -Iinclude
STD := -std=^s
CXXFLAGS := $(IFLAGS) $(STD) ^f
LDFLAGS := -Llib
LDLIBS := ^l
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

build: out/^n
	@echo Binary written to out/^n

out/^n: $(OBJ)
	@mkdir -p out
	@echo Linking $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

build/%.o: src/%.cpp
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo Compiled $<

build/%.o: src/%.cc
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo Compiled $<

build/%.o: src/%.cxx
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo Compiled $<

build/%.o: src/%.c++
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo Compiled $<

build/%.o: src/%.c
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo Compiled $<

build/%.o: src/%.C
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo Compiled $<

run: out/^n
	./out/^n
	@echo ^n Completed

clean:
	@rm -rf build/
	@rm -rf out/
	@echo Build Cleaned

config:
	@echo Make Configuration
	@echo
	@echo Cores: $(CORES)
	@echo Compiler: $(CXX)
	@echo Flags: $(CXXFLAGS)
	@echo Include Flags: $(IFLAGS)
	@echo Link Static Flags: $(LDFLAGS)
	@echo Link Library Flags: $(LDLIBS) 
	@echo
	@echo Current Compile Command: $(CXX) $(CXXFLAGS) -c src/foo/bar.cpp -o build/foo/bar.o
	@echo
	@echo Current Link Command: $(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o out/^n
	@echo
	@echo Found Source: $(SRC)
	@echo
	@echo Needed Objects: $(OBJ)
	@echo
	@echo Current suffer.project.json
	@cat ./suffer.project.json
	@echo

help:
	@echo Defined Targets: help, build, run, clean, config
