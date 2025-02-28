#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "StringDataSource.h"

TEST(CSVBusSystem, SimpleTest) {
    auto StopSource = std::make_shared<CStringDataSource>(
        "stop_id,node_id\n"
        "1001,1\n"
        "1002,2\n"
    );
    auto RouteSource = std::make_shared<CStringDataSource>(
        "route,stop_id\n"
        "A,1001\n"
        "A,1002\n"
    );
    
    auto StopReader = std::make_shared<CDSVReader>(StopSource, ',');
    auto RouteReader = std::make_shared<CDSVReader>(RouteSource, ',');
    CCSVBusSystem BusSystem(StopReader, RouteReader);
    
    EXPECT_EQ(BusSystem.StopCount(), 2);
    EXPECT_EQ(BusSystem.RouteCount(), 1);
    
    auto Stop1 = BusSystem.StopByID(1001);
    ASSERT_NE(Stop1, nullptr);
    EXPECT_EQ(Stop1->ID(), 1001);
    EXPECT_EQ(Stop1->NodeID(), 1);
    
    auto Route = BusSystem.RouteByName("A");
    ASSERT_NE(Route, nullptr);
    EXPECT_EQ(Route->Name(), "A");
    EXPECT_EQ(Route->StopCount(), 2);
    EXPECT_EQ(Route->GetStopID(0), 1001);
    EXPECT_EQ(Route->GetStopID(1), 1002);
} 