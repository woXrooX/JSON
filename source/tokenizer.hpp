#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <fstream>
#include <vector>

namespace woXrooX{
  class Tokenizer{
  public:
    Tokenizer(const std::string& fileName){
      this->open(fileName);
    }

    // Close this->file If Still Open
    ~Tokenizer(){
      if(this->file.is_open()) this->file.close();
    }

    void open(const std::string& file){
      this->file.open(file, std::ios::in);

      // Check If Opened Successfully
      if(!this->file.is_open()){
        std::cout << "Could Not Open The File!" << '\n';
        return;
      }

      // Reading Into this->data
      std::string line;
      while(getline(this->file, line)) this->data += line + '\n';

      // Close this->file
      this->file.close();

      // Loaded data
      std::cout << this->data;

      // Remove White Spaces
      // this->removeSpaces();

      // Init lexer
      Lexer::init(this->data);

    }

  private:
    /////////// Methods
    void removeSpaces(){
      bool isString = false;
      std::string buffer = "";

      for(size_t i = 0; i < this->data.length(); i++){
        switch(this->data[i]){
          // isString Toggler
          case '"':
            if((i-1) >= 0 && this->data[i-1] != '\\') isString = !isString;

          // White Spaces
          case ' ':
          case '\t':
          case '\r':
          case '\n':
            // if(isString == true){
              // buffer += this->data[i];
            // }
            break;

          // Default
          default:
            buffer += this->data[i];
            break;
        }

      }

      this->data = buffer;

    }

    void handleObject(){
    }

    void lexer(){
      for(; this->currentIndex < this->data.length(); this->currentIndex++){
        switch(this->data[this->currentIndex]){
          //// curlyBrackets
          // curlyBracketOpen
          case '{':
            if(this->type == Types::string){
              value += this->data[this->currentIndex];
              break;

            }

            // Object Start
            else if(this->type == Types::unset){
              this->delimiter = Delimiters::curlyBracketOpen;
              this->type = Types::object;

              this->handleObject();

              break;

            }

          // curlyBracketClose
          case '}':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;

            }

            // Object End
            else{
              this->delimiter = Delimiters::curlyBracketClose;

              this->type = Types::unset;

              this->token.value = this->value;
              this->token.type = Types::object;
              this->tokens.push_back(this->token);

              this->value = "";

              break;

            }

          //// squareBrackets
          // squareBracketOpen
          case '[':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;

            }

            // Array Start
            else if(this->type == Types::unset){
              this->delimiter = Delimiters::squareBracketOpen;
              this->type = Types::array;
              break;
            }

          // squareBracketClose
          case ']':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;
            }

            // Array End
            else{
              this->delimiter = Delimiters::squareBracketClose;

              this->type = Types::unset;

              this->token.value = this->value;
              this->token.type = Types::array;
              this->tokens.push_back(this->token);

              this->value = "";

              break;
            }

          //// doubleQuotes
          case '"':
            // String Start
            if(this->type == Types::unset){
              this->delimiter = Delimiters::doubleQuotesOpen;
              this->type = Types::string;
              break;
            }

            // String End
            else if(
              this->type == Types::string &&
              (this->currentIndex - 1) >= 0 &&
              this->data[this->currentIndex - 1] != '\\'
            ){
              this->delimiter = Delimiters::doubleQuotesClose;

              this->type = Types::unset;

              this->token.value = this->value;
              this->token.type = Types::string;
              this->tokens.push_back(this->token);

              this->value = "";

              break;
            }

          //// Singular(s)
          // whiteSpace
          case ' ':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;
            }
            break;

          // colon
          case ':':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;
            }
            break;

          // comma
          case ',':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;
            }
            break;

          //// Real Numbers
          // Negative
          case '-':
          // Decimal Point
          case '.':
          // E & e Needs to be implemented
          // 0 - 9
          case '0' ... '9':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;
            }

            else if(this->type == Types::unset){
              this->type = Types::realNumber;
              this->value += this->data[this->currentIndex];

              // this->token.value = this->value;
              // this->token.type = Types::realNumber;
              // this->tokens.push_back(this->token);

              // this->value = "";

              break;
            }

            else if(this->type == Types::realNumber){
              this->value += this->data[this->currentIndex];
              break;
            }

          //// a-zA-Z
          // case 'a' ... 'z':
          // case 'A' ... 'Z':
          //   if(this->delimiter == Delimiters::doubleQuotesOpen){
          //     this->value += this->data[this->currentIndex];
          //     break;
          //
          //   }

          //// Boolean
          // True
          case 't':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;
            }

            else if(
              (this->currentIndex + 3) <= this->data.length() &&
              this->data[this->currentIndex + 1] == 'r' &&
              this->data[this->currentIndex + 2] == 'u' &&
              this->data[this->currentIndex + 3] == 'e'
            ){
              this->currentIndex += 3;

              this->token.value = "true";
              this->token.type = Types::boolean;
              this->tokens.push_back(this->token);

              this->value = "";

              break;
            }

          // False
          case 'f':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;
            }

            else if(
              (this->currentIndex + 4) <= this->data.length() &&
              this->data[this->currentIndex + 1] == 'a' &&
              this->data[this->currentIndex + 2] == 'l' &&
              this->data[this->currentIndex + 3] == 's' &&
              this->data[this->currentIndex + 4] == 'e'
            ){
              this->currentIndex += 4;

              this->token.value = "false";
              this->token.type = Types::boolean;
              this->tokens.push_back(this->token);

              this->value = "";

              break;
            }

          //// Null
          case 'n':
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
              break;
            }

            else if(
              (this->currentIndex + 3) <= this->data.length() &&
              this->data[this->currentIndex + 1] == 'u' &&
              this->data[this->currentIndex + 2] == 'l' &&
              this->data[this->currentIndex + 3] == 'l'
            ){
              this->currentIndex += 3;

              this->token.value = "null";
              this->token.type = Types::null;
              this->tokens.push_back(this->token);

              this->value = "";

              break;

            }


          default:
            if(this->type == Types::string){
              this->value += this->data[this->currentIndex];
            }

            else if(this->type == Types::realNumber){
              this->token.value = this->value;
              this->token.type = Types::realNumber;
              this->tokens.push_back(this->token);

              this->type = Types::unset;
              this->value = "";
            }

            break;

        }

      }

    }

    void rollBackToToken(){
      if(this->currentIndex >= this->data.length()) this->currentIndex = this->previousIndex;

    }


    ///////// Variables
    std::fstream file;
    std::string data;
    unsigned int currentIndex = 0;
    unsigned int previousIndex = 0;

    Types type;
    Delimiters delimiter;
    std::string value;

    Token token;
    std::vector<Token> tokens;

  };

}

#endif
