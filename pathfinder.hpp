#pragma once
#include <list>
#include <map>
#include <queue>
#include <ctime>
#include <exception>

using namespace std;

// ############################################################################
// ### General Explanations
// ############################################################################

// The purpose is to implement a function FindPath(...) to find one of the
// shortest path between a Start location and a Target location within a
// map composed of cells, some are impassible.

// The algorithm A* was used for this.
// The idea is to begin with Start location, and explore one by one
// the adjacent cells, priorityzing the most promising cells first.
// For each new explored cells, the previous cell in shortest path
// is stored - it will be used to recompute the shortest path at the end.
// The priority score is in party based on an estimation (called heuristics)
// of the distance of the current cell to the Target. 
// Here we choose a heuristics that cannot overestimate the actual distance.
// It can then be proven that A* algorythm is guaranteed to find the
// shortest path.

// Since we prioritized most promising cells, the algo is significantly
// faster than one that would explore the full map.

// More on A* : https://en.wikipedia.org/wiki/A*_search_algorithm

// ############################################################################
// ### Unit tests
// ############################################################################

// Unit tests were written using Catch2 framework.
// They cannot be submitted with the solution, but they can be found here :
// TODO set github and paste link

// ############################################################################
// ### In multi thread environment
// ############################################################################

// While the algo does not use multiple threads, FindPath() could be called
// in several threads with some shared data.
// The function has been designed to be reentrant. Meaning it is safe for the
// calling code to call it in several threads as long as they ensure :
// - pMap is const in all threads
// - pOutBuffer is not shared (each thread has its own instance)

// ############################################################################
// ### HEADERS
// ############################################################################

int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY, 
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize);


struct Coordinates
{
  Coordinates(): X(-1), Y(-1) {}
  Coordinates(const int X, const int Y): X(X), Y(Y) {}
  int X,Y;
};
bool operator==(const Coordinates& lhs, const Coordinates& rhs);
bool operator!=(const Coordinates& lhs, const Coordinates& rhs);
bool operator<(const Coordinates& lhs, const Coordinates& rhs);

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

// --- Override std::priority_queue, to be more user-friendly in code ---
// in case of tie, the queue has a FIFO dequeue order.
// ex: PriorityQueue<Coordinates> q;
// to enqueue : q.put(Coordinates(0,0), 12);  where 12 is the priority
// to dequeue item with lower priority : q.get();
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