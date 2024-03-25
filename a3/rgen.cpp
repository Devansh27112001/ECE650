#include <unistd.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <fcntl.h>
#include <vector>
#include <string>
#define DEF_S_VAL 10
#define DEF_N_VAL 5
#define DEF_L_VAL 5
#define DEF_C_VAL 20
using namespace std;
ifstream urandom("/dev/urandom");
vector<string> streetsData;
struct Point{
  float x;
  float y;
};

//---------------Overlapping lines--------------------/
bool OverlappingLines(int x0, int y0,int x1, int y1,int x2, int y2,int x3, int y3)
{
  Point p0 = {static_cast<float>(x0),static_cast<float>(y0)};
  Point p1 = {static_cast<float>(x1),static_cast<float>(y1)};
  Point p2 = {static_cast<float>(x2),static_cast<float>(y2)};
  Point p3 = {static_cast<float>(x3),static_cast<float>(y3)};
  float minX01 = min(p0.x,p1.x);
  float minY01 = min(p0.y,p1.y);
  float minX23 = min(p2.x,p3.x);
  float minY23 = min(p2.y,p3.y);
  float maxX01 = max(p0.x,p1.x);
  float maxY01 = max(p0.y,p1.y);
  float maxX23 = max(p2.x,p3.x);
  float maxY23 = max(p2.y,p3.y);

  if(p0.x != p1.x && p2.x != p3.x)
  {
    float a = (p1.y - p0.y) / (p1.x - p0.x);
    float b = (p3.y - p2.y) / (p3.x - p2.x);
    if(a == b) //parellel
    {
      float d = abs(p0.y * (p1.x - p0.x) - p0.x * (p1.y - p0.y) - p2.y * (p3.x - p2.x) + p2.x * (p3.y - p2.y));
      if(d == 0) // Overlap
      {// Formula from ece650-a1.py
        if((p2.x > minX01 && p2.x < maxX01 && p2.y > minY01 && p2.y < maxY01) || 
                    (p3.x > minX01 && p3.x < maxX01 && p3.y > minY01 && p3.y < maxY01) ||
                    (p0.x > minX23 && p0.x < maxX23 && p0.y > minY23 && p0.y < maxY23) || 
                    (p1.x > minX23 && p1.x < maxX23 && p1.y > minY23 && p1.y < maxY23)){
                      return true;  
                    }
                    else{
                      return false;
                    }

        
      }
      else{
        return false;
      }
      
    }
    else{
      return false;
    }
  }
  else{
    return false;
  }
}

// -------Checking if two line segments overlap---------/
bool overlapping(vector<int> pointList,vector<vector<int>> allPoints){
   for (auto i = static_cast<size_t>(2); i < pointList.size(); i = i + 2) {
		for (auto j = static_cast<size_t>(0); j < allPoints.size(); j = j + 2) {
			for (auto k = static_cast<size_t>(2); k < allPoints[j].size(); k =  k + 2) {
				if (OverlappingLines(pointList[i], pointList[i + 1],pointList[i-2],pointList[i-1],allPoints[j][k],allPoints[j][k+1],allPoints[j][k-2],allPoints[j][k-1] )) {
					return true;
				}
			}
		}
	}
	return false; 
}

//-----Checking if the point is valid----------/
bool validPoint(vector<int>pointList,int xcor,int ycor)
{
  // First point
  if(pointList.empty())
  {
    return true;
  }

    //Checking for all the points. if any of them are same.
    for(size_t i = 0; i < pointList.size() - 1; i+=2)
    {
      if(xcor == pointList[i] && ycor == pointList[i + 1])
      {
        return false;
      }
    }
  return true;
}


//-------------Getting a random no.------------------
int get_random(int min, int max){
  unsigned int num;
  int file = -1;
  if ((file = open("/dev/urandom", O_RDONLY)) == -1) {
    throw runtime_error("Unable to open /dev/urandom");
    exit(0);
  }
  read(file, &num, sizeof(unsigned int));
  close(file);
  return min + (num % (max - min + 1));
}

