#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include <cstdio>

/**
 * A program to test parsing JSON using rapidjson library.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace rapidjson;
using namespace std;

int main(int argc, char **argv) {
  const char json[] = "{\"confidence\": 55.039787, \"is_same_person\": true, \"session_id\": \"b58bbfc672abce074111166dabcd8888\"}";
  printf("Original JSON:\n %s\n", json);
  
  Document document;

  if (document.Parse(json).HasParseError()) {
    return 1;
  }

  printf("\nParsing result:\n");
  printf("confidence: [%g], is_same_person: [%s], session_id: [%s]\n",
	 document["confidence"].GetDouble(),
	 document["is_same_person"].GetBool() ? "true" : "false",
	 document["session_id"].GetString());

  return 0;
}
