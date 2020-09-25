#pragma once
#include <list>
#include <map>
#include <queue>
#include <ctime>
#include <exception>

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