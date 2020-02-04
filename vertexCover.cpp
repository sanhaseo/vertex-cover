#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
using namespace std;

void incrementSetSize(int setId, unordered_map<int, int>& setSizes) {
  if (setSizes.find(setId) == setSizes.end()) setSizes[setId] = 1;
  else setSizes[setId] += 1;
}

void readData(string fileName, vector<pair<int, int> >& setlist, unordered_map<int, int>& setSizes) {
  double simThr = 0.6;
  string line;
  ifstream ifs(fileName.c_str());

  int lineNum = 0;
  while (getline(ifs, line)) {
    istringstream iss(line);
    int v1, v2;
    double sim;
    iss >> v1 >> v2 >> sim;
    if (sim < simThr) continue;
    
    lineNum++;
    if (lineNum % 10000 == 0) cout << "Reading line " << lineNum << endl;
    setlist.push_back(make_pair(v1, lineNum));
    setlist.push_back(make_pair(v2, lineNum));
    
    incrementSetSize(v1, setSizes);
    incrementSetSize(v2, setSizes);
  }
  ifs.close();
}

void printSetlist(vector<pair<int, int> >& setlist) {
  cout << "setlist:" << endl;
  vector<pair<int, int> >::iterator it;
  for (it=setlist.begin(); it!=setlist.end(); ++it) {
    cout << it->first << " " << it->second << endl;
  }
}

void printSetSizes(unordered_map<int, int>& setSizes) {
  cout << "set sizes:" << endl;
  unordered_map<int, int>::iterator it;
  for (it=setSizes.begin(); it!=setSizes.end(); ++it) {
    cout << it->first << " " << it->second << endl;
  }
}

int getMaxSetId(unordered_map<int, int>& setSizes) {
  unordered_map<int, int>::iterator max = setSizes.begin();
  unordered_map<int, int>::iterator it;
  for (it=setSizes.begin(); it!=setSizes.end(); ++it) {
    if (it->second > max->second) max = it;
  }
  return max->first;
}

void removeSetElements(int setId, vector<pair<int, int> >& setlist, unordered_map<int, int>& setSizes) {
  vector<pair<int, int> >::iterator it;
  for (it=setlist.begin(); it!=setlist.end(); ++it) {
    if (it->first == setId) {
      int element = it->second;
    }
  }
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cout << "./program edgelistFile outputFile" << endl;
    return 1; 
  }

  string edgelistFileName = argv[1];
  string outFileName = argv[2];

  vector<pair<int, int> > setlist;
  unordered_map<int, int> setSizes;
  vector<int> coverSets;

  readData(edgelistFileName, setlist, setSizes);
  cout << "Finished reading data" << endl;
  //printSetlist(setlist);
  //cout << endl;
  printSetSizes(setSizes);
  int maxSetId = getMaxSetId(setSizes);
  cout << "set count=" << setSizes.size() << endl;
  cout << "maxset id=" << maxSetId << "; maxset size=" << setSizes[maxSetId] << endl;

  return 0;
}
