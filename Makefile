CXX=g++
CXXFLAGS=-g -Wall -std=c++17 -I include -I /opt/homebrew/include -I /usr/local/include
TESTLDFLAGS=-L/opt/homebrew/lib -L/usr/local/lib -lgtest -lgtest_main -lpthread -lexpat -lstdc++

# Directories
OBJDIR=obj
BINDIR=bin

# Source files
LIBSOURCES=$(wildcard src/*.cpp)
TESTSOURCES=$(wildcard testsrc/*.cpp)

# Object files
LIBOBJECTS=$(LIBSOURCES:src/%.cpp=$(OBJDIR)/%.o)
TESTOBJECTS=$(TESTSOURCES:testsrc/%.cpp=$(OBJDIR)/%.o)

# Test executables
TESTSTRUTILS=$(BINDIR)/teststrutils
TESTSTRDATASOURCE=$(BINDIR)/teststrdatasource
TESTSTRDATASINK=$(BINDIR)/teststrdatasink
TESTDSV=$(BINDIR)/testdsv
TESTXML=$(BINDIR)/testxml

# Add new test executables
TESTCSVBS=$(BINDIR)/testcsvbs
TESTOSM=$(BINDIR)/testosm

# All test executables
TESTS=$(TESTSTRUTILS) $(TESTSTRDATASOURCE) $(TESTSTRDATASINK) $(TESTDSV) $(TESTXML) $(TESTCSVBS) $(TESTOSM)

all: directories $(TESTS)

directories:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

# Test executables
$(TESTSTRUTILS): $(OBJDIR)/StringUtils.o $(OBJDIR)/StringUtilsTest.o
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

$(TESTSTRDATASOURCE): $(OBJDIR)/StringDataSource.o $(OBJDIR)/StringDataSourceTest.o
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

$(TESTSTRDATASINK): $(OBJDIR)/StringDataSink.o $(OBJDIR)/StringDataSinkTest.o
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

$(TESTDSV): $(OBJDIR)/DSVReader.o $(OBJDIR)/DSVWriter.o $(OBJDIR)/StringDataSource.o $(OBJDIR)/StringDataSink.o $(OBJDIR)/DSVTest.o
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

$(TESTXML): $(OBJDIR)/XMLReader.o $(OBJDIR)/XMLWriter.o $(OBJDIR)/StringDataSource.o $(OBJDIR)/StringDataSink.o $(OBJDIR)/XMLTest.o
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

# Add new test targets
$(TESTCSVBS): $(OBJDIR)/CSVBusSystem.o $(OBJDIR)/CSVBusSystemTest.o $(OBJDIR)/StringDataSource.o $(OBJDIR)/DSVReader.o
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

$(TESTOSM): $(OBJDIR)/OpenStreetMap.o $(OBJDIR)/OpenStreetMapTest.o $(OBJDIR)/XMLReader.o $(OBJDIR)/StringDataSource.o
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

# Object files
$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: testsrc/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Run tests
test: all
	./$(TESTSTRUTILS)
	./$(TESTSTRDATASOURCE)
	./$(TESTSTRDATASINK)
	./$(TESTDSV)
	./$(TESTXML)
	./$(TESTCSVBS)
	./$(TESTOSM)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

.PHONY: all clean test directories 