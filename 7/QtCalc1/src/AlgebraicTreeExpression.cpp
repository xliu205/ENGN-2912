#include <stdlib.h>
#include <string.h>
#include "AlgebraicTreeExpression.hpp"

AlgebraicTreeExpression::~AlgebraicTreeExpression() {
    // TBD
}

AlgebraicTreeExpression::AlgebraicTreeExpression() {
    // TBD
    _root = (AlgebraicTreeNode*)0;
}

bool AlgebraicTreeExpression::isEmpty() const {
    if(this->_root->isInvalid()) return true;
    return false; // TBD
}

//void AlgebraicTreeExpression::rightParenthese(bool valid){
//    this->right_parenthese = valid;
//}


int AlgebraicTreeExpression::getrightParenthese(){
    return right_parenthese;
}

double AlgebraicTreeExpression::evaluate() const {
    if(this->_root->isNumber()) return this->_root->evaluate();
    if(this->_root->isOperation()) return this->_root->evaluate();
    return 0.0; // TBD
}

int AlgebraicTreeExpression::getLength() const{
    return this->actual_length;
}

char* AlgebraicTreeExpression::toString() const {
    // TBD
    unsigned N = this->_root->toStringLength();
    char* str = new char[N];
    memset(str,'\0',N*sizeof(char));
    this->_root->toString(*str);
    return str;
}

void AlgebraicTreeExpression::setRoot(AlgebraicTreeNode* root) {
    // TBD
    this->_root = root;
}

AlgebraicTreeExpression::AlgebraicTreeExpression(const char* expression) {
    // TBD
    unsigned length = 0;
    const char* temp = expression;
    unsigned strLength = strlen(temp);
    unsigned i = 0;
    int parenthese = 0;
    unsigned label = 0;
    //    while(isspace(*(expression+length))) length++;
    
    if (*expression=='('){
        length += _parse(expression, &this->_root);
    }
    
    //    else{
    //        if(parenthese<0) this->actual_length = label+1;
    //        else this->actual_length = parenthese;
    //    }
    this->actual_length = length;
    if(length != strLength){
        this->setRoot(new AlgebraicTreeNumber(0.0));
        this->_root->setInvalid(true);
    }
}

unsigned AlgebraicTreeExpression::_parse
(const char* expression, AlgebraicTreeNode** nodePtr) {
    unsigned cur_len = strlen(expression);
    unsigned length = 0;
    char*    ptr = (char*)expression;
    bool     onlyCheckSyntax = (*nodePtr==(AlgebraicTreeNode*)0);
    unsigned strlen = 0;
    
    while(isspace(*(ptr+length))) length++;
    strlen = length;
    //  if(onlyCheckSyntax==true) {
    //    // TBD
    //
    //  } else /* if(onlyCheckSyntax==false) */ {
    // TBD
    if(*(ptr+length) == '('){
        length++;
        while(isspace(*(ptr+strlen+1))) strlen++;
        if(*(ptr+length) == '('){
            int parentheses = 0;
            bool isOperator = false;
            while(!isOperator){
                strlen++;
                if(*(ptr+strlen) == '(') parentheses++;
                else if(*(ptr+strlen)== ')') parentheses--;
                if(parentheses==0) isOperator=true;
            }
            if(strlen >= cur_len) return length;
            else{
                strlen++;
                while(isspace(*(expression+strlen))){
                    strlen++;
                }
                AlgebraicTreeOperation* operation =new AlgebraicTreeOperation();
                if(*(expression+strlen)=='+') operation->setType(ADD);
                else if(*(expression+strlen)=='-') operation->setType(SUBTRACT);
                else if(*(expression+strlen)=='*') operation->setType(MULTIPLY);
                else if(*(expression+strlen)=='/') operation->setType(DIVIDE);
                else return length;
                length += 1;
                if(onlyCheckSyntax==true) setRoot(operation);
                else *nodePtr = operation;
                operation->setChildLeft(new AlgebraicTreeOperation());
                unsigned cl = _parse(expression+1, operation->getChildLeftPtr());
                if (cl==0) return length;
                length += cl;
                operation->setChildRight(new AlgebraicTreeOperation());
                unsigned cr = _parse(expression+strlen+1,operation->getChildRightPtr());
                if(cr==0) return length;
                length += cr;
                if(*(expression+length)==')' && length < cur_len&& right_parenthese==0) length++;
                else right_parenthese++;
                return length;
            }
        }else{
            AlgebraicTreeNumber* left_number = new AlgebraicTreeNumber();
            char* endPtr;
            double value = strtod(ptr+length, &endPtr);
            unsigned strLen = endPtr-ptr-1;
            if(value==0 && *(ptr+length)!='0'){
                left_number->setInvalid(true);
                left_number->setValue(0);
                return length;
            }
            else{
                left_number->setValue(value);
                length+= strLen;
            }
            AlgebraicTreeOperation* operation = new AlgebraicTreeOperation();
            if(*(expression+length)=='+') operation->setType(ADD);
            else if(*(expression+length)=='-') operation->setType(SUBTRACT);
            else if(*(expression+length)=='*') operation->setType(MULTIPLY);
            else if(*(expression+length)=='/') operation->setType(DIVIDE);
            else {
                right_parenthese++;
                return length;
            }
            length += 1;
            if(onlyCheckSyntax==true) this->setRoot(operation);
            else *nodePtr = operation;
            operation->setChildLeft(left_number);
            if(*(ptr+length)=='('){
                operation->setChildRight(new AlgebraicTreeOperation());
                int c2 = _parse(ptr+length,operation->getChildRightPtr());
                if(c2==0) return length;
                length += c2;
                if(*(expression+length)==')' && length<cur_len && right_parenthese==0) length++;
                return length;
            }else{
                AlgebraicTreeNumber* right_number = new AlgebraicTreeNumber();
                char* endPtr2;
                double value2 = strtod(ptr+length, &endPtr2);
                unsigned strLen2 = endPtr2-ptr-length;
                if(value2==0 && *(ptr+length)!='0'){
                    right_number->setInvalid(true);
                    right_number->setValue(0);
                    return length;
                }
                else{
                    right_number->setValue(value2);
                    length+= strLen2;
                }
                operation->setChildRight(right_number);
                while(isspace(*(expression+length))) length++;
                if(*(expression+length)==')' && length<cur_len) length++;
                else right_parenthese++;
                return length;
            }
        }
    }else{
        AlgebraicTreeNumber* number = new AlgebraicTreeNumber();
        char* endPtr;
        double value = strtod(ptr, &endPtr);
        unsigned strLen = endPtr-expression;
        if(value==0 && *ptr!='0'){
            number->setInvalid(true);
            number->setValue(0);
            return 0;
        }
        else{
            number->setValue(value);
            *nodePtr = number;
            length+= strLen;
            return length;
        }
    }
    return length;
}


