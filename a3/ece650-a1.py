#!/usr/bin/python3
import sys
import re   
def pp(x):
    if isinstance(x,float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)

class generateVertex(object):
    def __init__(self,x,y):
          self.x = float(x)
          self.y = float(y)
                  
    def __repr__(self):
         return '(' + pp(self.x) + ', ' + pp(self.y) + ')'
     

class generateLine(object):
    def __init__(self, src, dest):
        self.src = src
        self.dest = dest

    def __repr__(self):
        return str(self.src) + '-->' + str(self.dest)
        
class generate_Vertices_Edges:

    @staticmethod
    def generate_streets(streets_data,street_keys):
    
        name_of_streets = street_keys

        intersecting_point_dict={}
        intersecting_point_list = []
        idVertex = 1
        listVertex = set()
        vertices_dict = {}
        vertices_edge = {}


        for m in range(len(name_of_streets) - 1):   
            for n in range(m+1,len(name_of_streets)):                                              
                for line_1 in streets_data[name_of_streets[m]]:
                    for line_2 in streets_data[name_of_streets[n]]:
                        intersecting = is_intersecting(line_1, line_2)
                        
                        if(intersecting != False and intersecting is not None):
                            
                            intersecting_point = generateVertex(intersecting[0],intersecting[1])
                            intersecting_point_dict[intersecting_point] = []
                            intersecting_point_dict[intersecting_point].append(line_2)
                            intersecting_point_dict[intersecting_point].append(line_1)

                            intersecting_point_list.append(intersecting_point)
                                                                                                        
                            for point in [line_1.src,line_1.dest,line_2.src,line_2.dest]:  
                                if str(point) not in listVertex:
                                    vertices_dict[idVertex] = point
                                    idVertex +=1
                                    listVertex.add(str(point))
                                    
                                else:
                                    pass
                                        

                            if str(intersecting_point) not in listVertex:
                                vertices_dict[idVertex] = intersecting_point
                                idVertex+=1
                                listVertex.add(str(intersecting_point))
                            else:
                                pass
    
                                
                            
        for key,value in vertices_dict.items():
            vertices_edge[str(value)] = key
        
        listOfEdges = generate_Vertices_Edges.generateEdges(intersecting_point_dict,intersecting_point_list,name_of_streets,streets_data)

        generate_Vertices_Edges.print_Graph(vertices_dict,vertices_edge,listOfEdges)
        

    @staticmethod
    def generateEdges(intersecting_point_dict,intersecting_point_list,name_of_streets,data):
        for m in range(len(intersecting_point_list)-1):
            for n in range(m+1,len(intersecting_point_list)):
                if intersecting_point_list[n].x < intersecting_point_list[m].x:
                    intersecting_point_list[m],intersecting_point_list[n] = intersecting_point_list[n],intersecting_point_list[m]

                elif intersecting_point_list[n].x == intersecting_point_list[m].x:
                    if intersecting_point_list[n].y < intersecting_point_list[m].y:
                        intersecting_point_list[m],intersecting_point_list[n] = intersecting_point_list[n],intersecting_point_list[m]

        listOfEdges = set()
        length_s = len(name_of_streets)                    
        for name in range(length_s):                        
            for line_segment in data[name_of_streets[name]]:
                if line_segment.src.x > line_segment.dest.x:   
                    line_segment.src,line_segment.dest = line_segment.dest,line_segment.src
                elif line_segment.src.x == line_segment.dest.x:
                    if line_segment.src.y > line_segment.dest.y:
                        line_segment.src,line_segment.dest = line_segment.dest,line_segment.src
                        
                else:
                    pass                                  
                list_new = []
                for intersection in intersecting_point_list:             
                    for intersect in intersecting_point_dict[intersection]:  
                            if intersect == line_segment:
                                if str(line_segment.src) not in list_new:
                                    list_new.append(str(line_segment.src))

                                if str(intersection) not in list_new:
                                    list_new.append(str(intersection)) 

                if str(line_segment.dest) not in list_new:
                    list_new.append(str(line_segment.dest))        

                for m in range(len(list_new) - 1):                          
                    edge = generateLine(list_new[m],list_new[m+1])
                    listOfEdges.add((edge.src,edge.dest))
        return [generateLine(src,dest) for src,dest in listOfEdges] 
  
    @staticmethod
    def print_Graph(vertices_dict,vertices_edge,listOfEdges):     
   
            sys.stdout.write('V {}\n'.format(len(vertices_dict)))
            sys.stdout.flush()

            sys.stdout.write("E {")    
            for count , edge in enumerate(listOfEdges):
                if count == len(listOfEdges) - 1 :
                    sys.stdout.write("<" + str(vertices_edge[edge.src]) + "," + str(vertices_edge[edge.dest]) + ">")

                else:
                    sys.stdout.write("<" + str(vertices_edge[edge.src]) + "," + str(vertices_edge[edge.dest]) + ">,")
            sys.stdout.write('}\n')
            sys.stdout.flush()
            vertices_edge.clear()
            vertices_dict.clear()
            

