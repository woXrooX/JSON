#include <iostream>

#include "json.hpp"

int main(){
  std::string data = R"(
{
  "int": 5,
  "str": "Some Text",
  "arr": [1, 2, 3, "a", "b"],
  "obj": {
    "child": false
  }
}
  )";

  // std::string a = "aaa";
  // woXrooX::Json b;
  // woXrooX::Json j;
  // j = b;
  // j.setRealNumber(5);
  // std::cout << j.getRealNumber() << '\n';
  // std::cout << "\nSize: " << sizeof(j) << '\n';


  woXrooX::Tokenizer t("./source/test.json");

  return 0;

}
