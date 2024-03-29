# LoRa Image Transmission
# Lionel Isoz
#######################################################
CXX=g++
CPPFILES= Transmitter.cpp Receiver.cpp
OFILES = $(CPPFILES:.cpp=.o)
EXE_Tx=Transmitter
EXE_Rx=Receiver
LibPath=radioheadLib/
#######################################################
CPPFLAGS=-std=c++11 -g -Wall -DRH_PLATFORM=RH_PLATFORM_RPI -D__RASPBERRY_PI_ -I $(LibPath)
LDFLAGS=-lwiringPi

RH95_SRCS=$(LibPath)RH_RF95.cpp \
	$(LibPath)RHGenericSPI.cpp \
	$(LibPath)RHLinuxSPI.cpp \
 	$(LibPath)RHGenericDriver.cpp
RH95_OBJS=$(subst .cpp,.o,$(RH95_SRCS))
#######################################################
all:
	@echo "Type : 'make Transmitter' or 'make Receiver'"

# default command
%.o: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

$(EXE_Tx): Transmitter.o $(RH95_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(EXE_Rx): Receiver.o $(RH95_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

Receiver.o: Receiver.cpp define.h
Transmitter.o: Transmitter.cpp define.h

clean:
	rm -f $(RH95_OBJS) $(EXE_Rx) $(EXE_Tx) $(OFILES)
