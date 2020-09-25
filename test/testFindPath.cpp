#include "catch.hpp"
#include "../pathfinder.hpp"

using namespace std;

TEST_CASE("findPath - Small map with no obstacle")
{
  const Coordinates start(0,0);
  const Coordinates target(1,1);
  const int mapWidth  = 2;
  const int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1,
                                           1, 1};
  const int outBufferSize = 10;
  int outputBuffer[outBufferSize];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  REQUIRE(length == 2);
  CHECK(outputBuffer[0] == 2);
  CHECK(outputBuffer[1] == 3);
}

TEST_CASE("findPath - Small map with 1 obstacle")
{
  const Coordinates start(0,0);
  const Coordinates target(1,1);
  const int mapWidth  = 2;
  const int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1,
                                           0, 1};
  const int outBufferSize = 10;
  int outputBuffer[outBufferSize];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  REQUIRE(length == 2);
  CHECK(outputBuffer[0] == 1);
  CHECK(outputBuffer[1] == 3);
}

TEST_CASE("findPath - Small map with no obstacle, not enough buffer for output")
{
  const Coordinates start(0,0);
  const Coordinates target(1,1);
  const int mapWidth  = 2;
  const int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1,
                                           1, 1};
  const int outBufferSize = 1;
  int outputBuffer[outBufferSize];
  int bufferInit = outputBuffer[0];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  CHECK(length == 2);
  CHECK(outputBuffer[0] == bufferInit); // check outputBuffer was not changed
}

TEST_CASE("findPath - Small map with no possible path")
{
  const Coordinates start(0,0);
  const Coordinates target(1,1);
  const int mapWidth  = 2;
  const int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 0,
                                           0, 1};
  const int outBufferSize = 1;
  int outputBuffer[outBufferSize];
  int bufferInit = outputBuffer[0];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  CHECK(length == -1);
  CHECK(outputBuffer[0] == bufferInit); // check outputBuffer was not changed
}

TEST_CASE("findPath - Start and Target are the same location")
{
  const Coordinates start(0,0);
  const Coordinates target(0,0);
  const int mapWidth  = 2;
  const int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1,
                                           1, 1};
  const int outBufferSize = 1;
  int outputBuffer[outBufferSize];
  int bufferInit = outputBuffer[0];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  REQUIRE(length == 0);
  CHECK(outputBuffer[0] == bufferInit); // check outputBuffer was not changed
}

TEST_CASE("findPath - Example provided by instructions 1")
{
  const Coordinates start(0,0);
  const Coordinates target(1,2);
  const int mapWidth  = 4;
  const int mapHeight = 3;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
  const int outBufferSize = 12;
  int outputBuffer[outBufferSize];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  REQUIRE(length == 3);
  CHECK(outputBuffer[ 0] == 1);
  CHECK(outputBuffer[ 1] == 5);
  CHECK(outputBuffer[ 2] == 9);
}

TEST_CASE("findPath - Example provided by instructions 2")
{
  const Coordinates start(2,0);
  const Coordinates target(0,2);
  const int mapWidth  = 3;
  const int mapHeight = 3;
  unsigned char pMap[mapWidth*mapHeight] = {0, 0, 1, 0, 1, 1, 1, 0, 1};
  const int outBufferSize = 7;
  int outputBuffer[outBufferSize];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  CHECK(length == -1);
}

TEST_CASE("findPath - Complex path")
{
  const Coordinates start(0,9);
  const Coordinates target(9,3);
  const int mapWidth  = 10;
  const int mapHeight = 10;
  unsigned char pMap[mapWidth*mapHeight] ={0, 1, 0, 1, 1, 1, 1, 1, 0, 1,
                                           0, 1, 0, 1, 0, 0, 0, 0, 0, 1,
                                           1, 1, 0, 1, 0, 1, 1, 1, 0, 1,
                                           1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
                                           1, 1, 0, 1, 0, 0, 0, 1, 0, 1,
                                           1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
                                           1, 1, 0, 0, 1, 0, 1, 1, 0, 1,
                                           1, 1, 1, 0, 1, 1, 0, 1, 1, 1,
                                           1, 0, 1, 1, 0, 1, 0, 0, 0, 1,
                                           1, 1, 0, 1, 1, 1, 0, 0, 0, 1};
  const int outBufferSize = 100;
  int outputBuffer[outBufferSize];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  REQUIRE(length == 33);
  CHECK(outputBuffer[ 0] == 80);
  CHECK(outputBuffer[ 1] == 70);
  CHECK(outputBuffer[ 2] == 71);
  CHECK(outputBuffer[ 3] == 72);
  CHECK(outputBuffer[ 4] == 82);
  CHECK(outputBuffer[ 5] == 83);
  CHECK(outputBuffer[ 6] == 93);
  CHECK(outputBuffer[ 7] == 94);
  CHECK(outputBuffer[ 8] == 95);
  CHECK(outputBuffer[ 9] == 85);
  CHECK(outputBuffer[10] == 75);
  CHECK(outputBuffer[11] == 74);
  CHECK(outputBuffer[12] == 64);
  CHECK(outputBuffer[13] == 54);
  CHECK(outputBuffer[14] == 53);
  CHECK(outputBuffer[15] == 43);
  CHECK(outputBuffer[16] == 33);
  CHECK(outputBuffer[17] == 34);
  CHECK(outputBuffer[18] == 35);
  CHECK(outputBuffer[19] == 25);
  CHECK(outputBuffer[20] == 26);
  CHECK(outputBuffer[21] == 27);
  CHECK(outputBuffer[22] == 37);
  CHECK(outputBuffer[23] == 47);
  CHECK(outputBuffer[24] == 57);
  CHECK(outputBuffer[25] == 67);
  CHECK(outputBuffer[26] == 77);
  CHECK(outputBuffer[27] == 78);
  CHECK(outputBuffer[28] == 79);
  CHECK(outputBuffer[29] == 69);
  CHECK(outputBuffer[30] == 59);
  CHECK(outputBuffer[31] == 49);
  CHECK(outputBuffer[32] == 39);
}

