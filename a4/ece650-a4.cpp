// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <minisat/core/Solver.h>
using namespace std;


string replacestring(string allVertices, const string& toReplace, char withCh) {

    for(char c: toReplace) // Loop over each character of the string
    {
        size_t found = allVertices.find(c); //Find the position of the character we want to remove
        while (found != string::npos) {// While our position in the sting is in range.
            allVertices[found] = withCh;// Change the character at position.
            found = allVertices.find(c, found+1);// Find the net occurence of the character.

        }
    }

    return allVertices;// return modified string.

}

vector<string> split(const string& str, char delimiter) {
    vector<string> result;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}
class Graph{
    private:
        vector<int> *adjacentList;
    public:
        // Constructor
        Graph() : adjacentList(nullptr){}

        //Destructor
        ~Graph(){
          //Delete the memory for each vector in adjacency list
          for(vector<int>::size_type i = 0; i < adjacentList->size() + 1; ++i){
            adjacentList[i].clear();
          }
          delete[] adjacentList;
        }

        void setVector(int n){
            adjacentList = new vector<int>[n];
        }

        bool addEdges(int vertex1, int vertex2){
           
            for(unsigned int i = 0; i < adjacentList[vertex1].size();++i){
                    if(adjacentList[vertex1][i] == vertex2){
                        return false;
                        break;
                    }
                }
            adjacentList[vertex1].push_back(vertex2);
            adjacentList[vertex2].push_back(vertex1);
            return true;

        }
};

vector<int> vertex_coversat(int size,int k, vector<int> Vertices)
{
    Minisat::Solver solver;
    vector<vector<Minisat::Lit>> x(size);
    vector<int> first;
    vector<int> last;

    for(unsigned int i = 0; i < size; ++i)
        {
            for(unsigned int j = 0; j < k; ++j)
            {
                Minisat::Lit li = Minisat::mkLit(solver.newVar());
                x[i].push_back(li);
            }
        }

     //At least one vertice is the ith vertex in the vertex cover
     for(unsigned int i = 0; i < k; ++i)
     {
        Minisat::vec<Minisat::Lit> li;
        for(unsigned int j = 0; j < size; ++j)
        {
            li.push(x[j][i]);
        }
        solver.addClause(li);
        li.clear();
     } 

      // No one vertex can  appear twice in vertex cover
      for(unsigned int i = 0; i < size; ++i)
      {
        for(unsigned int j = 0; j < k - 1; j++)
        {
            for(unsigned int q = j+1 ; q < k; ++q)
            {
                solver.addClause(~x[i][j],~x[i][q]);
            }
        }
      }
        //No more than one vertex in two position in vertex cover
        for(unsigned int i = 0; i < k; ++i)
        {
            for(unsigned int j = 0; j < size - 1; ++j)
            {
                for(unsigned int q = j + 1 ; q < size ; ++q)
                {
                    solver.addClause(~x[j][i],~x[q][i]);
                }
            }
        }

        for(unsigned int i = 0; i < Vertices.size(); ++i)
        {
            if( i % 2 == 0)
            {
                first.push_back(Vertices[i]);
            }
            else
            {
                last.push_back(Vertices[i]);
            }
        }

        //Every edge is incident on at least one vertex in vertex cover
        for (unsigned int i = 0; i < first.size(); ++i) 
	    {
            Minisat::vec<Minisat::Lit> li;
            for (unsigned int j = 0; j < k; ++j) 
            {
                li.push(x[first[i]][j]);
                li.push(x[last[i]][j]);
            }
            solver.addClause(li);
            li.clear();
        }

        bool sat = solver.solve();

        if (sat) 
            {
                vector<int> answer;
                for (unsigned int i = 0; i < size; i++) 
                {
                    for (int j = 0; j < k; ++j) 
                    {
                        if (Minisat::toInt(solver.modelValue(x[i][j])) == 0) 
                        {
                            answer.push_back(i);
                        }
                    }
                }
                return answer;
            } else {
                return {-1};
            }

}

