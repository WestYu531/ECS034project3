~# CSVBusSystem Documentation

## Overview
The `CCSVBusSystem` class provides functionality to read and manage bus system data from CSV files. It implements the `CBusSystem` interface and handles bus stops and routes.

## Class Definition
```cpp
class CCSVBusSystem : public CBusSystem {
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
        
    public:
        CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc);
        ~CCSVBusSystem();
        
        std::size_t StopCount() const noexcept override;
        std::size_t RouteCount() const noexcept override;
        std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept override;
        std::shared_ptr<SStop> StopByID(TStopID id) const noexcept override;
        std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept override;
        std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept override;
};
```

## Input Format

### Stops CSV Format
```csv
stop_id,node_id
1001,2849810514
1002,2849805223
```

### Routes CSV Format
```csv
route,stop_id
A,1001
A,1002
B,1001
```

## Usage Example
```cpp
// Create data sources and readers
auto StopSource = std::make_shared<CStringDataSource>(stopsCsvData);
auto RouteSource = std::make_shared<CStringDataSource>(routesCsvData);
auto StopReader = std::make_shared<CDSVReader>(StopSource, ',');
auto RouteReader = std::make_shared<CDSVReader>(RouteSource, ',');

// Create bus system
CCSVBusSystem BusSystem(StopReader, RouteReader);

// Get stop information
auto Stop = BusSystem.StopByID(1001);
if(Stop) {
    std::cout << "Stop " << Stop->ID() << " at node " << Stop->NodeID() << std::endl;
}

// Get route information
auto Route = BusSystem.RouteByName("A");
if(Route) {
    for(size_t i = 0; i < Route->StopCount(); i++) {
        std::cout << "Stop: " << Route->GetStopID(i) << std::endl;
    }
}
```

## Error Handling
- Invalid stop IDs return nullptr
- Invalid route names return nullptr
- Invalid indices return nullptr
- All member functions are noexcept ~