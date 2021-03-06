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
    CodeException::CodeException(std::string message, std::string line, Token t) : std::runtime_error("CodeException"), msg(message), line(line), token(t)
    {
        std::stringstream s;
        std::string padding(token.getCol(), ' ');
        s << "L " << t.getRow() << ":" << t.getCol() << " ";
        s << msg << std::endl;
        s << line << std::endl;
        s << padding << '^' << std::endl;
        
        msg = s.str();
    }

    const char *CodeException::what() const noexcept
    {        
        return msg.c_str();
    }


    Lexer::Lexer()
    {
    }

    Lexer::Lexer(std::string path)
    {
        tokenizeFile(path);
    }

    Token Lexer::getNext()
    {
    if(it == tokens.end())
            return Token(TokenType::Eof,"");

        Token tk = *it;
        ++it;

        return tk;
    }

    std::string Lexer::getLine(int lineno)
    {
        int idx = lineno - 1;

        if (idx < 0 || idx >= lines.size())
            return "";

        return lines[idx];
    }

    void Lexer::appendToken(std::string data)
    {
        Token tk(TokenType::Symbol, data);
        tk.setCol(st_col-data.size());
        tk.setRow(st_row);

        if(data.size() >= 1){
            if(data[0] == '$'){
               tk.setType(TokenType::Reg);
               tk.setValue(data.substr(1, data.size()));
            }

            if(data[0] == '#'){
                tk.setType(TokenType::Ref);
                tk.setValue(data.substr(1, data.size()));
            }

            if(data[0] >= '0' && data[0] <= '9'){
                tk.setType(TokenType::Value);
            }
        }

        if(keywords.find(data) != keywords.end()){
            tk.setType(keywords[data]);
        }

        tokens.push_back(tk);
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
    
    // TODO : Implement correctly row/col counting
    void Lexer::tokenizeString(std::string data)
    {
        std::string current_token;
        std::string current_line;

        // State -1 : Not in quoted string
        // State 0 : In quoted string
        // State 1 : Escape sequence
        int state = -1;
        st_row = 1;
        st_col = 0;
        char current_char;
        
        for (std::string::iterator it = data.begin(); it != data.end(); ++it) {
            current_char = *it;
            st_col++;

            if (current_char == '\n') {
                lines.push_back(current_line);
                current_line = "";
                st_col = 0;
            } else {
                current_line += current_char;
            }

            if (state >= 0) {
                switch(state) {
                    case 0:
                        if (current_char == '"') {
                            state = -1;

                            Token t(TokenType::String,current_token);
                            t.setCol(st_col-current_token.size());
                            t.setRow(st_row);

                            appendToken(t);
                            current_token.clear();
                        } else if(current_char == '\\') {
                            state = 1;
                        } else {
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
                            
                            lines.push_back(current_line);
                            current_line = "";

                            st_row++;
                            st_col = 0;
                            break;
                        case '\n':
                            lines.push_back(current_line);
                            current_line = "";

                            st_col = 0;
                            st_row++;
                            break;
                        case '"':
                            state = 0;
                            break;
                        case ',':
                        case '[':
                        case ']':
                            if(current_token.length() > 0){
                                appendToken(current_token);
                                current_token.clear();
                            }

                            // TODO : Clean this
                            switch(current_char){
                                case ',':
                                    appendToken(Token(TokenType::Sep,","));
                                    break;
                                case ']':
                                    appendToken(Token(TokenType::Rbrack,"]"));
                                    break;
                                case '[':
                                    appendToken(Token(TokenType::Lbrack,"["));
                                    break;
                            }
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

        if(current_line.length() > 0)
            lines.push_back(current_line);
        
        appendToken(Token(TokenType::Eof, ""));
        it = tokens.begin();
    }
}
