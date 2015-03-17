#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <cstdio>
#include <fstream>      // std::ifstream

/**
 * A program to test parsing a JSON file using rapidjson library.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace rapidjson;
using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: ./parse_json_file [json_file_to_parse]\n");
    return 1;
  }

  string jsonFile = argv[1];

  // read the whole JSON file to a string
  ifstream fin(jsonFile.c_str(), ios::in);
  istreambuf_iterator<char> begin(fin), end;
  string jsonFromFile(begin, end);
  fin.close();

  printf("Original JSON:\n %s\n", jsonFromFile.c_str());
  
  Document document;

  if (document.Parse(jsonFromFile.c_str()).HasParseError()) {
    return 1;
  }

  printf("\nParsing result:\n");
  const Value& faces = document["face"];
  printf("face_id: [%s]\n", faces[0]["face_id"].GetString());

  return 0;
}
