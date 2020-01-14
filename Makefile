PROJ_NAME = bbudp

PROJ_DIR = $(shell pwd)
PROJ_SRCDIR = $(PROJ_DIR)/src
PROJ_OBJDIR = $(PROJ_DIR)/obj
PROJ_BINDIR = $(PROJ_DIR)/bin
PROJ_TESTDIR = $(PROJ_DIR)/Test

CXX = g++
OPTIM = -O2

CFLAGS = -Wall -Wextra -m64 -std=c++11 -pthread -w -g -fPIC -O3
LINKFLAGS = 

TRADIR = /opt/autotrade
INCDIR = -I$(PROJ_SRCDIR) -I$(TRADIR)/include -I$(PROJ_TESTDIR)  
LIBDIR = $(TRADIR)/lib
API_INCLUDES=$(wildcard $(TRADIR)/include/*.h)

LIBS = -lpthread -lssl -lcrypto -lrt

COMPILE = $(CXX) -c $(CFLAGS) $(INCDIR)
LINK = $(CXX) $(LINKFLAGS) $(LIBDIR)

SUFFIXES += .d

uniq = $(if $1, $(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

SOURCES = $(shell find $(PROJ_SRCDIR) -name "*.cpp")
SRC_FILES = $(SOURCES:$(PROJ_SRCDIR)/%=%)
SRC_OBJS = $(SRC_FILES:%.cpp=$(PROJ_DIR)/obj/%.o)
OBJ_SUBDIRS = $(call uniq, $(dir $(SRC_OBJS)))
SRC_DEPS = $(SOURCES:%.cpp=%.d)

BASE_OBJS = $(shell find $(PROJ_OBJDIR)/Postman -name "*.o") $(shell find $(PROJ_OBJDIR)/Security -name "*.o") $(shell find $(PROJ_OBJDIR)/Rudp -name "*.o") 

TEST_SRCS = $(shell find $(PROJ_TESTDIR) -name "*.cpp")
TEST_OBJS = $(patsubst %.cpp, $(PROJ_TESTDIR)/%.o, $(notdir $(TEST_SRCS)))
TEST_DEPS = $(TEST_SRCS:%.cpp=%.d)
TEST_BINS = $(wildcard $(PROJ_BINDIR)/*test*)

API_INCLUDES = $(wildcard $(TRADIR)/include/*.h) $(wildcard $(TRADIR)/include/xtp/*.h)

DEPS = $(SRC_DEPS) $(TEST_DEPS)

echo: 
	echo $(TEST_OBJS)
	echo $(TEST_BINS)
	echo $(SOURCES) 
	echo $(SRC_FILES)
	echo $(SRC_OBJS)
	echo $(SRC_DEPS)
	echo $(OBJ_SUBDIRS)

testclean:
	rm -f $(TEST_BINS) $(TEST_OBJS) 

objclean:
	rm -f $(SRC_OBJS) $(TEST_OBJS)

depclean:
	rm -f $(DEPS)

clean: testclean objclean depclean

$(PROJ_SRCDIR)/%.d: $(PROJ_SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(INCDIR) -MM $< -MT "$(PROJ_OBJDIR)/$*.o" -MF $@

$(PROJ_TESTDIR)/%.d: $(PROJ_TESTDIR)/%.cpp
	$(CXX) $(CFLAGS) $(INCDIR) -MM $< -MT "$(PROJ_TESTDIR)/$*.o" -MF $@

ifneq "$(strip $(DEPS))" ""
  include $(DEPS)
endif

$(PROJ_OBJDIR)/%.o: $(PROJ_SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(INCDIR) -c -o $@ $<

$(PROJ_TESTDIR)/%.o: $(PROJ_TESTDIR)/%.cpp
	$(CXX) $(CFLAGS) $(INCDIR) -c -o $@ $<

depend: $(DEPS) 

objdir: 
	mkdir -p $(PROJ_BINDIR) $(OBJ_SUBDIRS)

obj: objdir $(SRC_OBJS) 

test%: $(PROJ_TESTDIR)/Test%.o obj
	$(eval DEP_FILES := $(patsubst %.hpp, %.cpp, $(patsubst %.H, %.cpp, $(shell sed 's/^.*://;s/\\//g' $(<:%.o=%.d)))))
	$(eval DEP_FILES := $(foreach f,$(filter-out $(API_INCLUDES), $(call uniq, $(DEP_FILES))), $(wildcard $(f))))
	$(eval DEP_OBJS := $(DEP_FILES:%.cpp=%.o))
	$(eval DEP_OBJS := $(filter-out $(SRC_OBJS), $(DEP_OBJS:$(PROJ_SRCDIR)/%.o=$(PROJ_OBJDIR)/%.o)))
	$(CXX) $(LINKFLAGS) -o $(PROJ_BINDIR)/$@ $(BASE_OBJS) $(patsubst %.cpp, %.o, $(DEP_OBJS)) $(LIBS)
