


EXE_NAME=LoRaImageTransmission
CXX=g++
CPPFILES = main.cpp  # Take all the .cpp file
OFILES = $(CPPFILES:.cpp=.o)  # Take all the .o file associated to the .cc

INCLUDES_PATH=-I lib/radiohead/
CPPFLAGS=-std=c++11 -g -Wall -DRH_PLATFORM=RH_PLATFORM_RPI -D__RASPBERRY_PI_ $(INCLUDES_PATH)
LDFLAGS=-lwiringPi

RH95_SRCS=lib/radiohead/RH_RF95.cpp \
	lib/radiohead/RHGenericSPI.cpp \
	lib/radiohead/RHLinuxSPI.cpp \
 	lib/radiohead/RHGenericDriver.cpp
RH95_OBJS=$(subst .cpp,.o,$(RH95_SRCS))

all: $(EXE_NAME)

$(EXE_NAME): $(OFILES) $(RH95_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f $(EXE_NAME) $(RH95_OBJS) $(OFILES)