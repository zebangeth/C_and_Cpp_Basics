#ifndef STORY_H
#define STORY_H

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "page.h"

using namespace std;

class Story {
 private:
  map<size_t, Page> pages;      //key: pageNum, val: Page instance
  map<string, long> curStates;  // current states; key: variable name, val: numbers

 public:
  // constructor
  Story(){};

  // copy constructor
  Story(const Story & rhs) { pages = rhs.pages; }

  // assignment operator
  Story & operator=(const Story & rhs) {
    if (this != &rhs) {
      pages.clear();
      pages = rhs.pages;
    }
    return *this;
  }

  //destructor
  ~Story() {}

  // add new page to pages map
  void addPage(const Page & page) { pages[page.getNumber()] = page; }

  // parse story.txt file and corresponding pages
  // 1. check if a line is valid or not (should be an empty line, or number@type:filename, or pagenum:destpage:text; if not valid, exit failure)
  // 2. parse each line
  // 2a. if it's a page declaration line (number@type:filename), create a new Page instance and add it to pages
  // 2b. if it's a choice line (pagenum:destpage:text), add choices for the pagenum
  // need to check for error cases:
  // i. only normal type page can have choices
  // ii. page numbers need to be sequential
  void parseStory(const string & directory) {
    ifstream storyFile((directory + "/story.txt").c_str());
    if (!storyFile.is_open()) {
      cerr << "Failed to open story file in directory: " << directory << endl;
      exit(EXIT_FAILURE);
    }

    string line;
    size_t lastPageNum = static_cast<size_t>(-1);  // to track page order
    while (getline(storyFile, line)) {
      // skip empty lines
      if (line.find_first_not_of(" \n\t\r\f\v") == string::npos)
        continue;

      // check for page declaration line
      size_t atPos = line.find('@');
      if (atPos != string::npos) {
        size_t pageNum = atoi(line.substr(0, atPos).c_str());
        if (pageNum != lastPageNum + 1) {
          cerr << "Pages are not sequential" << endl;
          exit(EXIT_FAILURE);
        }
        lastPageNum = pageNum;

        char typeChar = line[atPos + 1];
        if (typeChar != 'N' && typeChar != 'W' && typeChar != 'L') {
          cerr << "Invalid page type: " << typeChar << endl;
          exit(EXIT_FAILURE);
        }
        pageType type = (typeChar == 'N' ? Normal : (typeChar == 'W' ? Win : Lose));

        string filename = line.substr(line.find(':') + 1);
        ifstream pageFile((directory + "/" + filename).c_str());
        if (!pageFile.is_open()) {
          cerr << "Failed to open page file: " << filename << endl;
          exit(EXIT_FAILURE);
        }

        string content((istreambuf_iterator<char>(pageFile)),
                       istreambuf_iterator<char>());
        Page newPage(pageNum, type, content);
        addPage(newPage);
      }
      // check for choice declaration line
      else {
        size_t colonPos1 = line.find(':');
        size_t colonPos2 = line.rfind(':');
        if (colonPos1 == string::npos || colonPos2 == string::npos ||
            colonPos1 == colonPos2) {
          cerr << "Invalid choice format" << endl;
          exit(EXIT_FAILURE);
        }

        size_t pageNum = atoi(line.substr(0, colonPos1).c_str());
        size_t destPageNum =
            atoi(line.substr(colonPos1 + 1, colonPos2 - colonPos1 - 1).c_str());
        string choiceText = line.substr(colonPos2 + 1);

        Page * page = getPage(pageNum);
        if (page == NULL || page->getType() != Normal) {
          cerr << "Invalid page number or type for choice" << endl;
          exit(EXIT_FAILURE);
        }
        page->addChoice(destPageNum, choiceText);
      }
    }
  }

