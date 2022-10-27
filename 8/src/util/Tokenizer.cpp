
//
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include "Tokenizer.hpp"
static unsigned line_num = 1;
bool Tokenizer::get(){
    char cstr[512];
    char c;
    if(_ifstr.peek()!=EOF){
        while(isspace(_ifstr.peek())||_ifstr.peek()==','){
            _ifstr.get();
        }
        if(_ifstr.peek()=='{'||_ifstr.peek()=='}'||_ifstr.peek()=='['||_ifstr.peek()==']'){
            _ifstr.get(c);
            string str(1,c);
            str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
            str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
            this->std::__1::basic_string<char>::operator=(str);
        }else{
            string str = "";
            while(!isspace(_ifstr.peek())&&_ifstr.peek()!=','){
                str += _ifstr.get();
            }
            this->std::__1::basic_string<char>::operator=(str);
        }
        return 1;
    }else{
        return 0;
    }
}

bool Tokenizer::expecting(const string& str){
    this->erase(std::remove(this->begin(), this->end(), '\r'), this->end());
    this->erase(std::remove(this->begin(), this->end(), '\n'), this->end());
    return get() && (str==*this);
}

unsigned Tokenizer::getLineNum(){
    return line_num;
}
