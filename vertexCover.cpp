#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

void incrementSetSize(int setId, unordered_map<int, int>& setSizes) {
  if (setSizes.find(setId) == setSizes.end()) setSizes[setId] = 1;
  else setSizes[setId] += 1;
}

void decrementSetSize(int setId, unordered_map<int, int>& setSizes) {
  setSizes[setId] -= 1;
  if (setSizes[setId] <= 0) setSizes.erase(setId);
}

// Read and parse edgelist file. Populate setlist and setSizes.
void readData(string fileName, vector<tuple<int, int, int> >& setlist, unordered_map<int, int>& setSizes) {
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
    setlist.push_back(make_tuple(v1, v2, lineNum));
    incrementSetSize(v1, setSizes);
    incrementSetSize(v2, setSizes);
  }
  ifs.close();
}

void printSetlist(vector<tuple<int, int, int> >& setlist) {
  cout << "setlist:" << endl;
  vector<tuple<int, int, int> >::iterator it;
  for (it=setlist.begin(); it!=setlist.end(); ++it) {
    cout << get<0>(*it) << ", " << get<1>(*it) << " --- " << get<2>(*it) << endl;
  }
}

void printSetSizes(unordered_map<int, int>& setSizes) {
  cout << "set sizes:" << endl;
  unordered_map<int, int>::iterator it;
  for (it=setSizes.begin(); it!=setSizes.end(); ++it) {
    cout << it->first << " " << it->second << endl;
  }
}

// Get id of the largest set.
int getMaxSetId(unordered_map<int, int>& setSizes) {
  unordered_map<int, int>::iterator max = setSizes.begin();
  unordered_map<int, int>::iterator it;
  for (it=setSizes.begin(); it!=setSizes.end(); ++it) {
    if (it->second > max->second) max = it;
  }
  return max->first;
}

// Remove the given set and its elements from other sets. Update set sizes.
void removeSetElements(int setId, vector<tuple<int, int, int> >& setlist, unordered_map<int, int>& setSizes) {
  // Remove tuples containing the given set.
  vector<tuple<int, int, int> >::iterator rmEnd = remove_if(setlist.begin(), setlist.end(),
  [&setId] (tuple<int, int, int>& t) {
    return get<0>(t)==setId || get<1>(t)==setId;
  });
  // Decrement sizes of affected sets.
  vector<tuple<int, int, int> >::iterator it;
  for (it=rmEnd; it!=setlist.end(); ++it) {
    int otherSetId = get<0>(*it) == setId ? get<1>(*it) : get<0>(*it);
    decrementSetSize(otherSetId, setSizes);
  }
  setSizes.erase(setId);
  // Erase removed tuples.
  setlist.erase(rmEnd, setlist.end());
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cout << "./program edgelistFile outputFile" << endl;
    return 1; 
  }

  string edgelistFileName = argv[1];
  string outFileName = argv[2];

  vector<tuple<int, int, int> > setlist;
  unordered_map<int, int> setSizes;
  vector<int> coverSets;

  readData(edgelistFileName, setlist, setSizes);
  cout << "Finished reading data" << endl;
  printSetlist(setlist);
  cout << endl;
  printSetSizes(setSizes);
  int maxSetId = getMaxSetId(setSizes);
  cout << "set count=" << setSizes.size() << endl;
  cout << "maxset id=" << maxSetId << "; maxset size=" << setSizes[maxSetId] << endl;
  cout << "Removing max set..." << endl;
  removeSetElements(maxSetId, setlist, setSizes);
  printSetlist(setlist);
  cout << endl;
  printSetSizes(setSizes);

  return 0;
}
