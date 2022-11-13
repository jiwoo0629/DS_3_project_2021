#ifndef SOLUTION_H
#define SOLUTION_H
#define _CRT_SECURE_NO_WARNINGS

#include "Result.h"
#include "Graph.h"
#include <cmath>
#include <set>
#include <queue>
#include <list>
#include <fstream>
#include <string>

class Manager
{
private:
    const char* RESULT_LOG_PATH = "log.txt"; // the filepath for the result log

    ofstream fout; // the file stream for the result log

    Graph m_graph; // graph instance to manage the vertics.

    list<string> reportdata; //string that stores every word in "reportdata.txt"
    int RabinkarpCount; //Count that checks if duplicate title exists
    string Company[7]; //string array that stores name of cities

public:
    ~Manager(); //Destructor
    void Run(const char* filepath); //Run the program using commands in 'filepath'
    void PrintError(Result result); //Print Error Message
    void PrintSuccess(const char* command); //Print Success Message

private:
    Result Load(const char* filepath); //Load the data from 'filepath'
    Result Print(); //Print a mapdata
    Result FindPathBfs(int startVertexKey, int endVertexKey); //find the shortest path from startVertexKey to endVertexKey with BFS
    Result FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey); //find the shortest path from startVertexKey to endVertexKey with Dijkstra using Set
    Result FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey); //find the shortest path from startVertexKey to endVertexKey with Bellmanford using Min Heap
    Result FindShortestPathBellmanFord(int startVertexKey, int endVertexKey); //find the shortest path from startVertexKey to endVertexKey with Bellmanford 
    Result FindShortestPathFloyd(); //find the shortest path for every pair with Floyd
    Result RabinKarpCompare(const char* CompareString, const char* ComparedString); //find the sentence that involves CompareString using RabinKarp algorithm
};

#endif
