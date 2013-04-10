/*
  Program: Earley Parser Adaption (Top Down Parser)
  Author:  Jared Sealey
  Description: BFS derivation tree to check if 
               a user defined finalState exists 
               in a user defined Grammar
*/
#include<fstream>
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<list>
#include<map>
using namespace std;
typedef struct node {
  node(string state, node *parent)
    :m_state(state),m_parent(parent){}
	bool m_visited;
	string m_state;
	node* m_parent;
};
list<node *> states;
int main()
{
  vector< vector<string> > rules = vector< vector<string> >('Z',vector<string>());
  vector<node*> nodesToFree;
///////////////////////////////////////////////////////////////////
// Define your final state and your grammar rules
// Constraints: 1. non-terminals must be A-Z
//              2. terminals must be a-z
//              3. Start state must be S
//         
///////////////////////////////////////////////////////////////////
  string finalState = "aaab";

  // S -> AA | A | B
  rules['S'].push_back("AA");
  rules['S'].push_back("A");
  rules['S'].push_back("B");

  // A -> AaA | a | b
  rules['A'].push_back("AaA");
  rules['A'].push_back("a");
  rules['A'].push_back("b");

  // B -> b | a | A
  rules['B'].push_back("b");
  rules['B'].push_back("a");
  rules['B'].push_back("A");

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
  bool foundSolution = false;
  states.push_back(new node("S", NULL));
  while(!states.empty()){
  	node* cur = states.front();
  	states.pop_front();
    // No way we can match if our current state is larger than our final state
    if(cur->m_state.size() > finalState.size()) continue;
    // Check if our state is the final state
    if(finalState.compare(cur->m_state) == 0){
      string output = finalState;
      cur=cur->m_parent;
      for(;cur;cur=cur->m_parent)
        output = cur->m_state + " => " + output;
      cout << output << endl;
      foundSolution = true;
      continue;
    }
    // Find the next non-terminal
    char non_terminal = '0';
    int non_terminal_index = 0;
    for(int sz=cur->m_state.size(), i=0; i < sz;++i){
      if(cur->m_state[i] >= 'A' && cur->m_state[i] <= 'Z'){
        non_terminal = cur->m_state[i];
        non_terminal_index = i;
      }
    }
    // Apply all rules and push new states onto the queue
    if(non_terminal - '0')
      for(int sz=rules[non_terminal].size(),i=0;i < sz;++i){
        nodesToFree.push_back(new node(string(cur->m_state).replace(non_terminal_index,1,rules[non_terminal][i]),cur));
        states.push_back(nodesToFree[nodesToFree.size()-1]);
      }
  }
  for(int i=0; i < nodesToFree.size();++i) delete nodesToFree[i];
  if(foundSolution == false) cout << "no solution available" << endl;
  return 0;
}