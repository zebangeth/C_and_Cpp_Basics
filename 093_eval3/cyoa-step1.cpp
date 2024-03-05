#include "page.h"
#include "story.h"

using namespace std;

int main(int argc, char ** argv) {
  if (argc != 2) {
    cerr << "Invalid input argument" << endl;
    return EXIT_FAILURE;
  }

  string directory = argv[1];

  // Create a Story object
  Story story;
  story.parseStory(directory);
  story.displayStory();

  return EXIT_SUCCESS;
}