  // (STEP4) parseStory method for story with conditions
  void parseStoryWithCondition(const string & directory) {
    ifstream storyFile((directory + "/story.txt").c_str());
    if (!storyFile.is_open()) {
      cerr << "Failed to open story file in directory: " << directory << endl;
      exit(EXIT_FAILURE);
    }

    string line;
    size_t lastPageNum = static_cast<size_t>(-1);  // to track page order
    while (getline(storyFile, line)) {
      // skip empty lines
      if (line.find_first_not_of(" \n\t\r\f\v") == string::npos)
        continue;

      size_t atPos = line.find('@');
      if (atPos != string::npos) {
        // page declaration line - "pageNum@type:filename.txt"
        size_t pageNum = atoi(line.substr(0, atPos).c_str());
        if (pageNum != lastPageNum + 1) {
          cerr << "Pages are not sequential" << endl;
          exit(EXIT_FAILURE);
        }
        lastPageNum = pageNum;

        char typeChar = line[atPos + 1];
        if (typeChar != 'N' && typeChar != 'W' && typeChar != 'L') {
          cerr << "Invalid page type: " << typeChar << endl;
          exit(EXIT_FAILURE);
        }
        pageType type = (typeChar == 'N' ? Normal : (typeChar == 'W' ? Win : Lose));

        string filename = line.substr(line.find(':') + 1);
        ifstream pageFile((directory + "/" + filename).c_str());
        if (!pageFile.is_open()) {
          cerr << "Failed to open page file: " << filename << endl;
          exit(EXIT_FAILURE);
        }

        string content((istreambuf_iterator<char>(pageFile)),
                       istreambuf_iterator<char>());
        Page newPage(pageNum, type, content);
        addPage(newPage);
      }
      else {
        size_t colonPos1, colonPos2, dollarPos, bracketPos1, bracketPos2;
        string varName, varValueStr, condition;
        long varValue;
        size_t pageNum;

        dollarPos = line.find('$');
        bracketPos1 = line.find('[');
        if (dollarPos != string::npos) {
          // variable assignment - "pageNum$var=value"
          pageNum = atoi(line.substr(0, dollarPos).c_str());
          varName = line.substr(dollarPos + 1, line.find('=') - dollarPos - 1);
          varValueStr = line.substr(line.find('=') + 1);
          varValue = atol(varValueStr.c_str());

          Page * page = getPage(pageNum);
          if (page) {
            page->addVariableAssignment(varName, varValue);
          }
          // initialize the variable in curStates
          curStates[varName] = 0;
        }
        else if (bracketPos1 != string::npos) {
          // choice declaration with conditions - "pageNum[var=value]:destPageNum:choiceText"
          pageNum = atoi(line.substr(0, bracketPos1).c_str());
          bracketPos2 = line.find(']');
          condition =
              line.substr(bracketPos1 + 1, bracketPos2 - bracketPos1 - 1);  // var=value
          colonPos1 = line.find(':', bracketPos2);
          colonPos2 = line.rfind(':');
          size_t destPageNum =
              atoi(line.substr(colonPos1 + 1, colonPos2 - colonPos1 - 1).c_str());
          string choiceText = line.substr(colonPos2 + 1);
          Page * page = getPage(pageNum);
          page->addChoiceWithCondition(destPageNum, choiceText, condition);
        }
        else {
          // choice declaration without conditions - "pageNum:destPageNum:choiceText"
          colonPos1 = line.find(':');
          colonPos2 = line.rfind(':');
          if (colonPos1 == string::npos || colonPos2 == string::npos ||
              colonPos1 == colonPos2) {
            cerr << "Invalid choice format" << endl;
            exit(EXIT_FAILURE);
          }

          pageNum = atoi(line.substr(0, colonPos1).c_str());
          size_t destPageNum =
              atoi(line.substr(colonPos1 + 1, colonPos2 - colonPos1 - 1).c_str());
          string choiceText = line.substr(colonPos2 + 1);

          Page * page = getPage(pageNum);
          if (page == NULL || page->getType() != Normal) {
            cerr << "Invalid page number or type for choice" << endl;
            exit(EXIT_FAILURE);
          }
          page->addChoice(destPageNum, choiceText);
        }
      }
    }
  }

  Page * getPage(size_t pageNum) {
    map<size_t, Page>::iterator it = pages.find(pageNum);
    if (it != pages.end()) {
      return &(it->second);
    }
    return NULL;
  }

  //display all the pages in the story
  void displayStory() const {
    for (map<size_t, Page>::const_iterator it = pages.begin(); it != pages.end(); ++it) {
      it->second.displayPage();
    }
  }

  // check the story.txt file for:
  // 1. choice can redirect to a valid page (3a)
  // 2. every page is referenced by at least one other page (3b)
  // 3. WIN and LOSE page exist (3c)
  void checkStory() const {
    set<size_t> choiceMentionedPages;
    bool winExist = false;
    bool loseExist = false;

    // loop each page in pages, ensure it's in choiceMentionedPages
    for (map<size_t, Page>::const_iterator it = pages.begin(); it != pages.end(); ++it) {
      // add choice mentioned pages to the set
      const vector<pair<size_t, string> > & choices = it->second.getChoices();
      for (size_t i = 0; i < choices.size(); i++) {
        choiceMentionedPages.insert(choices[i].first);
      }
      if (it->second.getType() == Win)
        winExist = true;
      if (it->second.getType() == Lose)
        loseExist = true;
    }
    // check if every choice can redirect to a valid page (3a)
    for (set<size_t>::const_iterator it = choiceMentionedPages.begin();
         it != choiceMentionedPages.end();
         ++it) {
      if (pages.find(*it) == pages.end()) {
        cerr << "Choice references a non-existent page." << endl;
        exit(EXIT_FAILURE);
      }
    }
    // loop each page in pages, ensure it's in choiceMentionedPages (3b)
    for (map<size_t, Page>::const_iterator it = pages.begin(); it != pages.end(); ++it) {
      if (it->first != 0 &&
          choiceMentionedPages.find(it->first) == choiceMentionedPages.end()) {
        cerr << "Some page(s) are not referenced." << endl;
        exit(EXIT_FAILURE);
      }
    }
    // check if both WIN and LOSE page exist (3c)
    if (!winExist || !loseExist) {
      cerr << "No WIN or LOSE page." << endl;
      exit(EXIT_FAILURE);
    }
  }

