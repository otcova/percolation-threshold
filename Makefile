SOURCES := $(shell find codi/ -name "*.cpp")
OBJECTS := $(SOURCES:codi/%.cpp=build/%.o)

CPP_FLAGS := -Wall -std=c++17 -O3


build: build/main.exe

run: build
	./build/main.exe

build/main.exe: $(OBJECTS)
	@mkdir -p $(dir $@)
	g++ $(CPP_FLAGS) -o $@ $(OBJECTS)

build/%.o: codi/%.cpp
	@mkdir -p $(dir $@)
	g++ $(CPP_FLAGS) -MMD -o $@ -c $<

generator:
	python3 ./codi/graph/generador_grafos.py 

clean:
	rm -r ./build/

-include $(OBJECTS:.o=.d)

