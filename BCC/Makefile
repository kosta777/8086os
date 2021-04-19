SRC = ..\src\*.cpp 
INCLUDES = ..\h\*.h 
LIBS = ..\BCC\*.lib ..\*.lib 
INCLUDEPATH = C:\bc31\include;..\h;
LIBSPATH = C:\bc31\lib;..\BCC;..\;
TARGET = OS.exe
$(TARGET):	$(SRC) $(INCLUDES) configuration.xmi
	bcc.exe -mh -v -e$(TARGET) -I$(INCLUDEPATH) -L$(LIBSPATH) $(SRC) $(LIBS)
all:	$(TARGET)
clean:
	-del *.obj
	-del $(TARGET)
	-del *.asm
configuration.xmi:	
	configuration.xmi