#include "prelude.h"
#include "storage.h"
#include "pug.h"
Bool   newSyntax  = TRUE;
Char   apChar[2]  = {' ', '.'};
String typeStr[2] = {"::", ":"};
String consStr[2] = {":", "::"};
String bindStr[2] = {"bind", ".."};
String uptoStr[2] = {"..", "..."};
String langLevels[4] = {"pug", "pup", "kit", "cat"}; // TODO: Should we validate against these?
static String currentLangLevel = 0;

Text textDot;
Text textBind[2], textCons[2];
Cell varDot;
