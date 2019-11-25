#include "robot.hpp"

using namespace okapi::literals;

void generatePaths(){
    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {40_in, 0_in, 0_deg}}, "cubeAdvance", {0.4, 1, 5.0});
    robot::chassisProfiler->storePath("/usd/tt/paths/cubeAdvance", "cubeAdvance");
    robot::chassisProfiler->removePath("cubeAdvance");

    robot::chassisProfiler->generatePath({{0_in, 0_in, 0_deg}, {48_in, 0_in, 0_deg}}, "smallZoneAdvance", {0.6, 0.8, 5.0});
    robot::chassisProfiler->storePath("/usd/tt/paths/smallZoneAdvance", "smallZoneAdvance");
    robot::chassisProfiler->removePath("smallZoneAdvance");
}