//Put any include statements here
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include "Edges.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <queue>
#include <list>
//This class will be used to create a graph library.
struct Line{
	int v1;
	int v2;
	double weight;
};
struct sort_by_weight{
	inline bool operator() (const Line& a, const Line& b){
		return(a.weight > b.weight);
	}
};

//This class will be used to create a graph library.
enum Type {UNDIRECTED, DIRECTED};
typedef std::priority_queue<Line, std::vector<Line>, sort_by_weight> queueType;
class Graph{
    	private:
    	    //Put your private data members here
    	    std::vector<std::vector<Edges>> firstVector;
	    //undirected DFT auxicilary function
	    std::vector<std::vector<Edges>> undirectedVector;
	    void numConnectedComponentsHelper(int start, std::vector<bool> & unvisited);
	    void DFTAux(int start, std::vector<bool> & unvisited, std::vector<int> & depthTrace,std::vector<std::vector<Edges>> & testVec);
		int root(int i);
		bool find(int p, int q);
		void unite(int p, int q);
		int closenessHelper(int source, int end);
		void MSTHelper(int start, std::vector<bool> & unvisited, std::ofstream& outfile);
		void setEdges(int source, std::vector<bool>& unvisited, queueType& temp);
     	    int numVertices;
    	    int numEdges;
    	    bool directed;
 		std::vector<int> parentVector;
 
	public:
		//Construct an empty graph of the specified type
		Graph(Type t);
		//Delete a graph
		~Graph();
		//Read a graph from a file
		void readFromFile(std::string file);
		//Write a graph to a file
		void writeToFile(std::string file);
		//Empty
		bool empty();
		//Add edge
		void addEdge(int v1, int v2, double weight);
		//Add vertex
		void addVertex();
		//Count connected components
		int numConnectedComponents();
		//Tree check
		bool tree();
		//Depth First Traverse - proceed from source
		void DFT(int source, std::string file);
		//Breadth First Traverse - proceed from source
		void BFT(int source, std::string file);
		//Closeness - determine minimum number of edges to get
		// from one node to the other
		int closeness(int v1, int v2);
		//* Partition - determine if you can partition the graph
		bool partitionable();
		//* MST - print the minimum spanning tree of the graph
		//to a file with the passed name (return whether or not
                //this operation was successful)
		bool MST(std::string file);
		//* Step Away - print the nodes who are a degree of
		//closeness from the source to a file with the passed name
		void stepAway(int source, int closeness, std::string file);
};