Graph g;
int main(int argc, char** argv) {
    // Test code. Replaced with your code
    string input;
    string command;
    string operation;
    int verticeSize = 0;
    int size = 0;
    
    

    while(!cin.eof()){
        try{
            getline(cin,input);
            if(input.empty()){
                continue;
            }

            istringstream data(input,istringstream::in);
            data >> command;
            if(command.compare("V") == 0){
                data >> operation;
                verticeSize = stoi(operation);
                try{
                    if(verticeSize <= 1){
                        throw runtime_error("Invalid input");
                    }
                    size = verticeSize + 1;
                    g.setVector(size);
                }

                catch(exception& e){
                    cerr << "ERROR:" << e.what() <<endl;
                }
                
            }

            else if(command.compare("E") == 0){
                
                string edges;
                bool flag1 = true;
                data >> operation;
                for(unsigned int i = 0; i < operation.length();i++){
                  if(operation[i] == '<'){
                        i++;
                        string edge_1 = "";
                        string edge_2 = "";
                        while(operation[i]!= ',' && operation[i] !='>' ){
                            edge_1+=operation[i];
                            i++;
                        }

                        if(operation[i] == ','){
                            i++;
                            while(i < operation.length() && operation[i] !='>'){
                                edge_2+=operation[i];
                                i++;
                            }
                        }
                      
                        int vertex_1 = stoi(edge_1);
                        int vertex_2 = stoi(edge_2);      
                        if(verticeSize == 0){
                            cerr << "Error:Enter No. of vertices"<<endl;
                            g.setVector(0);
                            break;
                        }
                        else if(vertex_1 > verticeSize or vertex_2 > verticeSize or vertex_1 == vertex_2 or vertex_1<1 or vertex_2 < 1){
                            verticeSize = 0;
                            g.setVector(0);
                            break;
                        }

                        else{
                            bool flag = g.addEdges(vertex_1,vertex_2);
                            if(!flag){
                                verticeSize = 0;
                                g.setVector(0);
                                break;
                            }
                            
                        }
                        
                    }  
                  }
                  edges = replacestring(operation,"{",' ');
                  edges = replacestring(edges,"}",' ');
                  edges = replacestring(edges,"<",' ');
                  edges = replacestring(edges,">",' ');
                  vector<string> vertices = split(edges,','); 
                  vector<int> intVertices;

                  // Converting the modified string vector to an integer vector wherein each element of the vertices will ve converted to an integer. 
                  for(const string& vertice:vertices)
                  {
                    intVertices.push_back(stoi(vertice));
                  }

                  for(unsigned int i = 0 ; i < intVertices.size(); i += 2)
                  {
                    if(i%2 == 0)
                    {
                        if((intVertices[i] > verticeSize || intVertices[i + 1] > verticeSize) || (intVertices[i] == intVertices[i+1]) || (intVertices[i] <= 0 || intVertices[i+1] <= 0))
                        {
                            flag1 = false;
                            cerr<<"Error: Invalid Vertice Input"<<endl;
                            break;
                        }

                    }
                  }


                if(flag1){

                  int low = 1;
                  int high = size;
                  vector<int> answer;
                  vector<int> finalAnswer;

                  while(low<=high)
                  {
                    int k = floor((low+high)/2);
                    answer = vertex_coversat(size,k,intVertices);
                    vector<int> tmp(answer.size(), -1);
                    bool flag = equal(answer.begin(),answer.end(),tmp.begin());

                    if(!flag)
                    {
                        high = k - 1;
                        finalAnswer.clear();
                        finalAnswer = answer;
                    }
                    else{
                        low = k + 1;
                    }
                  }
                  sort(finalAnswer.begin(),finalAnswer.end());
                  for(int v2: finalAnswer)
                  {
                    cout<< v2 << " ";
                  }
                  cout<<"\n";
                }
                    
            }
            }
        catch(...){
            cerr<<"Error: Invalid input"<<endl;
        }

        
    }
    return 0;
}

