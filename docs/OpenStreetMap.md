# OpenStreetMap Documentation

## Overview
The `COpenStreetMap` class provides functionality to read and manage OpenStreetMap (OSM) XML data. It implements the `CStreetMap` interface and handles nodes and ways.

## Class Definition
```cpp
class COpenStreetMap : public CStreetMap {
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
        
    public:
        COpenStreetMap(std::shared_ptr<CXMLReader> src);
        ~COpenStreetMap();
        
        std::size_t NodeCount() const noexcept override;
        std::size_t WayCount() const noexcept override;
        std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept override;
        std::shared_ptr<SNode> NodeByID(TNodeID id) const noexcept override;
        std::shared_ptr<SWay> WayByIndex(std::size_t index) const noexcept override;
        std::shared_ptr<SWay> WayByID(TWayID id) const noexcept override;
};
```

## Input Format
```xml
<?xml version='1.0' encoding='UTF-8'?>
<osm version="0.6">
    <node id="1" lat="38.5" lon="-121.7"/>
    <node id="2" lat="38.6" lon="-121.8">
        <tag k="name" v="Davis"/>
    </node>
    <way id="10">
        <nd ref="1"/>
        <nd ref="2"/>
        <tag k="highway" v="residential"/>
    </way>
</osm>
```

## Usage Example
```cpp
// Create data source and reader
auto Source = std::make_shared<CStringDataSource>(osmXmlData);
auto Reader = std::make_shared<CXMLReader>(Source);

// Create OpenStreetMap
COpenStreetMap Map(Reader);

// Get node information
auto Node = Map.NodeByID(1);
if(Node) {
    auto Location = Node->Location();
    std::cout << "Node at: " << Location.first << "," << Location.second << std::endl;
    
    if(Node->HasAttribute("name")) {
        std::cout << "Name: " << Node->GetAttribute("name") << std::endl;
    }
}

// Get way information
auto Way = Map.WayByID(10);
if(Way) {
    for(size_t i = 0; i < Way->NodeCount(); i++) {
        std::cout << "Node: " << Way->GetNodeID(i) << std::endl;
    }
}
```

## Error Handling
- Invalid node IDs return nullptr
- Invalid way IDs return nullptr
- Invalid indices return nullptr
- All member functions are noexcept ~