  // begin story on page 0 and display the story with an interactive mode
  void beginStory() {
    size_t curPageNum = 0;
    while (true) {
      Page * curPage = getPage(curPageNum);
      if (!curPage) {
        cerr << "Invalid page number" << endl;
        exit(EXIT_FAILURE);
      }
      curPage->displayPageNoNum();

      if (curPage->getType() == Win || curPage->getType() == Lose) {
        break;
      }

      size_t choice;
      while (true) {
        cin >> choice;
        if (cin.eof()) {
          cerr << "Input ended on a normal page. Exiting with failure." << endl;
          exit(EXIT_FAILURE);
        }
        if (cin.fail() || choice <= 0 || choice > curPage->getChoices().size()) {
          cout << "That is not a valid choice, please try again" << endl;
        }
        else {
          curPageNum = curPage->getChoices()[choice - 1].first;
          break;
        }
      }
    }
  }

  void setVariable(const string & varName, long value) { curStates[varName] = value; }

  // (STEP4) begin story on page 0 and display the story with an interactive mode
  void beginStoryWithCondition() {
    size_t curPageNum = 0;
    while (true) {
      Page * curPage = getPage(curPageNum);
      if (!curPage) {
        cerr << "Invalid page number" << endl;
        exit(EXIT_FAILURE);
      }

      // initialize curStates
      const map<string, long> & assignments = curPage->getVariableAssignments();
      for (map<string, long>::const_iterator it = assignments.begin();
           it != assignments.end();
           ++it) {
        setVariable(it->first, it->second);
      }

      // pass current state to display page with conditions
      curPage->displayPageNoNum(curStates);

      if (curPage->getType() == Win || curPage->getType() == Lose) {
        break;
      }

      size_t choice;
      while (true) {
        cin >> choice;
        if (cin.eof()) {
          cerr << "Input ended on a normal page. Exiting with failure." << endl;
          exit(EXIT_FAILURE);
        }
        if (cin.fail() || choice <= 0 || choice > curPage->getChoices().size()) {
          cout << "That is not a valid choice, please try again" << endl;
        }
        else {
          // printCurStates();
          // check if the choice is available based on the current state
          if (curPage->isChoiceAvailable(choice - 1, curStates)) {
            curPageNum = curPage->getChoices()[choice - 1].first;
            break;
          }
          else {
            cout << "That choice is not available at this time, please try again" << endl;
          }
        }
      }
    }
  }

  // find all the winning paths, keep a visited set to avoid cycle
  void findWinPaths() {
    vector<string> path;
    set<size_t> visited;
    bool hasWinPath = false;

    findWinPathsDFS(0, path, visited, hasWinPath);

    if (!hasWinPath) {
      cout << "This story is unwinnable!" << endl;
    }
  }

  // DFS approach to find all the winning paths
  void findWinPathsDFS(size_t pageNum,
                       vector<string> & path,
                       set<size_t> & visited,
                       bool & hasWinPath) {
    visited.insert(pageNum);
    Page * currentPage = getPage(pageNum);

    // if the page is invalid
    if (!currentPage) {
      visited.erase(pageNum);
      return;
    }

    // if the page is a win
    if (currentPage->getType() == Win) {
      ostringstream winPathComponent;
      winPathComponent << pageNum;
      path.push_back(winPathComponent.str());
      printPath(path);
      path.pop_back();
      hasWinPath = true;
      visited.erase(pageNum);
      return;
    }

    const vector<pair<size_t, string> > & choices = currentPage->getChoices();
    for (size_t i = 0; i < choices.size(); ++i) {
      if (visited.find(choices[i].first) == visited.end()) {
        ostringstream pathComponent;
        pathComponent << pageNum << "(" << (i + 1) << "),";
        path.push_back(pathComponent.str());
        findWinPathsDFS(choices[i].first, path, visited, hasWinPath);
        path.pop_back();
      }
    }

    visited.erase(pageNum);
  }

  // print a (win) path
  void printPath(const vector<string> & path) {
    for (size_t i = 0; i < path.size(); ++i) {
      cout << path[i];
    }
    cout << "(win)" << endl;
  }

  // print curStates for debug
  void printCurStates() const {
    cout << "{";
    for (map<string, long>::const_iterator it = curStates.begin(); it != curStates.end();
         ++it) {
      cout << it->first << ": " << it->second << "; ";
    }
    cout << "}\n";
  }
};

#endif  // STORY_H
