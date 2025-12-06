#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

//WORDLEHELPER WORKING TOO SLOW FOR LASR
// void wordleHelper(const string& in,
//                   string& current,
//                   string floating,
//                   const set<string>& dict,
//                   set<string>& res,
//                   size_t index);

void genWords(string curr, string floating, int idx, 
              const set<string>& dict, set<string>& res);
int dashes(string in, int idx);
void expandIndex(const string& curr, const string& floating,
                  size_t idx, size_t dashNum,
                  const set<string>& dict,
                  set<string>& res);
bool inFloating(char c, const string& floating);



// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> res;
    genWords(in, floating, 0, dict, res);
    return res;
    // string current = in;
    // wordleHelper(in, current, floating, dict, res, 0);
    // return res;

}

// Define any helper functions here
void genWords(string curr, string floating, int idx, 
              const set<string>& dict, set<string>& res)
{
  //count num '-' pos starts at idx
  size_t dashNum = dashes(curr, idx);

  //pruning --> can't place all floating letters
  if(floating.length() > dashNum) return;

  //Base: no blanks left --> accept if all floats were used
  if(dashNum == 0){
    if(floating.empty() && dict.find(curr) != dict.end()) res.insert(curr);
    return;
  }

  expandIndex(curr, floating, idx, dashNum, dict, res);
}

void expandIndex(const string& curr, const string& floating,
                  size_t idx, size_t dashNum,
                  const set<string>& dict,
                  set<string>& res)
{

  //#1 --> curr pos fixed --> move forward
  if(curr[idx] != '-'){
    genWords(curr, floating, idx+1, dict, res);
    return;
  }
  
  //#2 --> try each float letter
  for(size_t i = 0; i < floating.length(); i++){
    char l = floating[i];

    string next = curr;
    next[idx] = l;

    string new_float = floating;
    new_float.erase(i, 1); //remove used floating l
    genWords(next, new_float, idx + 1, dict, res);
  }

  //#3 --> try each reg letter
  if(floating.length() < dashNum){
    for(char l = 'a'; l <= 'z'; l++){

      if(inFloating(l, floating)) continue;
      string next = curr;
      next[idx] = l;
      genWords(next, floating, idx+1, dict, res);
    }
  }

}

//cnt of how many '-' chars left from idx
int dashes(string in, int idx)
{
  int count = 0;
  for(size_t i = idx; i < in.length(); i++){
    if(in[i] == '-'){
      count++;
    }
  }
  return count;
}

bool inFloating(char c, const string& floating){
  return floating.find(c) != string::npos;
}