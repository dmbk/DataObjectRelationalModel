
#BOOST_ROOT := /home/dulitha/boost/1_71_0/gcc/5.4.0/
BOOST_DIR := $(BOOST_ROOT)/include
CXX      := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -std=c++14 -g 
LDFLAGS  := -L/usr/lib -lstdc++ -lm -L $(BOOST_ROOT)/lib -lboost_serialization

BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := program
INCLUDE  := -IDORM/include -Iinclude/ -I$(BOOST_DIR)
SRC      :=                      \
   $(wildcard src/*.cpp)         \

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o) 

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	g++ -o  $(APP_DIR)/$(TARGET) $(SRC) $(INCLUDE) $(CXXFLAGS) $(LDFLAGS)
	#$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)  

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

