#include "CSVBusSystem.h"
#include <unordered_map>
#include <vector>

struct CCSVBusSystem::SImplementation {
    struct SStop : public CBusSystem::SStop {
        TStopID DID;
        CStreetMap::TNodeID DNodeID;
        
        TStopID ID() const noexcept override {
            return DID;
        }
        
        CStreetMap::TNodeID NodeID() const noexcept override {
            return DNodeID;
        }
    };
    
    struct SRoute : public CBusSystem::SRoute {
        std::string DName;
        std::vector<TStopID> DStopIDs;
        
        std::string Name() const noexcept override {
            return DName;
        }
        
        std::size_t StopCount() const noexcept override {
            return DStopIDs.size();
        }
        
        TStopID GetStopID(std::size_t index) const noexcept override {
            if(index >= DStopIDs.size()) {
                return InvalidStopID;
            }
            return DStopIDs[index];
        }
    };
    
    std::vector<std::shared_ptr<SStop>> DStops;
    std::vector<std::shared_ptr<SRoute>> DRoutes;
    std::unordered_map<TStopID, std::shared_ptr<SStop>> DStopMap;
    std::unordered_map<std::string, std::shared_ptr<SRoute>> DRouteMap;
    
    SImplementation(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc) {
        // Skip header row
        std::vector<std::string> Row;
        stopsrc->ReadRow(Row);
        
        // Read stops
        while(stopsrc->ReadRow(Row)) {
            if(Row.size() >= 2) {
                auto Stop = std::make_shared<SStop>();
                Stop->DID = std::stoull(Row[0]);
                Stop->DNodeID = std::stoull(Row[1]);
                DStops.push_back(Stop);
                DStopMap[Stop->DID] = Stop;
            }
        }
        
        // Skip header row
        routesrc->ReadRow(Row);
        
        // Read routes
        std::shared_ptr<SRoute> CurrentRoute;
        while(routesrc->ReadRow(Row)) {
            if(Row.size() >= 2) {
                auto RouteName = Row[0];
                auto StopID = std::stoull(Row[1]);
                
                if(!CurrentRoute || CurrentRoute->DName != RouteName) {
                    CurrentRoute = std::make_shared<SRoute>();
                    CurrentRoute->DName = RouteName;
                    DRoutes.push_back(CurrentRoute);
                    DRouteMap[RouteName] = CurrentRoute;
                }
                
                CurrentRoute->DStopIDs.push_back(StopID);
            }
        }
    }
};

CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc) {
    DImplementation = std::make_unique<SImplementation>(stopsrc, routesrc);
}

CCSVBusSystem::~CCSVBusSystem() = default;

std::size_t CCSVBusSystem::StopCount() const noexcept {
    return DImplementation->DStops.size();
}

std::size_t CCSVBusSystem::RouteCount() const noexcept {
    return DImplementation->DRoutes.size();
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept {
    if(index >= DImplementation->DStops.size()) {
        return nullptr;
    }
    return DImplementation->DStops[index];
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept {
    auto iter = DImplementation->DStopMap.find(id);
    if(iter == DImplementation->DStopMap.end()) {
        return nullptr;
    }
    return iter->second;
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept {
    if(index >= DImplementation->DRoutes.size()) {
        return nullptr;
    }
    return DImplementation->DRoutes[index];
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept {
    auto iter = DImplementation->DRouteMap.find(name);
    if(iter == DImplementation->DRouteMap.end()) {
        return nullptr;
    }
    return iter->second;
} 