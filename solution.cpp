#include <list>
#include <map>
#include <queue>
#include <ctime>
#include <exception>
#include <cstdlib>
#include <cassert>

using namespace std;

// ############################################################################
// ### Unit tests
// ############################################################################

// Unit tests were written using Catch2 framework.
// They cannot be submitted with the solution, but they can be found here :
// https://github.com/aurelien-delay/pathfinder/tree/master/test

// ############################################################################
// ### In multi thread environment
// ############################################################################

// While the algo does not use multiple threads, FindPath() could be called
// in several threads with some shared data.
// The function has been designed to be reentrant. Meaning it is safe for the
// calling code to call it in several threads as long as they ensure :
// - pMap is const in all threads
// - pOutBuffer is not shared (each thread has its own instance)

// Making the function thread safe, i.e. supporting that pMap and pOutBuffer
// are shared among several threads would necessitate to just lock the full function
// and disabling thus parrarel execution. 
// Ensuring Reentrancy was deemed enough.

// ############################################################################
// ### HEADERS
// ############################################################################

/*! \brief  API to implement, finding the shortest path between Start and Target
 *          in the input map.
 * 
 *  \return length of the shortest path, or -1 if none can be found.
 *  \throw  BadInputException if the following conditions are not respected in input
 *          1≤nMapWidth,nMapHeight
 *          0≤nStartX,nTargetX<nMapWidth
 *          0≤nStartY,nTargetY<nMapHeight
 *          Both Start and Target are empty locations
 *          nOutBufferSize≥0.
 */
int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY, 
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize);

/*! \brief Coordinates on the 2D map.
 *
 *  With appropriate operators in order to be used in maps and queues.
 */
struct Coordinates
{
  Coordinates(): X(-1), Y(-1) {}
  Coordinates(const int X, const int Y): X(X), Y(Y) {}
  int X,Y;
};
bool operator==(const Coordinates& lhs, const Coordinates& rhs);
bool operator!=(const Coordinates& lhs, const Coordinates& rhs);
bool operator<(const Coordinates& lhs, const Coordinates& rhs);

/*! \brief Class describing and providing all operations pertaining to the map. */
class Map
{
  public:
  Map(const unsigned char* pMap, const int nMapWidth, const int nMapHeight): 
    _pMap(pMap), _mapWidth(nMapWidth), _mapHeight(nMapHeight){}

  const list<Coordinates> findNeighbors(const Coordinates& cell) const;

  bool isCellOutOfBounds(const Coordinates& coordCell) const;
  bool isCellOk(const Coordinates& coordCell) const;

  int coordinatesToIndex(const Coordinates& coordinates) const;
  const Coordinates indexToCoordinates(const int index) const;
  int distance(const Coordinates& cellA, const Coordinates& cellB) const;

  private:
  const unsigned char* _pMap;
  int _mapWidth, _mapHeight;
};

/*! \brief Central class that will process A* algorythm to find shortest path  */
class Pathfinder
{
  public:
  Pathfinder(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY, 
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize):
             _start(nStartX, nStartY), _target(nTargetX, nTargetY),
             _map(pMap, nMapWidth, nMapHeight),
             _outBuffer(pOutBuffer), _outBufferSize(nOutBufferSize)
             {}

  int findPath();

  private:
  const map<Coordinates, Coordinates> Astar() const;
  int convertToOutput(const map<Coordinates, Coordinates>& shortestPathMap);

  Coordinates _start, _target;
  Map _map;
  int* _outBuffer;
  int _outBufferSize;
};

/*! \brief Exception to return if the input does not respect the rules  */
class BadInputException : public exception
{
    string _msg;
public:
    BadInputException(const string& msg) : _msg(msg){}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

/*! \brief Override std::priority_queue, to be more user-friendly in code
 *
 *  in case of tie, the queue has a FIFO dequeue order.
 *  ex: PriorityQueue<Coordinates> q;
 *  to enqueue : q.put(Coordinates(0,0), 12);  where 12 is the priority
 *  to dequeue item with lower priority : Coordinates coord = q.dequeue();
 */
bool operator<(const pair<int, time_t>& lhs, const pair<int, time_t>& rhs);
template<typename T>
struct PriorityQueue {
  typedef pair< pair<int, time_t>, T> PQElement;
  priority_queue<PQElement, vector<PQElement>,
                 greater<PQElement>> elements;

  inline bool empty() const {
     return elements.empty();
  }

