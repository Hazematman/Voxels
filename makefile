SRCS = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
LD_FLAGS = -lsfml-system -lsfml-window -lsfml-graphics -lGLEW -lGL
CC_FLAGS = -g 
TITLE = voxels

$(TITLE): $(OBJS)
	g++ -o $@ $^ $(LD_FLAGS)
%.o: %.cpp
	g++ $(CC_FLAGS) -c $< -o $@

clean:
	rm $(OBJS)
	rm $(TITLE)

run:
	./$(TITLE)
