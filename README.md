# Project 3 - Bus System and OpenStreetMap Parser

This project implements a bus system and OpenStreetMap parser in C++. It builds upon Project 2's DSV and XML parsers to handle bus route information and map data.

## Components

### Bus System Components
- CCSVBusSystem: Reads and manages bus system data from CSV files
- Handles bus stops and routes
- Maps stops to OSM nodes

### OpenStreetMap Components
- COpenStreetMap: Reads and manages OpenStreetMap XML data
- Handles nodes and ways
- Supports attributes for both nodes and ways

### DSV Components
- CDSVReader: Reads delimiter-separated value files
- CDSVWriter: Writes delimiter-separated value files
- Supports custom delimiters
- Handles quoted values and escaping

### XML Components
- CXMLReader: Reads XML files using the Expat library
- CXMLWriter: Writes XML files with proper formatting
- Supports XML attributes and nested elements
- Handles character data and special characters

### Utility Components
- CDataSource: Abstract base class for data input
- CDataSink: Abstract base class for data output
- CStringDataSource: String-based implementation of CDataSource
- CStringDataSink: String-based implementation of CDataSink

## Building and Testing

### Prerequisites
- C++17 compatible compiler
- Google Test framework
- Expat XML library
- Make build system

### Build Instructions
1. Ensure all prerequisites are installed
2. Run `make` to build all components
3. Run `make test` to execute all unit tests
4. Run `make clean` to remove build artifacts

### Test Executables
- teststrutils: Tests string utility functions
- teststrdatasource: Tests string data source
- teststrdatasink: Tests string data sink
- testdsv: Tests DSV reader and writer
- testxml: Tests XML reader and writer
- testcsvbs: Tests CSV bus system
- testosm: Tests OpenStreetMap parser

## Implementation Details

### DSV Format Support
- Handles comma-separated values (CSV)
- Supports custom delimiters
- Properly handles quoted fields
- Escapes double quotes within fields
- Supports empty fields and lines

### XML Format Support
- Full XML element support (start/end/complete)
- Attribute handling
- Character data support
- Special character escaping
- Proper indentation

## External Libraries Used
- Google Test (gtest) for unit testing
- Expat for XML parsing

## File Structure
- include/: Header files
- src/: Implementation files
- testsrc/: Test files
- docs/: Documentation
- data/: Sample data files
- Makefile: Build system

## Documentation
Detailed documentation for each component can be found in the docs/ directory:
- DSVReader.md
- DSVWriter.md
- XMLReader.md
- XMLWriter.md
- CSVBusSystem.md
- OpenStreetMap.md