def generate_lines(points):
    pointList = []
    lineList = []
    try:  
        try:
          for point in points:
            if(len(point)>=2):  
              vertex = generateVertex(point[0],point[1])
              if vertex not in pointList:
                    pointList.append(vertex)
            else:
                print("Please enter 2 co-ordinates:",point)

          
        except:
          IndexError

        
        length_point_List = len(pointList)
        for i in range(length_point_List- 1):
            lineList.append(generateLine(pointList[i],pointList[i+1]))
           
        return lineList
    except: 
        TypeError 

def is_not_parellel(a , co_ordinate, b):
    return min(a,b) <= co_ordinate <= max(a,b)

def is_intersecting(l1,l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dest.x, l1.dest.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dest.x,l2.dest.y
     
    xnum = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
    xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    yden = xden

    if(xden == 0):
        return False
    else:
        xcoordinate = xnum / xden;
        ycoordinate = ynum / yden;
        if is_not_parellel(x1, xcoordinate,x2) and is_not_parellel(y1, ycoordinate,y2) and is_not_parellel(x3, xcoordinate,x4) and is_not_parellel(y3, ycoordinate,y4):                  
            return [xcoordinate,ycoordinate]
        
def main():
    streets_data = {}
   
    while True:
        try:
            input_string = sys.stdin.readline()
            if input_string == "":
                break
            
            pattern = r'^\s*(add|mod|rm|gg)((?: +?"[a-zA-Z ]+?" *?))?((?:\([-]?[0-9]+?,[-]?[0-9]+?\) *?)*)?$'
            if ')(' in input_string:
                print('Error: There should be a space between two co-ordinates')
                continue
            elif '"(' in input_string:
                print('Error:There should be space between name of the street and the co-ordinates')
                continue

            matched_string = re.match(pattern,input_string)
            if matched_string is not None:
                input_operation = matched_string.group(1).strip()
                input_street_name = matched_string.group(2)
                input_co_ordinates = matched_string.group(3).strip()           

                final_co_ordinates = re.findall(r'\((\-?\d+),(\-?\d+)\)',input_co_ordinates)
                
            else:
                print('Error: Please enter the input in a correct format')
                continue
            try:  
                if input_operation == 'add':
                        input_street_name = input_street_name.replace('"','').strip()
                        
                        if len(input_street_name) < 1:
                            print("Error! Please enter a valid street name and it can't be empty")
                            continue
                        input_street_name = input_street_name.lower()    
                    
                        if input_street_name in streets_data:
                            print("Error: Street name " +input_street_name+ " already exists!")
                            continue

                        if len(final_co_ordinates) < 2:
                            print("Error! Please enter minimum two co-ordinates")
                            continue
                        
                        value_co_ordinates = generate_lines(final_co_ordinates)
                        streets_data[input_street_name] = value_co_ordinates
                                        
                if input_operation == 'mod':
                        input_street_name = input_street_name.replace('"','').strip()
                        
                        if len(input_street_name) < 1:
                            print("Error! Please enter a valid street name and it can't be empty")
                            continue
                        input_street_name = input_street_name.lower()    
                    
                        if input_street_name not in streets_data:
                            print("Error: You are trying to modify a street that does not exist, Try again")
                            continue

                        if len(final_co_ordinates) < 2:
                            print("Error! Please enter minimum two co-ordinates")
                            continue

                        
                        value_co_ordinates = generate_lines(final_co_ordinates)
                        streets_data[input_street_name] = value_co_ordinates
                        
                if input_operation == 'rm':
                        input_street_name = input_street_name.replace('"','').strip()
    
                        if len(input_street_name) < 1:
                            print("Error! Please enter a valid street name and it can't be empty")
                            continue
                        input_street_name = input_street_name.lower()    
                    
                        if input_street_name not in streets_data:
                            print("Error: The street you are trying to remove does not exist, Try again!")
                            continue
                    
                        del streets_data[input_street_name]
                        
                if input_operation == 'gg': 
                    if streets_data != {}:             
                        street_keys = list(streets_data.keys())
                        generate_Vertices_Edges.generate_streets(streets_data,street_keys)
                    else:
                        sys.stdout.write('V 0\n')
                        sys.stdout.flush()
                        sys.stdout.write('E {}\n')
                        sys.stdout.flush()
            except Exception as e:
                print('Error: '+str(e))

        except EOFError:
            sys.exit(0)
    # return exit code 0 on successful termination
    
if __name__ == "__main__":
    main()

