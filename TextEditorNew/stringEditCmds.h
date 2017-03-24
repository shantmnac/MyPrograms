#ifndef stringEditCmds_h
#define stringEditCmds_h

int deleteRange(int, int);
int editString(int, int, char);
int insertSymbol(int, int, char);
int replaceSubstring(int, int, char*, int, char*, int);
int deleteBraces(void);

#endif
