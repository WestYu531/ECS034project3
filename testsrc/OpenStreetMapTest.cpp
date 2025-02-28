#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "XMLReader.h"
#include "StringDataSource.h"

TEST(OpenStreetMap, SimpleTest) {
    auto Source = std::make_shared<CStringDataSource>(
        "<?xml version='1.0' encoding='UTF-8'?>"
        "<osm version=\"0.6\">"
        "<node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>"
        "<node id=\"2\" lat=\"38.6\" lon=\"-121.8\">"
        "    <tag k=\"name\" v=\"Davis\"/>"
        "</node>"
        "<way id=\"10\">"
        "    <nd ref=\"1\"/>"
        "    <nd ref=\"2\"/>"
        "    <tag k=\"highway\" v=\"residential\"/>"
        "</way>"
        "</osm>"
    );
    
    auto Reader = std::make_shared<CXMLReader>(Source);
    COpenStreetMap Map(Reader);
    
    EXPECT_EQ(Map.NodeCount(), 2);
    EXPECT_EQ(Map.WayCount(), 1);
    
    auto Node1 = Map.NodeByID(1);
    ASSERT_NE(Node1, nullptr);
    EXPECT_EQ(Node1->ID(), 1);
    auto Location1 = Node1->Location();
    EXPECT_DOUBLE_EQ(Location1.first, 38.5);
    EXPECT_DOUBLE_EQ(Location1.second, -121.7);
    EXPECT_EQ(Node1->AttributeCount(), 0);
    
    auto Node2 = Map.NodeByID(2);
    ASSERT_NE(Node2, nullptr);
    EXPECT_EQ(Node2->AttributeCount(), 1);
    EXPECT_TRUE(Node2->HasAttribute("name"));
    EXPECT_EQ(Node2->GetAttribute("name"), "Davis");
    
    auto Way = Map.WayByID(10);
    ASSERT_NE(Way, nullptr);
    EXPECT_EQ(Way->NodeCount(), 2);
    EXPECT_EQ(Way->GetNodeID(0), 1);
    EXPECT_EQ(Way->GetNodeID(1), 2);
    EXPECT_TRUE(Way->HasAttribute("highway"));
    EXPECT_EQ(Way->GetAttribute("highway"), "residential");
} 