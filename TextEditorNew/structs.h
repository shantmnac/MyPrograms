#ifndef structs_h
#define structs_h

struct listOfStrings{
    int curStringSize;
    struct listOfChars *firstChar;
    struct listOfChars *lastChar;
    struct listOfStrings *prev;
    struct listOfStrings *next;
};

struct listOfChars{
    char curChar;
    struct listOfChars *next;
    struct listOfChars *prev;
};

struct bufOfString{
    char *string;
    int size;
};

#endif
