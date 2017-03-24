#include <stdio.h>
#include <cstring>
#include <cstdlib>

class ExceptionCatcher{
public:
    ExceptionCatcher(int);
};

ExceptionCatcher::ExceptionCatcher(int size){
    printf("Не удалось выделить %i байт памяти!\n", size);
}

class String{
private:
    char* string;
    int stringLength;
public:
    String();
    String(const char*);
    String(const String&);
    
    ~String();
    
    String operator=(const String&);
    String operator=(const char*);
    String operator+(const String&)const;
    String operator+(const char*)const;
    friend String operator+(const char*, const String);
    
};

String::String():string(NULL), stringLength(0){}

String::String(const char* str){
    if (str != NULL) {
        stringLength = (int)strlen(str);
        if (stringLength) {
            string = (char*)malloc((stringLength + 1) * sizeof(char));
            if (string == NULL) {
                throw ExceptionCatcher((stringLength + 1) * sizeof(char));
            }
            strcpy(string, str);
        }
    }
    else{
        stringLength = 0;
        string = NULL;
    }
}

String::String(const String& str){
    stringLength = str.stringLength;
    if (stringLength) {
        string = (char*)malloc((stringLength + 1) * sizeof(char));
        if (string == NULL) {
            throw ExceptionCatcher((stringLength + 1) * sizeof(char));
        }
        strcpy(string, str.string);
    }
    else string = NULL;
}

String::~String(){
    if (string != NULL) {
        free(string);
        string = NULL;
    }
    stringLength = 0;
}

String String::operator=(const String& str){
    stringLength = str.stringLength;
    if (stringLength) {
        string = (char*)realloc(string, (stringLength + 1) * sizeof(char));
        if (string == NULL) {
            throw ExceptionCatcher((stringLength + 1) * sizeof(char));
        }
        strcpy(string, str.string);
        return *this;
    }
    else string = NULL;
    return *this;
}

String String::operator=(const char* str){
    if (str != NULL) {
        stringLength = (int)strlen(str);
        if (stringLength) {
            string = (char*)realloc(string, (stringLength + 1) * sizeof(char));
            if (string == NULL) {
                throw ExceptionCatcher((stringLength + 1) * sizeof(char));
            }
            strcpy(string, str);
            return *this;
        }
        else return String();
    }
    else return String();
}

String String::operator+(const String& str)const{
    String tmpString;
    tmpString.stringLength = stringLength + str.stringLength;
    tmpString.string = NULL;
    if (tmpString.stringLength) {
        tmpString.string = (char*)malloc((tmpString.stringLength + 1) * sizeof(char));
        if (tmpString.string == NULL) {
            throw ExceptionCatcher((tmpString.stringLength + 1) * sizeof(char));
        }
        if (string != NULL) {
            strcpy(tmpString.string, string);
        }
        if (str.string != NULL) {
            strcat(tmpString.string, str.string);
        }
        
        return tmpString;
    }
    else{
        return String();
    }
}

String String::operator+(const char* str)const{
    String tmpString;
    tmpString.string = string;
    if (str != NULL) {
        tmpString.stringLength = stringLength + (int)strlen(str);
        tmpString.string = NULL;
        if (tmpString.stringLength) {
            tmpString.string = (char*)malloc((tmpString.stringLength + 1) * sizeof(char));
            if (tmpString.string == NULL) {
                throw ExceptionCatcher((tmpString.stringLength + 1) * sizeof(char));
            }
            if (string != NULL) {
                strcpy(tmpString.string, string);
            }
            if (str != NULL) {
                strcat(tmpString.string, str);
            }
            return tmpString;
        }
        else{
            return String();
        }
    }
    else return tmpString;
}

String operator+(const char* str1, const String str2){
    String tempStr;
    if (str1 != NULL) {
        tempStr.stringLength = str2.stringLength + (int)strlen(str1);
        if (tempStr.stringLength) {
            tempStr.string = (char*)malloc((tempStr.stringLength + 1) * sizeof(char));
            if (tempStr.string == NULL) {
                throw ExceptionCatcher((tempStr.stringLength + 1) * sizeof(char));
            }
            if (str1 != NULL) {
                strcpy(tempStr.string, str1);
            }
            if (str2.string != NULL) {
                strcat(tempStr.string, str2.string);
            }
        }
        return tempStr;
    }
    else{
        tempStr.stringLength = str2.stringLength;
        if (tempStr.stringLength) {
            tempStr.string = (char*)malloc((tempStr.stringLength + 1) * sizeof(char));
            if (tempStr.string == NULL) {
                throw ExceptionCatcher((tempStr.stringLength + 1) * sizeof(char));
            }
            if (str2.string != NULL) {
                strcpy(tempStr.string, str2.string);
            }
        }
        return tempStr;
    }
}

int main(){
    const String str1("bbb");
    String str2("ccc");
    String str3("aaa");
    String str = "ddd";
    
    str2 = str1;
    str2 = str2 + str3;
    str3 = str1 + "asd";
    str3 = "qwe" + str1;
    
    return 0;
}