INCLUDE = $(shell pkg-config --cflags opencv)
LIBS = $(shell pkg-config --libs opencv)

TARGET = MyCar
OBJECT = *.o
SOURCES =*.cpp

$(TARGET):$(OBJECT)
	g++ -o $(TARGET) $(OBJECT) -lwiringPi -lpthread $(LIBS) -I $(INCLUDE)

$(OBJECT):$(SOURCES)
	g++ -c $(SOURCES)
	
clean:
	rm -f $(OBJECT) $(TARGET)
rebuild:clean $(TARGET)