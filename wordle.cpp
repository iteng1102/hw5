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
void helperWordle(
    const std::string&in,
    std::string word,
    size_t index,
    const std::string floating, 
    const std::set<std::string>& dict,  
    std::set<std::string>& result);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> result;
    helperWordle(in, "", 0, floating, dict, result);
    return result;

}

// Define any helper functions here
void helperWordle (
    const std::string&in,
    std::string word,
    size_t index,
    const std::string floating, 
    const std::set<std::string>& dict,  
    std::set<std::string>& result)
{

  if (index == in.length()){ //checks for base case when the index reaches end of string 
    if (dict.find(word)!=dict.end() && floating.empty()==true){ //if the word is in the dictionary and all floating letters were used 
      result.insert(word); //insert word in result 
    }
    return;
  }

  if (in[index]!='-'){
    helperWordle (in, word + in[index], index +1, floating, dict, result); //recurse if we know the letter and add it to the word 
  }
  else{
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz"; 
    size_t remain = 0; 
    for (size_t i = index; i< in.length(); i++){ //calculate remaining dashes to see if floating fits 
      if (in[i]=='-'){
        remain++;
      }
    }
    for (int i = 0; i< 26; i++){ //goes through the alphabet if its a dash 
      char letter = alphabet[i];
      size_t pos = floating.find(letter); //check if the letter is in our floating 
      if (pos!= std::string::npos){ //if its in floating, delete it from floating
        //floating.erase(pos,1); old code
        std::string newFloating = floating; //make new floating cuz its constant  
        newFloating.erase(pos,1); //erase it from new floating 
        helperWordle(in, word + letter, index + 1, newFloating, dict, result); //recurse again after adding floating to word and go to next index with new floating  
      }
      else if (remain > floating.length()){ //add letter to word if there's space if considering other floating letters 
        helperWordle(in, word + letter, index + 1, floating, dict, result); //recurse again with next index
      }
    }
  }


}
