#include "prelude.h"
#include "storage.h"
#include "pug.h"
Bool   newSyntax  = TRUE;

#if MIDDOT
Char   apChar[2]  = {' ', 0xC2};
#else
Char apChar[2]  = {' ', '.'};
#endif

String typeStr[2] = {"::", ":"};
String consStr[2] = {":", "::"};
String bindStr[2] = {"bind", ".."};
String uptoStr[2] = {"..", "..."};

Text textDot;
Text textBind[2], textCons[2];
Cell varDot;
