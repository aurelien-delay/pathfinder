#include "catch.hpp"
#include "../pathfinder.hpp"

using namespace std;

TEST_CASE("Map - is cell out of bounds ?")
{
  const unsigned char pMap[] = {0, 0, 0, 0,
                                0, 0, 0, 0};
  Map _map(pMap, 4, 2);
  CHECK(_map.isCellOutOfBounds(Coordinates(-1,-1)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 0,-1)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 1,-1)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 2,-1)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 3,-1)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 4,-1)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates(-1, 0)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 4, 0)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates(-1, 1)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 4, 1)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates(-1, 2)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 0, 2)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 1, 2)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 2, 2)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 3, 2)) == true);
  CHECK(_map.isCellOutOfBounds(Coordinates( 4, 2)) == true);

  CHECK(_map.isCellOutOfBounds(Coordinates(0,0)) == false);
  CHECK(_map.isCellOutOfBounds(Coordinates(1,0)) == false);
  CHECK(_map.isCellOutOfBounds(Coordinates(2,0)) == false);
  CHECK(_map.isCellOutOfBounds(Coordinates(3,0)) == false);
  CHECK(_map.isCellOutOfBounds(Coordinates(0,1)) == false);
  CHECK(_map.isCellOutOfBounds(Coordinates(1,1)) == false);
  CHECK(_map.isCellOutOfBounds(Coordinates(2,1)) == false);
  CHECK(_map.isCellOutOfBounds(Coordinates(3,1)) == false);
}

TEST_CASE("Map - is Cell ok, meaning not out of bounds, nor impassable")
{
  const unsigned char pMap[] = {0, 1, 0,
                                1, 0, 1,
                                0, 1, 0};
  Map _map(pMap, 3, 3);
  // out of bounds
  CHECK(_map.isCellOk(Coordinates(-1,-1)) == false);
  CHECK(_map.isCellOk(Coordinates( 0,-1)) == false);
  CHECK(_map.isCellOk(Coordinates( 1,-1)) == false);
  CHECK(_map.isCellOk(Coordinates( 2,-1)) == false);
  CHECK(_map.isCellOk(Coordinates( 3,-1)) == false);
  CHECK(_map.isCellOk(Coordinates(-1, 0)) == false);
  CHECK(_map.isCellOk(Coordinates( 3, 0)) == false);
  CHECK(_map.isCellOk(Coordinates(-1, 1)) == false);
  CHECK(_map.isCellOk(Coordinates( 3, 1)) == false);
  CHECK(_map.isCellOk(Coordinates(-1, 2)) == false);
  CHECK(_map.isCellOk(Coordinates( 3, 2)) == false);
  CHECK(_map.isCellOk(Coordinates(-1, 3)) == false);
  CHECK(_map.isCellOk(Coordinates( 0, 3)) == false);
  CHECK(_map.isCellOk(Coordinates( 1, 3)) == false);
  CHECK(_map.isCellOk(Coordinates( 2, 3)) == false);
  CHECK(_map.isCellOk(Coordinates( 3, 3)) == false);

  // impassable
  CHECK(_map.isCellOk(Coordinates(0,0)) == false);
  CHECK(_map.isCellOk(Coordinates(0,2)) == false);
  CHECK(_map.isCellOk(Coordinates(1,1)) == false);
  CHECK(_map.isCellOk(Coordinates(2,0)) == false);
  CHECK(_map.isCellOk(Coordinates(2,2)) == false);

  // ok
  CHECK(_map.isCellOk(Coordinates(0,1)) == true);
  CHECK(_map.isCellOk(Coordinates(1,0)) == true);
  CHECK(_map.isCellOk(Coordinates(1,2)) == true);
  CHECK(_map.isCellOk(Coordinates(2,1)) == true);


}

TEST_CASE("Map - convert Coordinates to index")
{
  const unsigned char pMap[] = {0, 0, 0, 0,
                                0, 0, 0, 0};
  Map _map(pMap, 4, 2);
  CHECK(_map.coordinatesToIndex(Coordinates(0,0)) == 0);
  CHECK(_map.coordinatesToIndex(Coordinates(1,0)) == 1);
  CHECK(_map.coordinatesToIndex(Coordinates(2,0)) == 2);
  CHECK(_map.coordinatesToIndex(Coordinates(3,0)) == 3);
  CHECK(_map.coordinatesToIndex(Coordinates(0,1)) == 4);
  CHECK(_map.coordinatesToIndex(Coordinates(1,1)) == 5);
  CHECK(_map.coordinatesToIndex(Coordinates(2,1)) == 6);
  CHECK(_map.coordinatesToIndex(Coordinates(3,1)) == 7);
}

