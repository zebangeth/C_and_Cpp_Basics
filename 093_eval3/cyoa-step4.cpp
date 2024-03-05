#include "page.h"
#include "story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    cerr << "Invalid input argument" << endl;
    return EXIT_FAILURE;
  }

  string directory = argv[1];
  Story story;
  story.parseStoryWithCondition(directory);
  story.checkStory();
  story.beginStoryWithCondition();

  return EXIT_SUCCESS;
}
