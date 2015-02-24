#include "Graph.h"
using namespace std;
//Construct
Graph::Graph(Type t){
	directed = t;   
	numVertices = 0;
	numEdges = 0;
}
//Delete
Graph::~Graph(){}
//Read A Graph From A File
void Graph::readFromFile(string file){
	ifstream in;
	in.open(file);
	int vec1 = 0;
	int vec2 = 0;
	double weight = 0;
	//read file line by line, word by word
	if(in.is_open()){
        	string line, word;
        	stringstream ss;
        	in >> word;
		transform(word.begin(), word.end(), word.begin(), ::tolower);
        	if(word == "directed"){
        		directed = true;
        	}if(word == "undirected"){
        	        directed = false;
        	}
        	in >> numVertices;
        	in >> numEdges;
		numEdges = 0;
        	firstVector.resize(numVertices + 1);
		undirectedVector.resize(numVertices + 1);
        	while(getline(in,line)){
        	    	in >> vec1 >> vec2 >> weight;
        	    	addEdge(vec1, vec2, weight);
        	}
        	ss.clear();
    	}	
    //if file cannot be opened
    	else{
        	cerr<<"Invalid file name."<<endl;
        	throw out_of_range("File name out of range");
    	}
    	in.close();

}
//This function actually didn't change. by adding a undirected vector i found a way to screw with the
//logic. This still works how we want it to (to my knowledge)
void Graph::writeToFile(string file){
    	ofstream outFile(file);
    	string directString = "undirected";
    	if(directed){
    		directString = "directed";
    	}
    	outFile << directString << endl;
    	outFile << numVertices << endl;
    	outFile << numEdges<<endl;
    	for(int i = 0; i <= numVertices; i++){
        	for(unsigned int j = 0; j < firstVector[i].size(); j++){
			outFile<< i << " " << firstVector[i][j].getVertex() << " "<< firstVector[i][j].getWeight()<< endl;
		}
	}outFile.close();
}
//Empty
bool Graph::empty(){
	if(numVertices != 0){
		return 1;
	}return 0;
}
//I added the ability to add edges for our undirected graph but I didn't do edge cases yet.
void Graph::addEdge(int v1, int v2, double weight){
	bool inGraph = false;
	if(v1 <= 0 || v2 <= 0 || weight <= 0 || v1 > numVertices || v2 > numVertices || v1 == v2){
		cerr << "\n Invalid Input. \n";
	}else{
		for(unsigned int i = 0; i < firstVector[v1].size(); i++){
			if(firstVector[v1][i].getVertex() == v2){
				inGraph = true;
			}
		}if(!directed){
			for(unsigned int j = 0; j< firstVector[v2].size(); j++){
				if(firstVector[v2][j].getVertex() == v1){
					inGraph = true;
				}
			}
		}if(!inGraph){
			firstVector[v1].push_back(Edges(v2, weight));
			undirectedVector[v1].push_back(Edges(v2, weight));
			numEdges++;
			inGraph = false;
		}for(unsigned int p = 0; p < undirectedVector[v2].size(); p++){
			if(undirectedVector[v2][p].getVertex() == v1){
				inGraph = true;
			}
		}if(!inGraph){
			undirectedVector[v2].push_back(Edges(v1, weight));
			inGraph = false;
		}
	}
}
//Add Vertex
void Graph::addVertex(){
	    numVertices++;
	    firstVector.resize(numVertices + 1);
	    undirectedVector.resize(numVertices + 1);
}
//This utilizes my undirected Depth First Traversal to check how many 
//components are connected, and then it'll grab each vertex by itself 
//to check how many disjoint sets there are. This needs to work on an
//undirected graph because it is easier to find the connected components
//that way. If the graph is directed, then this function won't work 
//correctly and with my example it will leave 6 out of the file.
int Graph::numConnectedComponents(){
	int retVal = 0;
	vector<bool> unvisited(undirectedVector.size(), true);
	for(int i = 1; i <= numVertices; i++){
		if(unvisited[i]){
			retVal++;
			numConnectedComponentsHelper(i, unvisited);
		}
	}return retVal;
}
//Tree Check
bool Graph::tree(){
	if(numVertices == (numEdges+1) && this->numConnectedComponents() == 1){
		return true;
	}else{
		return false;
	}
}
//This function gets called ONLY by numConnectedComponents.
//This function acts the same as the DFT except it doesn't 
//print to a file
void Graph::numConnectedComponentsHelper(int start, vector<bool> & unvisited){
	unvisited[start] = false;
	vector<Edges>::iterator i;
	for(i = undirectedVector[start].begin(); i != undirectedVector[start].end(); ++i){
		if(unvisited[i->getVertex()]){
			numConnectedComponentsHelper(i->getVertex(), unvisited);
		}
	}
}
//Depth First Traverse
void Graph::DFT(int source, string file){
	if(source <= numVertices && source > 0){
		vector<int> depthTrace;
		ofstream outFile2(file);
		vector<bool> unvisited(firstVector.size(), true);	
		if(directed){
			DFTAux(source, unvisited, depthTrace, firstVector);
		}else{
			DFTAux(source, unvisited, depthTrace, undirectedVector);
		}depthTrace.push_back(source);
		for(unsigned int i = 0; i < depthTrace.size(); i++){
			outFile2 << depthTrace[i] << endl;
		}
	}else{
		cerr<<"\nInvalid input.\n"<<endl;
	}
}
//Graph DFT helper method
void Graph::DFTAux(int start, vector<bool> & unvisited, vector<int> & depthTrace, vector<vector<Edges>> & testVec){
	unvisited[start] = false;
	vector<Edges>::iterator i;
	for(i = testVec[start].begin(); i != testVec[start].end(); ++i){
		if(unvisited[i->getVertex()]){
			DFTAux(i->getVertex(), unvisited, depthTrace, testVec);
			depthTrace.push_back(i->getVertex());
		}
	}
}
//Breadth First Traverse
void Graph::BFT(int source, string file){
	if(source <= numVertices && source > 0){
		ofstream outfile(file);
		bool visited[numVertices+1];
		for(int i = 0; i < numVertices + 1; i++){
			visited[i] = false;
		}visited[source] = true;
		queue<int> holder;
		holder.push(source);
		while(!holder.empty()){
			source = holder.front();
			outfile<<source<<endl;
			if(firstVector[source].size() != 0){
				if(directed){
					for(unsigned int i = 0; i < firstVector[source].size(); i++){
						if(!visited[firstVector[source][i].getVertex()]){
							visited[firstVector[source][i].getVertex()] = true;
							holder.push(firstVector[source][i].getVertex());
						}
					}
				}else{
					for(unsigned int i = 0; i < undirectedVector[source].size(); i++){
						if(!visited[undirectedVector[source][i].getVertex()]){
							visited[undirectedVector[source][i].getVertex()] = true;
							holder.push(undirectedVector[source][i].getVertex());
						}
					}
				}
			}holder.pop();
		}outfile.close();
	}else{
		cerr<<"\nInvalid input. Exiting program.\n";
	}
}
int Graph::closenessHelper(int source, int end){
	if(source == end){
		return 0;
	}
	int retVal = 0;
	vector<int> distLabel(numVertices + 1, -1);
	vector<int> predLabel(numVertices + 1);
	distLabel[source] = 0;
	int distance = 0;
	queue<int> vertexQueue;
	vertexQueue.push(source);
	int vertex;
	while(distLabel[end] < 0 && !vertexQueue.empty()){
		vertex = vertexQueue.front();
		vertexQueue.pop();
		if(distLabel[vertex] > distance){
			distance++;
		}for(vector<Edges>::iterator it = firstVector[vertex].begin(); it != firstVector[vertex].end(); it++){
			if(distLabel[it->getVertex()] < 0){
				distLabel[it->getVertex()] = distance + 1;
				predLabel[it->getVertex()] = vertex;
				vertexQueue.push(it->getVertex());
			}
		}
	}distance++;
	vector<int> path(distance + 1);
	if(distLabel[end] < 0){
		return -1;
	}else{
		path[distance] = end;
		int distanceCount = 0;
		for(int k = distance - 1; k >= 0; k--){
			path[k] = predLabel[path[k+1]];
			distanceCount++;
		}retVal = distanceCount;
	}return retVal;
}
//Closeness
//this isn't working yet.
int Graph::closeness(int v1, int v2){
	if(v1 <= numVertices && v1 >= 0){
		int retVal1 = closenessHelper(v1, v2);
		int retVal2 = closenessHelper(v2, v1);
		if (retVal1 == -1){
			if(retVal2 != -1){
				return retVal2;
			}
			else{
				return -1;
			}
		}
		return retVal1;
	}else{
		return -1;
	}
}
//* Partition
bool Graph::partitionable(){
	int source = 1;
	int count = 1;
	string colored[numVertices + 1];
	bool visited[numVertices + 1];
	colored[1] = "BLUE";
	for(int i = 0; i < numVertices + 1; i++){
		visited[i] = false;
	}
	visited[source] = true;
	queue<int> holder;
	holder.push(source);
	while(!holder.empty() && source <= numVertices){
		source = holder.front();
		if(firstVector[source].size() != 0){
			for(unsigned int i = 0; i < firstVector[source].size(); i++){
				if(!visited[firstVector[source][i].getVertex()]){
					visited[firstVector[source][i].getVertex()] = true;
					if(firstVector[source].size() > 1){
						if(colored[source] == "BLUE"){
							colored[firstVector[source][i].getVertex()] = "RED";
							count++;
						}else if(colored[source] == "RED"){
							colored[firstVector[source][i].getVertex()] = "BLUE";
							count--;
						}holder.push(firstVector[source][i].getVertex());
						if(colored[source] == colored[firstVector[source][i].getVertex()]){
							return false;
						}
					}else{
						if(count == 1){
							colored[firstVector[source][i].getVertex()] = "RED";
							count++;
						}else if(count == 2){
							colored[firstVector[source][i].getVertex()] = "BLUE";
							count--;
						}
						holder.push(firstVector[source][i].getVertex());
						if(colored[source] == colored[firstVector[source][i].getVertex()]){
							return false;
						}
					}
				}else if(colored[source] == colored[firstVector[source][i].getVertex()]){
					return false;
				}
			}
		}else{
			source++;
			if(count == 1){
				colored[source] = "RED";
				count++;
			}else if(count == 2){
				colored[source] = "BLUE";
				count--;
			}holder.push(source);
			visited[source] = true;
		}holder.pop();
	}
	return true;
}
int Graph::root(int i){
	while(i != parentVector[i]){
		i = parentVector[i];
	}return i;
}
bool Graph::find(int p, int q){
	return root(p) == root(q);
}
void Graph::unite(int p, int q){
	int i = root(p);
	int j = root(q);
	parentVector[i] = j;
}
void Graph::setEdges(int source, vector<bool>& unvisited, queueType& temp){
	Line tempLine;
	unvisited[source] = false;
	queue<int> holder;
	holder.push(source);
	while(!holder.empty()){
		source = holder.front();
		unvisited[source] = false;
		//cout<<"SOURCE: "<<source<<endl;
		//cout<<"SIZE: " <<undirectedVector[source].size()<<endl;
		if(undirectedVector[source].size() != 0){
			for(unsigned int i = 0; i < undirectedVector[source].size(); i++){
				if(unvisited[undirectedVector[source][i].getVertex()]){
		//			cout<<"                 " << source<< " " << undirectedVector[source][i].getVertex()<<endl;
					if(directed){
		//				cout<< i << " " << firstVector[source].size()<<endl;
						if(i < firstVector[source].size()){
							if(firstVector[source][i].getVertex() == undirectedVector[source][i].getVertex()){
								tempLine.v2 = undirectedVector[source][i].getVertex();
								tempLine.v1 = source;
								tempLine.weight = undirectedVector[source][i].getWeight();
								temp.push(tempLine);
								holder.push(undirectedVector[source][i].getVertex());
						//		unvisited[undirectedVector[source][i].getVertex()] = false;
							}else{
								tempLine.v1 = undirectedVector[source][i].getVertex();
								tempLine.v2 = source;
								tempLine.weight = undirectedVector[source][i].getWeight();
								temp.push(tempLine);
								holder.push(undirectedVector[source][i].getVertex());
						//		unvisited[undirectedVector[source][i].getVertex()] = false;
							}
						}else{
							tempLine.v1 = undirectedVector[source][i].getVertex();
							tempLine.v2 = source;
							tempLine.weight = undirectedVector[source][i].getWeight();
							temp.push(tempLine);
							holder.push(undirectedVector[source][i].getVertex());
						//	unvisited[undirectedVector[source][i].getVertex()] = false;
						}
					}else{
						//cout<<"SOURCE: "<<source<<endl;
						//cout<<"V2: " << undirectedVector[source][i].getVertex()<<endl;
						tempLine.v1 = source;
						tempLine.v2 = undirectedVector[source][i].getVertex();
						tempLine.weight = undirectedVector[source][i].getWeight();
						temp.push(tempLine);
						holder.push(undirectedVector[source][i].getVertex());
						//unvisited[undirectedVector[source][i].getVertex()] = false;
					}
				}	
			}
		}
		if(!holder.empty()){holder.pop();}
	}
}	
void Graph::MSTHelper(int start, vector<bool> & unvisited, ofstream& outfile){
	queueType temp;
	Line tempLine;
	vector<Line> edgesVector;
	setEdges(start, unvisited, temp);
	parentVector.resize(numVertices + 1);
	for(int i = 1; i <= numVertices; i++){
		parentVector[i] = i;
	}vector<int> vertexVec(numVertices + 1, 0);
	int i = 0;
	edgesVector.resize(numEdges);
	while(!temp.empty()){
		tempLine = temp.top();
		temp.pop();
		if(!find(tempLine.v1, tempLine.v2)){
			//cout<<tempLine.v1 << " " << tempLine.v2 << " " << tempLine.weight<< endl;
			vertexVec[tempLine.v1] = tempLine.v1;
			vertexVec[tempLine.v2] = tempLine.v2;
			edgesVector[i] = tempLine;
			i++;
			unite(tempLine.v1, tempLine.v2);
		}
	}outfile<<"{ {";
	for(unsigned int i = 0; i < vertexVec.size(); i++){
		if(i == vertexVec.size() - 1){
			outfile<< i<<"}, { ";
		}else if(vertexVec[i] > 0){
			outfile<< i<<", ";
		}
	}for(unsigned int i = 0; i < edgesVector.size(); i++){
		//cout<<"                      " << edgesVector[i].v1 << " " << edgesVector[i].v2 <<" "<< edgesVector[i].weight<<endl;
		if(i != 0 && edgesVector[i].v1 != 0 && edgesVector[i].v2 != 0 && edgesVector[i].weight != 0){
			outfile<<", ";
		}
		if(i == edgesVector.size() - 1 && edgesVector[i].v1 != 0 && edgesVector[i].v2 != 0 && edgesVector[i].weight != 0){
			outfile<<"("<<edgesVector[i].v1<<","<<edgesVector[i].v2<<","<<edgesVector[i].weight<<") }";
		}else if(i == edgesVector.size() - 1){
			outfile<<" } }";
		}else if(edgesVector[i].v1 != 0 && edgesVector[i].v2 != 0 && edgesVector[i].weight != 0){
			outfile<<"("<<edgesVector[i].v1<<","<<edgesVector[i].v2<<","<<edgesVector[i].weight<<")";
		}
	}
}
//MST
bool Graph::MST(string file){
	ofstream outfile(file);
	bool retVal = false;
	vector<bool> unvisited(undirectedVector.size(), true);
	for(int i = 0; i < numConnectedComponents(); i++){
		for(unsigned int j = 1; j < firstVector.size();j++){
			if(unvisited[j]){
				retVal = true;
				MSTHelper(j, unvisited, outfile);
				outfile<<endl;
				j = firstVector.size();
			}
		}
	}outfile.close();
	return retVal;
}
//* Step Away
void Graph::stepAway(int source, int closeness, string file){
	if(source <= numVertices && source >= 0 && closeness >=-1){
		ofstream outfile(file);
		for(int i = 1; i <= numVertices; i++){
			if(this->closeness(source, i) == closeness){
				outfile<<i<<endl;
			}
		}
	}else{
		cerr<<"\nInvalid Input.\n";
	}
}