TEST_CASE("Map - convert Index to Coordinates")
{
  const unsigned char pMap[] = {0, 0, 0, 0,
                                0, 0, 0, 0};
  Map _map(pMap, 4, 2);
  CHECK(_map.indexToCoordinates(0) == Coordinates(0,0));
  CHECK(_map.indexToCoordinates(1) == Coordinates(1,0));
  CHECK(_map.indexToCoordinates(2) == Coordinates(2,0));
  CHECK(_map.indexToCoordinates(3) == Coordinates(3,0));
  CHECK(_map.indexToCoordinates(4) == Coordinates(0,1));
  CHECK(_map.indexToCoordinates(5) == Coordinates(1,1));
  CHECK(_map.indexToCoordinates(6) == Coordinates(2,1));
  CHECK(_map.indexToCoordinates(7) == Coordinates(3,1));
}

TEST_CASE("Map - distance between 2 cells, ignoring obstacles")
{
  const unsigned char pMap[] = {1, 0, 1,
                                1, 0, 1,
                                1, 0, 1};
  Map _map(pMap, 3, 3);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(0,0)) == 0);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(0,1)) == 1);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(0,2)) == 2);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(1,0)) == 1);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(1,1)) == 2);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(1,2)) == 3);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(2,0)) == 2);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(2,1)) == 3);
  CHECK(_map.distance(Coordinates(0,0), Coordinates(2,2)) == 4);

  CHECK(_map.distance(Coordinates(1,1), Coordinates(0,0)) == 2);
  CHECK(_map.distance(Coordinates(1,1), Coordinates(0,1)) == 1);
  CHECK(_map.distance(Coordinates(1,1), Coordinates(0,2)) == 2);
  CHECK(_map.distance(Coordinates(1,1), Coordinates(1,0)) == 1);
  CHECK(_map.distance(Coordinates(1,1), Coordinates(1,1)) == 0);
  CHECK(_map.distance(Coordinates(1,1), Coordinates(1,2)) == 1);
  CHECK(_map.distance(Coordinates(1,1), Coordinates(2,0)) == 2);
  CHECK(_map.distance(Coordinates(1,1), Coordinates(2,1)) == 1);
  CHECK(_map.distance(Coordinates(1,1), Coordinates(2,2)) == 2);
}

TEST_CASE("Map - find neighbors : for a given Cell, find the adjacent cells that are valid")
{
  const unsigned char pMap[] = {0, 0, 0,
                                0, 1, 0,
                                1, 1, 1,
                                1, 1, 1};
  Map _map(pMap, 3, 4);
  list<Coordinates> result;

  result = _map.findNeighbors(Coordinates(1,1));
  REQUIRE(result.size() == 1);
  CHECK( *next(result.begin(), 0) == Coordinates(1,2) );

  result = _map.findNeighbors(Coordinates(0,2));
  REQUIRE(result.size() == 2);
  CHECK( *next(result.begin(), 0) == Coordinates(0,3) );
  CHECK( *next(result.begin(), 1) == Coordinates(1,2) );

  result = _map.findNeighbors(Coordinates(1,2));
  REQUIRE(result.size() == 4);
  CHECK( *next(result.begin(), 0) == Coordinates(1,1) );
  CHECK( *next(result.begin(), 1) == Coordinates(1,3) );
  CHECK( *next(result.begin(), 2) == Coordinates(0,2) );
  CHECK( *next(result.begin(), 3) == Coordinates(2,2) );

  result = _map.findNeighbors(Coordinates(2,2));
  CHECK( *next(result.begin(), 0) == Coordinates(2,3) );
  CHECK( *next(result.begin(), 1) == Coordinates(1,2) );

  result = _map.findNeighbors(Coordinates(0,3));
  CHECK( *next(result.begin(), 0) == Coordinates(0,2) );
  CHECK( *next(result.begin(), 1) == Coordinates(1,3) );

  result = _map.findNeighbors(Coordinates(1,3));
  CHECK( *next(result.begin(), 0) == Coordinates(1,2) );
  CHECK( *next(result.begin(), 1) == Coordinates(0,3) );
  CHECK( *next(result.begin(), 2) == Coordinates(2,3) );

  result = _map.findNeighbors(Coordinates(2,3));
  CHECK( *next(result.begin(), 0) == Coordinates(2,2) );
  CHECK( *next(result.begin(), 1) == Coordinates(1,3) );
}