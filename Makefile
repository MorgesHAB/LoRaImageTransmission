# LoRa Image Transmission
# Lionel Isoz

EXE_NAME1=TRANSMIT
EXE_NAME2=RECEIVER
CXX=g++
CPP_TRANSMIT= Transmitter.cpp define.h
CPP_RECEIIVER= Receiver.cpp define.h
OFILES = $(CPPFILES:.cpp=.o)  # Take all the .o file associated to the .cc

INCLUDES_PATH=-I radioheadLib/
CPPFLAGS=-std=c++11 -g -Wall -DRH_PLATFORM=RH_PLATFORM_RPI -D__RASPBERRY_PI_ $(INCLUDES_PATH)
LDFLAGS=-lwiringPi

RH95_SRCS=lib/radiohead/RH_RF95.cpp \
	lib/radiohead/RHGenericSPI.cpp \
	lib/radiohead/RHLinuxSPI.cpp \
 	lib/radiohead/RHGenericDriver.cpp
RH95_OBJS=$(subst .cpp,.o,$(RH95_SRCS))

all: $(EXE_NAME1)

$(EXE_NAME1): $(OFILES) $(RH95_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

Transmitter:
	$(CXX) $(CPPFLAGS) $(CPP_TRANSMIT)-o $(EXE_NAME1)

Receiver:
	$(CXX) $(CPPFLAGS) $(CPP_RECEIIVER)-o $(EXE_NAME2)

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CC) -MM $(CFLAGS) $(CFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	rm -f $(EXE_NAME1) $(EXE_NAME2) $(RH95_OBJS) $(OFILES)
