#include "Edges.h"
#include <stdexcept>
#include <fstream>
using namespace std;

Edges::Edges(){}

Edges::Edges(int v, double w){
    vertex = v;
    weight = w;
}
int Edges::getVertex(){
    return vertex;
}

double Edges::getWeight(){
    return weight;
}
void Edges::setVertex(int v1){
	vertex = v1;
}
