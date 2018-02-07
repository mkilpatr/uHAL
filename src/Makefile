C             = gcc
CXX           = g++
CCFlags       = -g -O1 -w -Wall -pedantic -std=c++11 -fPIC
CCFlagsRoot   = `root-config --cflags --evelibs --glibs`
Amc13Flag     = -D__AMC13__
DevFlags      =

AMC13DIR=/opt/cactus/include/amc13
IncludeDirs   =  /opt/cactus/include ../ /opt/root_v5.34.14.source/include
IncludePaths  = $(IncludeDirs:%=-I%)

LibraryDirs   = /opt/cactus/lib ../lib /opt/root_v5.34.14.source/lib
LibraryPaths  = $(LibraryDirs:%=-L%)

ifneq ("$(wildcard $(AMC13DIR))","")
	ExternalObjects= $(LibraryPaths) -lpthread  -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal -lboost_system -lboost_filesystem -lboost_regex -lboost_thread -lcactus_amc13_amc13 -lPh1_Interface -lPh1_Description -lPh1_System -lPh1_Utils -lPh1_Amc13 -lHist -lCore -lGpad -lGui -lGX11 -lMatrix -lMathCore -lCint -lGraf -lRIO -lThread -lHistPainter
	ExternalGLIBObjects= $(LibraryPaths) -lpthread  -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal -lboost_system -lboost_filesystem -lboost_regex -lboost_thread -lcactus_amc13_amc13 -lHist -lCore -lGpad -lGui -lGX11 -lMatrix -lMathCore -lCint -lGraf -lRIO -lThread -lHistPainter
	AMC13INSTALLED = yes
	binaries       = fedtester glibsetup setHits
else
	ExternalObjects= $(LibraryPaths) -lpthread  -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal -lboost_system -lboost_filesystem -lboost_regex -lboost_thread -lPh1_Interface -lPh1_Description -lPh1_System -lPh1_Utils -lHist -lCore -lGpad -lGui -lGX11 
	ExternalGLIBObjects= $(LibraryPaths) -lpthread  -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal -lboost_system -lboost_filesystem -lboost_regex -lboost_thread -lcactus_amc13_amc13 -lHist -lCore -lGpad -lGui -lGX11 -lMatrix -lMathCore -lCint -lGraf -lRIO -lThread -lHistPainter
	AMC13INSTALLED = no
	binaries       = fedtester glibsetup setHits
endif


.PHONY: clean $(binaries) 
	#run fpgaconfig amc13test

binaries=fedtester glibsetup setHits


all: $(binaries)

print:
	@echo '*****************************'
	@echo 'Amc13 SW installed:' $(AMC13INSTALLED)
	@echo '*****************************'

fedtester: fedtester.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	cp $@ ../bin

#setHits: setHits.cc
#	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
#	cp $@ ../bin

#glibsetup: glibsetup.cc
#	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalGLIBObjects)
#	cp $@ ../bin

clean:
	rm -f $(binaries) *.o
