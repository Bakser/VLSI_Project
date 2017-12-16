 #include <iostream>  
 #include <fstream>  
 #include <sstream>  
 #include <vector>  
 #include <set>  
 #include <map>  
 #include <utility>  
   
 #include <cstdlib>  
    
 using namespace std;
   
   
 #include "parser_PG.hpp"  
   
 typedef map<string,int>::value_type entry;  
map<int,int> fa,Real;
map<int,double> I,fixedV,edges[2000000],G;
int cnt_real=0,cnt_fixed=0,cnt_edges=0;
inline void addedge(int u,int v,double w){
	if(!edges[u].count(v)){
		edges[u][v]=0.0;
		cnt_edges++;
	}
	if(!G.count(u))
		G[u]=0.0;
	G[u]+=w;
	edges[u][v]+=w;
}
inline int find(int x){
	if(!fa.count(x)){
		I[x]=0.0;
		return fa[x]=x;
	}
	return fa[x]==x?x:find(fa[x]);
}
inline void merge(int x,int y){
	x=find(x);
	y=find(y);
	I[y]+=I[x];
	if(fixedV.count(x))
		fixedV[y]=fixedV[x];
	fa[x]=y;
}
 int main(int argc, char* argv[]){  
  if(argc != 4)  
  {  
   cout<<"Error: circuit file not found"<<endl;  
   exit(1);  
  }  
     
  ifstream in_file(argv[1]);  
  ofstream out_file(argv[2]);   
  ofstream out_map(argv[3]);  
    
  if(!in_file.is_open() || !out_file.is_open())  
  {  
   cout<<"Error: Can't open input file"<<endl;  
   exit(1);  
  }  
    
  stringstream SS;  
    
  parser_PG parPG;  
  vector<parser_PG> EdgeList;  
    
  string temp;  
  temp.reserve(100);  
    
  const string delim("*");  
  string::size_type begIdx, endIdx;  
    
  uint NumOfEdges=0;  
  uint NumOfNodes=0;  
  uint NumOfCurrentSors=0;  
  uint NumOfVoltageSors=0;  
  uint NumOfResistance=0;  
    
  while(!in_file.eof())  
  {  
   getline(in_file, temp);  
     
   if(!temp.compare(".op")) break;  
     
   begIdx = temp.find_first_of(delim);  
   if(begIdx != string::npos) continue;  
     
   NumOfEdges++;    
  }  
    
  in_file.close();   
  in_file.open(argv[1]);  
    
  EdgeList.reserve(NumOfEdges);  
    
  while(!in_file.eof())  
  {  
   getline(in_file, temp);  
     
   if(!temp.compare(".op")) break;  
     
   begIdx = temp.find_first_of(delim);  
   if(begIdx != string::npos) continue;  
       
   SS.clear();  
   SS.str("");  
     
   SS.str(temp);  
     
   SS >> parPG.edgeType >> parPG.sourceSymbol >> parPG.sinkSymbol >> parPG.branchValue;  
     
   EdgeList.push_back(parPG);  
     
  }  
   
  set<string> source_sink;  
  pair< set<string>::iterator, bool > check;  
    
  map<string,int> symToNum;  
  map<string,int>::iterator sourceNum;  
  map<string,int>::iterator sinkNum;  
  pair< map<string,int>::iterator, bool > status;  
     
  set<string>::iterator setIter;  
    
  status = symToNum.insert(entry("0",NumOfNodes));  
  if(status.second == true) NumOfNodes++;  
    
  for(int i=0; i<EdgeList.size(); i++)  
  {  
   status = symToNum.insert(entry(EdgeList[i].sourceSymbol,NumOfNodes));  
   if(status.second == true) NumOfNodes++;  
   status = symToNum.insert(entry(EdgeList[i].sinkSymbol,NumOfNodes));  
   if(status.second == true) NumOfNodes++;  
  }  
   
    
  string elements("riVvR");  
  string edgeType;  
  fixedV[0]=0.0; 
  for(int i=0; i<EdgeList.size(); i++)  
  {  
   sourceNum = symToNum.find(EdgeList[i].sourceSymbol);  
   sinkNum = symToNum.find(EdgeList[i].sinkSymbol);  
     
   begIdx = (EdgeList[i].edgeType).find_first_of(elements);  
   if(begIdx != string::npos)  
   {  
    endIdx = (EdgeList[i].edgeType).find_first_not_of(elements, begIdx);  
    edgeType = (EdgeList[i].edgeType).substr(begIdx, endIdx);  
      
    if(edgeType == "rr")  
    {  
      EdgeList[i].edgeType = "R";  
      NumOfResistance++;  
    }  
    else if(edgeType == "v")  
    {  
      EdgeList[i].edgeType = "V";  
      NumOfVoltageSors++;  
    }  
    else if(edgeType == "V")  
    {  
      EdgeList[i].edgeType = "R";  
      NumOfResistance++;  
    }  
    else if(edgeType == "i")  
    {  
      EdgeList[i].edgeType = "I";  
      NumOfCurrentSors++;  
    }  
    else if(edgeType == "R")  
    {  
      EdgeList[i].edgeType = "R";  
      NumOfResistance++;  
    }  
      
   }  
    parser_PG& tmp=EdgeList[i];
	int u=find((*sourceNum).second),v=find((*sinkNum).second);
	double w=tmp.branchValue;
	if(tmp.edgeType=="V"){
		if(w==0.0){
			merge(u,v);
		}
		else if(!v){
			fixedV[u]=w;
		}
		else cout<<"CNMGB"<<endl;
		tmp.del=1;
	}
	if(tmp.edgeType=="I"){//???
		I[v]+=w;
		tmp.del=1;
	}
	if(tmp.edgeType=="R"&&w==0.0){
		merge(u,v);
		tmp.del=1;
	}
   //out_file << EdgeList[i].edgeType <<i+1<< " " << (*sourceNum).second << " " << (*sinkNum).second << " " << EdgeList[i].branchValue << endl;  
  }  
  for(int i(0);i<=NumOfNodes;i++)
	  if(find(i)==i){
		Real[i]=cnt_real++;
		if(fixedV.count(i))
			cnt_fixed++;
	  }
  for(int i(0);i<EdgeList.size();i++)
	if(!EdgeList[i].del){
		sourceNum = symToNum.find(EdgeList[i].sourceSymbol);
		sinkNum = symToNum.find(EdgeList[i].sinkSymbol);
		parser_PG& tmp=EdgeList[i];
		int u=Real[find((*sourceNum).second)],v=Real[find((*sinkNum).second)];
		double w=tmp.branchValue;
		if(tmp.edgeType=="R"){
			addedge(u,v,1.0/w);
			addedge(v,u,1.0/w);
		}
		else cout<<"FUCK";
	}
  out_file<<cnt_real<<endl;
  for(int i(0);i<=NumOfNodes;i++)
	  if(find(i)==i){
		  out_file<<I[i]<<" "<<G[Real[i]]<<endl;
	  }
  out_file<<cnt_fixed<<endl;
  for(int i(0);i<=NumOfNodes;i++)
	  if(find(i)==i&&fixedV.count(i)){
		out_file<<Real[i]<<" "<<fixedV[i]<<endl;
	  }
  out_file<<cnt_edges<<endl;
	for(int i(0);i<cnt_real;i++)
		for(map<int,double>::iterator it=edges[i].begin();it!=edges[i].end();it++)
			out_file<<i<<" "<<it->first<<" "<<it->second<<endl;
  //out_file << ".end";  
	for(map<string,int>::iterator it=symToNum.begin();it!=symToNum.end();it++)
		out_map<<it->first<<" "<<Real[find(it->second)]<<endl;
  cout << "Total Number of Edges = " << NumOfEdges << endl;  
  cout << "Total Number of Nodes = " << NumOfNodes << endl;  
  cout << "Total Number of Current sources = " << NumOfCurrentSors << endl;  
  cout << "Total Number of Voltage sources(including shorts) = " << NumOfVoltageSors << endl;  
  cout << "Total Number of Resistance(including shorts) = " << NumOfResistance << endl;  
    
  return 0;  
 }  