//--------------MAIN--------------/
int main(int argc, char **argv){
  //urandom has opened or not.
 if(urandom.fail()){
    cerr<<"Error: Failed to open urandom\n";
    exit(1);
 }
 string cmdValue;
 int noOfStreets = DEF_S_VAL;// -s - Total no. og streets
 int noOfSegments = DEF_N_VAL;// -n - Total no of line-segments for each street
 int waitsec = DEF_L_VAL;// -l - Wait period
 int range = DEF_C_VAL;// -c range of co-ordinates [-20,20]
 int c;
 //https://git.uwaterloo.ca/ece650-1241/cpp/-/blob/main/using_getopt.cpp?ref_type=heads
 while((c = getopt(argc,argv,"s:n:l:c:"))!=-1)
 {
  switch(c)
  {
    case 's':
      cmdValue = optarg;
      noOfStreets = atoi(cmdValue.c_str());
      if(noOfStreets < 2 || optarg[0] == '-')
      {
        cerr<<"Error:Invalid s value"<<endl;
        exit(1);
      }
      break;
    case 'n':
      cmdValue = optarg;
      noOfSegments = atoi(cmdValue.c_str());
      if(noOfSegments < 1 || optarg[0] == '-')
      {
        cerr<<"Error:Invalid n value"<<endl;
        exit(1);
      }
      break;
    
    case 'l':
      cmdValue = optarg;
      waitsec = atoi(cmdValue.c_str());
      if(waitsec < 5 || optarg[0]== '-')
      {
        cerr<<"Error:Invalid l value"<<endl;
        exit(1);
      }
      break;
   case 'c':
    if (optarg != nullptr) 
    {
        cmdValue = optarg;
        range = atoi(cmdValue.c_str());
        if (range < 1 || optarg[0] == '-') 
        {
            cerr<<"Error:Invalid c value"<<endl;
            exit(1);
        }
        
    } 
    break;
    default:
      break;
  }
    
  }  
  vector<vector<int>> points_all;
  int s_num,n_no,l_no;
  while(true)
  {
  s_num = get_random(2,noOfStreets);
  l_no = get_random(5, waitsec);
  points_all.reserve(noOfStreets);
  streetsData.reserve(noOfStreets);
    if(range <= 2 && s_num >=3 && noOfSegments >=3)
      {
        cerr<<"Error: Failed to generate for simultaneous 25 attempts"<<endl;
        exit(1);
      }
  //This will clear street from the street database in a1
    if(streetsData.size()!=0)
    {
      for(auto i = static_cast<std::size_t>(0); i<streetsData.size();++i)
      {
        cout<<"rm "<<streetsData[i]<<endl;
      }
    }
    // This is used to clear streets in our ece650-a3.
    streetsData.clear();

    // Random Street name generation.
    int street = 0;
    string name;
    char lower = 97;
    while(street < s_num)
    {
      vector <int> pointList;
      n_no = get_random(1,noOfSegments);
    
      for(int j = 1; j<=n_no+1 ;j++)
      {
        int count = 0;
        int x_cor,y_cor;
        x_cor = get_random(-range, range);
        y_cor = get_random(-range, range);
      
        if(validPoint(pointList,x_cor,y_cor) && !overlapping(pointList,points_all))
        {
          pointList.push_back(x_cor);
          pointList.push_back(y_cor);
          count = 0;
          
        }

        else{
          if(count == 25)
          {
            cerr<<"Error: The no. of attempts to generate valid inputs exceeded 25"<<endl;
            break;
            exit(1);
          }
          count++;
          continue;
        }
      }
      for(int n = 0; n<5;++n)
      {
        unsigned int size = 42;
        urandom.read((char*)&size, sizeof(int));
        size%=26;
        lower+=size;
        name+=lower;
        lower = 97;
      }
      if(find(streetsData.begin(),streetsData.end(),name) == streetsData.end())
      {
        name = "\"" + name + "\"";
        streetsData.push_back(name);
        street+=1;        
      }
      else{
        street-=1;
      }
      points_all.push_back(pointList);
      cout<<"add "<< name <<" ";
      for(int i = 0; i <= n_no; i++)
      {
        cout<<"("<<pointList[i]<<","<<pointList[i+1]<<") ";
      }
      cout<<endl;
      name.clear();
    }
    cout<<"gg"<<endl;
    points_all.clear();
    sleep(l_no);
  }
}


  
