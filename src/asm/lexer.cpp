#include "asm/lexer.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iostream>

static char getEscape(char code)
{
    switch(code){
        case 'n':
            return '\n';
            break;
        case 't':
            return '\t';
            break;
        case 'r':
            return '\r';
            break;
        default:
            return code;
    }
}

namespace Asm {
    Lexer::Lexer()
    {
        it = tokens.begin();
    }

    Lexer::Lexer(std::string path)
    {
        tokenizeFile(path);
    }

    void Lexer::appendToken(std::string data)
    {
        // For debug we add everything as a symbol
        std::cout << "Token : " << data << std::endl;
        tokens.push_back(Token(TokenType::Symbol,data));
    }

    void Lexer::appendToken(Token t)
    {
        tokens.push_back(t);
    }

    void Lexer::tokenizeFile(std::string path)
    {
        std::fstream fd;
        std::stringstream s;

        fd.open(path, std::fstream::in);

        if(!fd.is_open()){
            throw std::runtime_error("Could not open input file");
        }

        s << fd.rdbuf();
        fd.close();

        tokenizeString(s.str());
    }

    void Lexer::tokenizeString(std::string data)
    {
        std::string current_token;
        
        // State -1 : Not in quoted string
        // State 0 : In quoted string
        // State 1 : Escape sequence
        int state = -1;
        int row = 1;
        int col = 0;
        char current_char;
        
        for(std::string::iterator it = data.begin(); it != data.end(); ++it){
            current_char = *it;

            if(state >= 0){
                switch(state){
                    case 0:
                        if(current_char == '"'){
                            state = -1;
                            appendToken(current_token);
                            current_token.clear();
                        }else if(current_char == '\\'){
                            state = 1;
                        }else{
                            current_token += current_char;
                        }
                        break;
                    case 1:
                        current_token += getEscape(current_char);
                        state = 0;
                    default:
                        break;
                        // Do nothing, maybe exception ?
                }
            }else{
            
                if(isspace(current_char)){
                    if(current_token.length() > 0){
                        appendToken(current_token);
                        current_token.clear();
                    }
                }else{
                    switch(current_char){
                        case ';': // Comment
                            while(*it != '\n' && it != data.end()) // Skips comments
                                ++it;
                            
                            row++;
                            col = 0;
                            break;
                        case '\n':
                            col = 0;
                            row++;
                            break;
                        case '"':
                            state = 0;
                            break;
                        default:
                            current_token += current_char;
                            break;
                    }
                }
            }
       }

        if(current_token.length() > 0)
            appendToken(current_token); 
    }
}
