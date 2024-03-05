#ifndef PAGE_H
#define PAGE_H

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum pageType { Normal, Win, Lose };

struct Condition {
  string varName;
  long conditionValue;
};

class Page {
 private:
  size_t pageNum;
  pageType type;
  string content;
  vector<pair<size_t, string> > choices;
  map<string, long>
      variableAssignments; // key: variable name, val: value - at this page set theese specified variables to some certain values
  map<size_t, Condition>
      choiceConditions; // key: choice index, val: condition - if the condition is not met, the choice is not available

 public:
  // constructor
  Page() : pageNum(0), type(Normal) {}

  Page(size_t num, pageType type, const string & content) :
      pageNum(num), type(type), content(content){};

  // add choices
  void addChoice(size_t destPageNum, const string & choiceText) {
    choices.push_back(make_pair(destPageNum, choiceText));
  }

  // add choices with conditions, and parse the `var=value` part of each choice
  void addChoiceWithCondition(size_t destPageNum,
                              const string & choiceText,
                              const string & condition) {
    choices.push_back(make_pair(destPageNum, choiceText));
    Condition cond = parseCondition(condition);
    choiceConditions[choices.size() - 1] = cond;  // map condition to the index of choice
  }

  // parse the `var=value` part of each choice
  Condition parseCondition(const string & condition) {
    Condition cond;
    size_t equalsPos = condition.find('=');
    if (equalsPos != string::npos) {
      cond.varName = condition.substr(0, equalsPos);
      cond.conditionValue = atol(condition.substr(equalsPos + 1).c_str());
    }
    return cond;
  }

  // display the full page for a given pageNum
  void displayPage() const {
    // print header
    cout << "Page " << pageNum << "\n==========" << endl;
    // print content
    cout << content << endl;

    // print choices and game output
    if (type == Normal) {
      cout << "What would you like to do?" << endl << endl;
      for (size_t i = 0; i < choices.size(); ++i) {
        cout << " " << (i + 1) << ". " << choices[i].second << endl;
      }
    }
    else if (type == Win) {
      cout << "Congratulations! You have won. Hooray!" << endl;
    }
    else if (type == Lose) {
      cout << "Sorry, you have lost. Better luck next time!" << endl;
    }
  }

  // display the full page for a given pageNum, without display page number
  void displayPageNoNum() const {
    // print content
    cout << content << endl;

    // print choices and game output
    if (type == Normal) {
      cout << "What would you like to do?" << endl << endl;
      for (size_t i = 0; i < choices.size(); ++i) {
        cout << " " << (i + 1) << ". " << choices[i].second << endl;
      }
    }
    else if (type == Win) {
      cout << "Congratulations! You have won. Hooray!" << endl;
    }
    else if (type == Lose) {
      cout << "Sorry, you have lost. Better luck next time!" << endl;
    }
  }

  // display the full page for a given pageNum, with condition (overloading)
  void displayPageNoNum(const map<string, long> & curStates) const {
    // print content
    cout << content << endl;

    // print choices with conditions and game output
    if (type == Normal) {
      cout << "What would you like to do?" << endl << endl;

      for (size_t i = 0; i < choices.size(); ++i) {
        if (choiceConditions.find(i) != choiceConditions.end()) {
          Condition cond = choiceConditions.at(i);
          if (curStates.find(cond.varName) != curStates.end() &&
              curStates.at(cond.varName) == cond.conditionValue) {
            cout << " " << (i + 1) << ". " << choices[i].second << endl;
          }
          else {
            cout << " " << (i + 1) << ". <UNAVAILABLE>" << endl;
          }
        }
        else {
          cout << " " << (i + 1) << ". " << choices[i].second << endl;
        }
      }
    }
    else if (type == Win) {
      cout << "Congratulations! You have won. Hooray!" << endl;
    }
    else if (type == Lose) {
      cout << "Sorry, you have lost. Better luck next time!" << endl;
    }
  }

  // getters to access private fields
  size_t getNumber() const { return pageNum; }

  pageType getType() const { return type; }

  const string & getText() const { return content; }

  const vector<pair<size_t, string> > & getChoices() const { return choices; }

  void setContent(const string & text) { content = text; }

  bool isChoiceAvailable(size_t choiceIndex, const map<string, long> & curStates) const {
    if (choiceConditions.find(choiceIndex) != choiceConditions.end()) {
      Condition cond = choiceConditions.at(choiceIndex);
      return curStates.find(cond.varName) != curStates.end() &&
             curStates.at(cond.varName) == cond.conditionValue;
    }
    return true;
  }

  void addVariableAssignment(const string & varName, long value) {
    variableAssignments[varName] = value;
  }

  const map<string, long> & getVariableAssignments() const { return variableAssignments; }
};

#endif  // PAGE_H
