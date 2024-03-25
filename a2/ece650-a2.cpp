// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <queue>
#include <vector>
using namespace std;

class Graph{
    private:
        vector<int> *adjacentList;
    public:
        void setVector(int n){
            adjacentList = new vector<int>[n];
        }

        void clearVector(){
            adjacentList->clear();
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

        void printShortestDistance(int source, int dest,int noOfVertex)
        {
            int pred[noOfVertex];

            if(!BFS(source,dest,noOfVertex,pred)){
                cerr << "Error: There is no path between the given source and destination\n";
                return;
            }

            if(BFS(source,dest,noOfVertex,pred)){
                vector<int> path;
                int current = dest;
                path.push_back(current);
                while(pred[current]!=-1){
                    path.push_back(pred[current]);
                    current = pred[current];
                }
                
                for (int i = path.size()-1; i>=0 ;i--){
                    if(i == static_cast<int>(path.size()) - 1){
                        cout << path[i];
                    }

                    else{
                        cout << "-"<<path[i];
                    }
                    

                }
            }


            
        }

        bool BFS(int fromVtxNum, int toVtxNum,int noOfVertex,int pred[])
        {
            
            queue<int> queue;

            bool visited[noOfVertex];

            //Standard BFS algorithm.
            for (int i = 0; i < noOfVertex; i++) {
                visited[i] = false;
                pred[i] = -1;
            }

            visited[fromVtxNum] = true;
            queue.push(fromVtxNum);

            while (!queue.empty()){
                int front = queue.front();
                queue.pop();
                for (unsigned int i=0;i<adjacentList[front].size();i++){
                    if (!visited[adjacentList[front][i]]){
                        visited[adjacentList[front][i]] = true;
                        pred[adjacentList[front][i]] = front;
                        queue.push(adjacentList[front][i]);

                        if (adjacentList[front][i]==toVtxNum)
                            return true;
                    }
                }
            }
            return false;
        }

    
};


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
                            cerr << "Error: Invalid edge input"<<endl;
                            verticeSize = 0;
                            g.setVector(0);
                            break;
                        }

                        else{
                            bool flag = g.addEdges(vertex_1,vertex_2);
                            if(!flag){
                                cerr << "Error:Edges already exists"<<endl;
                                verticeSize = 0;
                                g.setVector(0);
                                break;
                            }
                            
                        }
                        
                    }  
                  }  
            }

            else if (command.compare("s")==0){

                string source;
                string destination;
                
                //Getting the source vertex
                data >> source;
                int src = stoi(source);
          
                // Getting the destination vertex
                data >> destination;
                int dest = stoi(destination);
            

                try{
                    if(verticeSize == 0){
                        throw runtime_error("Enter No. of vertices");
                    }
                    if(source == destination){
                        throw runtime_error("Invalid source and destination");
                    }
                    g.printShortestDistance(src,dest,size);
                }

                catch(exception& e){
                    cerr<< "ERROR:"<< e.what() <<endl;
                }

                
            }
            }
        catch(...){
            cerr<<"Error: Invalid input"<<endl;
        }

        
    }
    return 0;
}

