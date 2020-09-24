CXX = g++
CPP_FLAGS = -g3 -O0 -pthread -I .
CPP_LIBS = -lOpenCL

CPP_SRCS = $(wildcard *.cpp)
CPP_SRCS += $(wildcard */*.cpp)
CPP_SRCS += $(wildcard */*.c)
CPP_HDRS = $(wildcard *.h)
CPP_HDRS += $(wildcard */*.h)

OBJS = $(patsubst src/%,bin/%.o,$(CPP_SRCS))

REQUIRED_DIRS = bin

all: bin/pomockl bin/kernel.cl

bin/pomockl: $(OBJS)
	$(CXX) $(CPP_FLAGS) $^ -o $@ $(CPP_LIBS)


bin/%.o: src/% $(CPP_HDRS)
	$(CXX) $(CPP_FLAGS) -o $@ -c $<


bin/kernel.cl: kernel.cl
	@cp kernel.cl bin/kernel.cl


.PHONY: all remake clean run debug printvars

remake: clean all

run: all
	@cd bin && ./pomockl

printvars:
	@echo "Sources: $(CPP_SRCS)"
	@echo "Objs: $(OBJS)"
	@echo "Headers: $(CPP_HDRS)"

debug: all
	@export POCL_DEVICES="basic" && export POCL_LEAVE_KERNEL_COMPILER_TEMP_FILES=1 && cd bin && gdb --args ./pomockl

clean:
	@echo Cleaning Binaries...
	@rm -f -r bin/*
	@mkdir -p $(REQUIRED_DIRS)

$(shell mkdir -p $(REQUIRED_DIRS))