  inline void put(T item, int priority) {
    time_t tiebreaker = time(nullptr);
    pair<int, time_t> priopair = {priority, tiebreaker};
    elements.emplace(priopair, item);
  }

  T dequeue() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

// ############################################################################
// ### IMPLEMENTATION
// ############################################################################

int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY, 
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize)
{
  // Check input
  if (nMapWidth < 1)          { throw BadInputException("in FindPath(), map width must be greater than 0.\n"); }
  if (nMapHeight < 1)         { throw BadInputException("in FindPath(), map height must be greater than 0.\n"); }
  if (nStartX < 0)            { throw BadInputException("in FindPath(), Start's abscissa must be greater or equal to 0.\n"); }
  if (nStartX >= nMapWidth)   { throw BadInputException("in FindPath(), Start's abscissa must be less than the map width.\n"); }
  if (nStartY < 0)            { throw BadInputException("in FindPath(), Start's ordinate must be greater or equal to.\n"); }
  if (nStartY >= nMapHeight)  { throw BadInputException("in FindPath(), Start's ordinate must be less than the map height.\n"); }
  if (nTargetX < 0)           { throw BadInputException("in FindPath(), Target's abscissa must be greater or equal to 0.\n"); }
  if (nTargetX >= nMapWidth)  { throw BadInputException("in FindPath(), Target's abscissa must be less than the map width.\n"); }
  if (nTargetY < 0)           { throw BadInputException("in FindPath(), Target's ordinate must be greater or equal to.\n"); }
  if (nTargetY >= nMapHeight) { throw BadInputException("in FindPath(), Target's ordinate must be less than the map height.\n"); }
  if (nOutBufferSize < 0)     { throw BadInputException("in FindPath(), output buffer size must be greater than 0.\n"); }
  // Start and Target location passability will be checked later

  Pathfinder pathfinder(nStartX, nStartY, nTargetX, nTargetY, pMap, nMapWidth, nMapHeight, pOutBuffer, nOutBufferSize);
  return pathfinder.findPath();
}

int Pathfinder::findPath()
{
  // finish to check input
  if (!_map.isCellOk(_start))  { throw BadInputException("in FindPath(), Start point must be passable.\n"); }
  if (!_map.isCellOk(_target)) { throw BadInputException("in FindPath(), Target point must be passable.\n"); }

  // Easy case : Target and Start are the same location
  if (_start == _target) { return 0; }

  // Use A* algorythm to fill a "Shortest path map"
  const map<Coordinates, Coordinates> shortestPathMap = Astar();

  // Use "Shortest path map" to build the output - will update pOutBuffer
  const int length = convertToOutput(shortestPathMap);

  return length;
}

const map<Coordinates, Coordinates> Pathfinder::Astar() const
{
  // "Shortest path" map.
  // Will contain for each cell, which is the previous cell in the shortest path.
  // By backtracking from Target to Start, we can reconstitute the shortest path.
  map<Coordinates, Coordinates> shortestPathMap;
  // "Cost From Start" map. 
  // Will contain for each cell the distance from the Start in the shortest path.
  map<Coordinates, int> costFromStart;
  costFromStart[_start] = 0;

  // Using priority queue in order to examine first "the most promising cell"
  // In this case, priority score is the addition of distance of the cell from the Start
  // and of the heuristics of A* algo, i.e. shortest distance without obstacle between the cell and the Target
  // and the queue first dequeues the item with lowest score.
  PriorityQueue<Coordinates> q;
  q.put(_start, 0);

  bool foundTarget = false;
  while( ! q.empty() )
  {
    const Coordinates currentCell = q.dequeue();

    // early exit - as soon as we found a path to the target
    if (currentCell == _target)
    {
      foundTarget = true;
      break;
    }
    
    // Loop on possible adjacent cells
    // Map::findNeighbors() will remove uneligible cells from the list (out of bounds and impassable cells)
    for (const Coordinates& nextCell : _map.findNeighbors(currentCell))
    {
      auto findCurrentCost = costFromStart.find(currentCell);
      assert(findCurrentCost != costFromStart.end());
      int newCost = findCurrentCost->second + 1; // it costs 1 to go from one cell to the next

      // Only examine the next cell it if it's the first time,
      // or if a shorter path from Start cell has been found.
      auto findIt = costFromStart.find(nextCell);
      if (findIt == costFromStart.end() || newCost < findIt->second)
      {
        const int heuristics = _map.distance(nextCell, _target); // distance without obstacle
        int priority = newCost + heuristics;
        q.put(nextCell, priority);
        costFromStart[nextCell] = newCost;
        shortestPathMap[nextCell] = currentCell;
      }
    }
  }

  // return "Shortest Path" map - enough to reconstitute shortest path and its length
  // if we could not find the target, return empty map
  if (!foundTarget)
  {
    shortestPathMap.clear();
  }
  return shortestPathMap;
}

int Pathfinder::convertToOutput(const map<Coordinates, Coordinates>& shortestPathMap)
{
  // if shortest path map is empty, it means there is no possible path.
  // just return -1
  if (shortestPathMap.empty())
  {
    return -1;
  }

  // backtrack from the target to the start
  // first we need to know the length of shortest path
  int length = 0;
  Coordinates currentCell = _target;
  while( currentCell != _start )
  {
    ++length;
    currentCell = shortestPathMap.find(currentCell)->second;
  }

  // if length is shorter than nOutBufferSize, 
  // then backtrack again and fill pOutBuffer (starting from the end to the start)
  if (length <= _outBufferSize)
  {
    currentCell = _target;
    int cursor = 0;
    while( currentCell != _start )
    {
      ++cursor;
      _outBuffer[length-cursor] = _map.coordinatesToIndex(currentCell);
      currentCell = shortestPathMap.find(currentCell)->second;
    }
  }

  return length;
}

bool Map::isCellOutOfBounds(const Coordinates& coordCell) const
{
  return (coordCell.X < 0 || coordCell.X >= _mapWidth || 
          coordCell.Y < 0 || coordCell.Y >= _mapHeight);
}

bool Map::isCellOk(const Coordinates& coordCell) const
{
  // check if cell is out of bounds
  if (isCellOutOfBounds(coordCell))
  {
    return false;
  }

  // check if cell is impassable
  const int indexCell = coordinatesToIndex(coordCell);
  assert(indexCell >= 0 && indexCell < _mapHeight*_mapWidth);
  if (_pMap[indexCell] == 0)
  {
    return false;
  }

  // else the cell is eligible
  return true;
}

const list<Coordinates> Map::findNeighbors(const Coordinates& cell) const
{
  list<Coordinates> outputNeighbors;
  const Coordinates upCell    = Coordinates(cell.X, cell.Y-1);
  const Coordinates downCell  = Coordinates(cell.X, cell.Y+1);
  const Coordinates leftCell  = Coordinates(cell.X-1, cell.Y);
  const Coordinates rightCell = Coordinates(cell.X+1, cell.Y);
  if (isCellOk(upCell))     outputNeighbors.push_back(upCell);
  if (isCellOk(downCell))   outputNeighbors.push_back(downCell);
  if (isCellOk(leftCell))   outputNeighbors.push_back(leftCell);
  if (isCellOk(rightCell))  outputNeighbors.push_back(rightCell);
  return outputNeighbors;
}

int Map::coordinatesToIndex(const Coordinates& coordinates) const
{
  assert(!isCellOutOfBounds(coordinates));
  return (coordinates.Y*_mapWidth + coordinates.X);
}
const Coordinates Map::indexToCoordinates(const int index) const
{
  assert(index>=0 && index < _mapWidth*_mapHeight);
  return Coordinates(index % _mapWidth, index / _mapWidth);
}

int Map::distance(const Coordinates& cellA, const Coordinates& cellB) const
{
  return (abs(cellA.X-cellB.X) + abs(cellA.Y-cellB.Y));
}

bool operator==(const Coordinates& lhs, const Coordinates& rhs)
{
  return ((lhs.X == rhs.X) && (lhs.Y == rhs.Y));
}

bool operator!=(const Coordinates& lhs, const Coordinates& rhs)
{
  return (!(lhs == rhs));
}

bool operator<(const Coordinates& lhs, const Coordinates& rhs)
{
  if (lhs.X < rhs.X)  return true;
  if (lhs.X > rhs.X)  return false;
  if (lhs.Y < rhs.Y)  return true;
  if (lhs.Y > rhs.Y)  return false;
  return false;
}

bool operator<(const std::pair<int, time_t>& lhs, const std::pair<int, time_t>& rhs)
{
  if (lhs.first < rhs.first)  return true;
  if (lhs.first > rhs.first)  return false;
  // for FIFO, we want smaller time to be dequeued first
  if (lhs.second > rhs.second)  return true;
  if (lhs.second < rhs.second)  return false;
  return false;
}