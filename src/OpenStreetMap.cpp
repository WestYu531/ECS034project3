#include "OpenStreetMap.h"
#include <unordered_map>
#include <vector>

struct COpenStreetMap::SImplementation {
    struct SNode : public CStreetMap::SNode {
        TNodeID DID;
        TLocation DLocation;
        std::unordered_map<std::string, std::string> DAttributes;
        
        TNodeID ID() const noexcept override {
            return DID;
        }
        
        TLocation Location() const noexcept override {
            return DLocation;
        }
        
        std::size_t AttributeCount() const noexcept override {
            return DAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override {
            if(index >= DAttributes.size()) {
                return "";
            }
            auto iter = DAttributes.begin();
            std::advance(iter, index);
            return iter->first;
        }
        
        bool HasAttribute(const std::string &key) const noexcept override {
            return DAttributes.find(key) != DAttributes.end();
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override {
            auto iter = DAttributes.find(key);
            if(iter == DAttributes.end()) {
                return "";
            }
            return iter->second;
        }
    };
    
    struct SWay : public CStreetMap::SWay {
        TWayID DID;
        std::vector<TNodeID> DNodeIDs;
        std::unordered_map<std::string, std::string> DAttributes;
        
        TWayID ID() const noexcept override {
            return DID;
        }
        
        std::size_t NodeCount() const noexcept override {
            return DNodeIDs.size();
        }
        
        TNodeID GetNodeID(std::size_t index) const noexcept override {
            if(index >= DNodeIDs.size()) {
                return InvalidNodeID;
            }
            return DNodeIDs[index];
        }
        
        std::size_t AttributeCount() const noexcept override {
            return DAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override {
            if(index >= DAttributes.size()) {
                return "";
            }
            auto iter = DAttributes.begin();
            std::advance(iter, index);
            return iter->first;
        }
        
        bool HasAttribute(const std::string &key) const noexcept override {
            return DAttributes.find(key) != DAttributes.end();
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override {
            auto iter = DAttributes.find(key);
            if(iter == DAttributes.end()) {
                return "";
            }
            return iter->second;
        }
    };
    
    std::vector<std::shared_ptr<SNode>> DNodes;
    std::vector<std::shared_ptr<SWay>> DWays;
    std::unordered_map<TNodeID, std::shared_ptr<SNode>> DNodeMap;
    std::unordered_map<TWayID, std::shared_ptr<SWay>> DWayMap;
    
    SImplementation(std::shared_ptr<CXMLReader> src) {
        SXMLEntity Entity;
        std::shared_ptr<SNode> CurrentNode;
        std::shared_ptr<SWay> CurrentWay;
        
        while(src->ReadEntity(Entity)) {
            if(Entity.DType == SXMLEntity::EType::StartElement) {
                if(Entity.DNameData == "node") {
                    CurrentNode = std::make_shared<SNode>();
                    CurrentNode->DID = std::stoull(Entity.AttributeValue("id"));
                    CurrentNode->DLocation.first = std::stod(Entity.AttributeValue("lat"));
                    CurrentNode->DLocation.second = std::stod(Entity.AttributeValue("lon"));
                    DNodes.push_back(CurrentNode);
                    DNodeMap[CurrentNode->DID] = CurrentNode;
                }
                else if(Entity.DNameData == "way") {
                    CurrentWay = std::make_shared<SWay>();
                    CurrentWay->DID = std::stoull(Entity.AttributeValue("id"));
                    DWays.push_back(CurrentWay);
                    DWayMap[CurrentWay->DID] = CurrentWay;
                }
                else if(Entity.DNameData == "nd" && CurrentWay) {
                    CurrentWay->DNodeIDs.push_back(std::stoull(Entity.AttributeValue("ref")));
                }
                else if(Entity.DNameData == "tag") {
                    if(CurrentNode) {
                        CurrentNode->DAttributes[Entity.AttributeValue("k")] = Entity.AttributeValue("v");
                    }
                    else if(CurrentWay) {
                        CurrentWay->DAttributes[Entity.AttributeValue("k")] = Entity.AttributeValue("v");
                    }
                }
            }
            else if(Entity.DType == SXMLEntity::EType::EndElement) {
                if(Entity.DNameData == "node") {
                    CurrentNode = nullptr;
                }
                else if(Entity.DNameData == "way") {
                    CurrentWay = nullptr;
                }
            }
        }
    }
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src) {
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap() = default;

std::size_t COpenStreetMap::NodeCount() const noexcept {
    return DImplementation->DNodes.size();
}

std::size_t COpenStreetMap::WayCount() const noexcept {
    return DImplementation->DWays.size();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept {
    if(index >= DImplementation->DNodes.size()) {
        return nullptr;
    }
    return DImplementation->DNodes[index];
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept {
    auto iter = DImplementation->DNodeMap.find(id);
    if(iter == DImplementation->DNodeMap.end()) {
        return nullptr;
    }
    return iter->second;
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept {
    if(index >= DImplementation->DWays.size()) {
        return nullptr;
    }
    return DImplementation->DWays[index];
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept {
    auto iter = DImplementation->DWayMap.find(id);
    if(iter == DImplementation->DWayMap.end()) {
        return nullptr;
    }
    return iter->second;
} 