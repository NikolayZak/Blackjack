CXX = g++
CXXFLAGS = -Wall -std=c++11 -O3
LDFLAGS = -lsqlite3

TARGET = "BJ_Sim"

OBJ_FILES = Absent_Map.o \
			Card_Shoe.o \
			Library.o \
			Blackjack.o \
			Strategy_Processing.o \
			Stats.o \
			main.o \

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(TARGET).exe *.o
	rm -f *.o