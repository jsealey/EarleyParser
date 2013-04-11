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
//              4. & equals epsilon
//         
///////////////////////////////////////////////////////////////////

  string finalState = "baabbb";

  // S -> aAa | bAb | epsilon
  rules['S'].push_back("aAa");
  rules['S'].push_back("bAb");
  rules['S'].push_back("&");

  // A -> SS
  rules['A'].push_back("SS");
  
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
  bool foundSolution = false;
  states.push_back(new node("S", NULL));
  while(!states.empty()){
    node* cur = states.front();
    states.pop_front();
    //cout << cur->m_state << endl;
    // No way we can match if our current state is larger than our final state
    // Update: If we're including epsilons, we might have a string larger than our final state
    //         Add 10 as an offset to catch most cases.
    if(cur->m_state.size() > finalState.size()+10) continue;
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
        break; // use the first occurence
      }
    }
    // Apply all rules and push new states onto the queue
    if(non_terminal - '0')
      for(int sz=rules[non_terminal].size(),i=0;i < sz;++i){
        if(rules[non_terminal][i][0] == '&')
          nodesToFree.push_back(new node(string(cur->m_state).erase(non_terminal_index,1),cur));
        else
          nodesToFree.push_back(new node(string(cur->m_state).replace(non_terminal_index,1,rules[non_terminal][i]),cur));
        states.push_back(nodesToFree[nodesToFree.size()-1]);
      }
  }
  for(int i=0; i < nodesToFree.size();++i) delete nodesToFree[i];
  if(foundSolution == false) cout << "no solution available" << endl;
  return 0;
}