#include "robot.hpp"

using namespace okapi::literals;

void generatePaths(){
    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {3.15_ft, 0_ft, 0_deg}}, "1A", {0.8, 0.8,10});
    robot::chassisProfiler->storePath("/usd/tt/paths/1A", "1A");
    robot::chassisProfiler->removePath("1A");

    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {0.75_ft, 0_ft, 0_deg}}, "1B", {0.45,1,10});
    robot::chassisProfiler->storePath("/usd/tt/paths/1B", "1B");
    robot::chassisProfiler->removePath("1B");

    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {2.4_ft, 1.5_ft, 45_deg}}, "1C", {0.8,0.65,10});
    robot::chassisProfiler->storePath("/usd/tt/paths/1C", "1C");
    robot::chassisProfiler->removePath("1C");

    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {1.5_ft, 0_ft, 0_deg}}, "2A", {1,1,10});
    robot::chassisProfiler->storePath("/usd/tt/paths/2A", "2A");
    robot::chassisProfiler->removePath("2A");

    robot::chassisProfiler->generatePath({{0_ft, 0_ft, 0_deg}, {2.6_ft, 0_ft, 0_deg}}, "2B", {0.85,0.8,10});
    robot::chassisProfiler->storePath("/usd/tt/paths/2B", "2B");
    robot::chassisProfiler->removePath("2B");
}