TEST_CASE("findPath - Several paths, only one is the shortest path")
{
  const Coordinates start(0,0);
  const Coordinates target(9,9);
  const int mapWidth  = 10;
  const int mapHeight = 10;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
                                           1, 0, 0, 0, 0, 1, 1, 1, 0, 1,
                                           1, 0, 0, 0, 0, 1, 0, 0, 0, 1,
                                           1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
                                           0, 0, 0, 1, 0, 1, 1, 1, 0, 1,
                                           1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
                                           1, 0, 0, 1, 0, 1, 1, 1, 0, 1,
                                           1, 0, 1, 1, 1, 1, 1, 1, 0, 1,
                                           1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const int outBufferSize = 100;
  int outputBuffer[outBufferSize];

  const int length = FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize);
  REQUIRE(length == 20);
  CHECK(outputBuffer[ 0] ==  1);
  CHECK(outputBuffer[ 1] ==  2);
  CHECK(outputBuffer[ 2] ==  3);
  CHECK(outputBuffer[ 3] ==  4);
  CHECK(outputBuffer[ 4] ==  5);
  CHECK(outputBuffer[ 5] == 15);
  CHECK(outputBuffer[ 6] == 16);
  CHECK(outputBuffer[ 7] == 17);
  CHECK(outputBuffer[ 8] ==  7);
  CHECK(outputBuffer[ 9] ==  8);
  CHECK(outputBuffer[10] ==  9);
  CHECK(outputBuffer[11] == 19);
  CHECK(outputBuffer[12] == 29);
  CHECK(outputBuffer[13] == 39);
  CHECK(outputBuffer[14] == 49);
  CHECK(outputBuffer[15] == 59);
  CHECK(outputBuffer[16] == 69);
  CHECK(outputBuffer[17] == 79);
  CHECK(outputBuffer[18] == 89);
  CHECK(outputBuffer[19] == 99);
}




TEST_CASE("findPath - Bad map size input, throw exception")
{
  Coordinates start(-1,0);
  Coordinates target(1,1);
  int mapWidth  = 0;
  int mapHeight = 2;
  unsigned char* pMap = nullptr;
  const int outBufferSize = 10;
  int outputBuffer[outBufferSize];

  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), map width must be greater than 0.\n");

  mapWidth = 2;
  mapHeight = 0;
  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), map height must be greater than 0.\n");
}

TEST_CASE("findPath - Bad Start location input, throw exception")
{
  Coordinates start(-1,0);
  Coordinates target(1,1);
  int mapWidth  = 2;
  int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1,
                                           1, 1};
  const int outBufferSize = 10;
  int outputBuffer[outBufferSize];

  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Start's abscissa must be greater or equal to 0.\n");

  start = Coordinates(2,0);
  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Start's abscissa must be less than the map width.\n");

  start = Coordinates(0,-1);
  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Start's ordinate must be greater or equal to.\n");

  start = Coordinates(0,2);
  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Start's ordinate must be less than the map height.\n");
}

TEST_CASE("findPath - Bad Target location input, throw exception")
{
  Coordinates start(0,0);
  Coordinates target(-1,1);
  int mapWidth  = 2;
  int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1,
                                           1, 1};
  const int outBufferSize = 10;
  int outputBuffer[outBufferSize];

  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Target's abscissa must be greater or equal to 0.\n");

  target = Coordinates(2,0);
  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Target's abscissa must be less than the map width.\n");

  target = Coordinates(0,-1);
  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Target's ordinate must be greater or equal to.\n");

  target = Coordinates(0,2);
  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Target's ordinate must be less than the map height.\n");
}

TEST_CASE("findPath - Bad output buffer size input, throw exception")
{
  Coordinates start(0,0);
  Coordinates target(1,1);
  int mapWidth  = 2;
  int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1,
                                           1, 1};
  const int outBufferSize = -1;
  int* outputBuffer = nullptr;

  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), output buffer size must be greater than 0.\n");
}

TEST_CASE("findPath - Start location is not passable, throw exception")
{
  Coordinates start(0,0);
  Coordinates target(1,1);
  int mapWidth  = 2;
  int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={0, 1,
                                           1, 1};
  const int outBufferSize = 10;
  int outputBuffer[outBufferSize];

  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Start point must be passable.\n");
}

TEST_CASE("findPath - Target location is not passable, throw exception")
{
  Coordinates start(0,0);
  Coordinates target(1,1);
  int mapWidth  = 2;
  int mapHeight = 2;
  unsigned char pMap[mapWidth*mapHeight] ={1, 1,
                                           1, 0};
  const int outBufferSize = 10;
  int outputBuffer[outBufferSize];

  CHECK_THROWS_WITH( FindPath(start.X, start.Y, target.X, target.Y, pMap, mapWidth, mapHeight, outputBuffer, outBufferSize),
                     "in FindPath(), Target point must be passable.\n");
}