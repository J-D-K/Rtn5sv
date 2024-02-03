CC			:=	gcc
CXX			:=	g++

INLCUDE		:= include
SOURCE		:= source

CFLAGS		:= -I$(INLCUDE) -Wall -O2
CXXFLAGS	:= $(CFLAGS)

LIBS		:= -lz

CFILES		:= $(foreach dir,$(SOURCE), $(wildcard $(dir)/*.c))
CPPFILES	:= $(foreach dir,$(SOURCE), $(wildcard $(dir)/*.cpp))
OFILES		:= $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

TARGET		:= rtn5sv

$(TARGET) : $(OFILES)
	$(CXX) -o $@ $(OFILES) $(LIBS)

%o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

%o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	@rm -f $(OFILES) $(TARGET)

echo:
	@echo "CFILES: " $(CFILES)
	@echo "CPPFILES: " $(CPPFILES)
	@echo "OFILES: " $(OFILES)
