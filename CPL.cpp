#include <cstdio>
#include <cstdlib>

const int chrAmpersand = 38;
const int chrApostrophe = 39;
const int chrAsterisk = 42;
const int chrBackslash = 92;
const int chrDel = 127;
const int chrDollar = 36;
const int chrEquals = 61;
const int chrExclamationMark = 33;
const int chrGreater = 62;
const int chrHash = 35;
const int chrLBrace = 123;
const int chrLBracket = 91;
const int chrLess = 60;
const int chrLowerA = 97;
const int chrLowerZ = 122;
const int chrLParen = 40;
const int chrMinus = 45;
const int chrNine = 57;
const int chrPercent = 37;
const int chrPipe = 124;
const int chrPlus = 43;
const int chrQuote = 34;
const int chrRBrace = 125;
const int chrRBracket = 93;
const int chrRParen = 41;
const int chrSemicolon = 59;
const int chrSlash = 47;
const int chrSpace = 32;
const int chrUpperA = 65;
const int chrUpperZ = 90;
const int chrZero = 48;
const int eof = 0;
const int valuesSize = 1000;
const int infoSize = 5000;
const int symConst = 3;
const int symFun = 1;
const int symKeyword = 4;
const int symUnknown = 5;
const int symVar = 0;
const int tokAdd = 13;
const int tokAnd = 18;
const int tokAssign = 30;
const int tokConst = 22;
const int tokDiv = 26;
const int tokEof = 0;
const int tokEqual = 4;
const int tokFun = 31;
const int tokGreater = 16;
const int tokGreaterOrEqual = 17;
const int tokHash = 21;
const int tokIf = 23;
const int tokInclude = 27;
const int tokInt = 28;
const int tokLBrace = 8;
const int tokLBracket = 9;
const int tokLess = 14;
const int tokLessOrEqual = 15;
const int tokLParen = 5;
const int tokMod = 36;
const int tokMul = 25;
const int tokName = 1;
const int tokNot = 12;
const int tokNotEqual = 34;
const int tokNumber = 2;
const int tokOr = 19;
const int tokRBrace = 10;
const int tokRBracket = 11;
const int tokReturn = 33;
const int tokRParen = 6;
const int tokSemicolon = 7;
const int tokStatic = 37;
const int tokString = 35;
const int tokSub = 20;
const int tokVar = 32;
const int tokVoid = 29;
const int tokWhile = 24;

static char curTxtChr;
static char datSeg[] = "section '.data' data readable writeable";
static char hdr[] = "format PE Console\nentry start\nsection '.idata' import data readable writeable\ndd 0\ndd 0\ndd 0\ndd rva kernelName\ndd rva kernelTable\ndd 0\ndd 0\ndd 0\ndd 0\ndd 0\nkernelTable:\nExitProcess dd rva exitProcess\nReadFile dd rva readFile\nWriteFile dd rva writeFile\nGetStdHandle dd rva getStdHandle\ndd 0\nkernelName:\ndb 'KERNEL32.DLL',0\nexitProcess:\ndw 0\ndb 'ExitProcess',0\nreadFile:\ndw 0\ndb 'ReadFile',0\nwriteFile:\ndw 0\ndb 'WriteFile',0\ngetStdHandle:\ndw 0\ndb 'GetStdHandle',0\nsection '.data' data readable writeable\nstdInHandle dd ?\nstdOutHandle dd ?\nioChr db ?\nbytesCount dd ?\nsection '.text' code readable executable\nexit:\ncall [ExitProcess]\ngetchar:\npush ebp\nmov ebp, esp\nmov [ioChr], 0\npush 0\npush bytesCount\npush 1\npush ioChr\npush [stdInHandle]\ncall [ReadFile]\nmov eax, 0\nmov al, [ioChr]\npop ebp\nret 0 \nputchar:\npush ebp\nmov ebp, esp\nmov eax, [ebp + 8]\nmov [ioChr], al\npush 0\npush bytesCount\npush 1\npush ioChr\npush [stdOutHandle]\ncall [WriteFile]\npop ebp\nret 4 \nteq:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njne @f\ninc ecx\n@@: push ecx\njmp edx\ntne:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\nje @f\ninc ecx\n@@: push ecx\njmp edx\ntls:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njge @f\ninc ecx\n@@: push ecx\njmp edx\ntle:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njg @f\ninc ecx\n@@: push ecx\njmp edx\ntgr:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njle @f\ninc ecx\n@@: push ecx\njmp edx\ntge:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njl @f\ninc ecx\n@@: push ecx\njmp edx\nadi:\npop edx\npop ebx\npop eax\nlea eax,[eax+4*ebx]\npush eax\njmp edx\nstart:\npush 0FFFFFFF6h\ncall [GetStdHandle]\nmov [stdInHandle], eax \npush 0FFFFFFF5h\ncall [GetStdHandle]\nmov [stdOutHandle], eax \ncall main\npush 0\ncall [ExitProcess]\n";
static char jmpBegin[] = "jmp b#0\n";
static char jmpEnd[] = "pop eax\ncmp eax,0\nje e#0\n";
static char opRead[] = "pop eax\npush dword[eax]\n";
static char opAdi[] = "call adi\n";
static char opTeq[] = "call teq\n";
static char opTge[] = "call tge\n";
static char opTgr[] = "call tgr\n";
static char opTle[] = "call tle\n";
static char opTls[] = "call tls\n";
static char opTne[] = "call tne\n";
static char opWrite[] = "pop ebx\npop eax\nmov [eax],ebx\n";
static char opCall[] = "call %0\n";
static char opPushAddress[] = "push %0\n";
// static char opVarDcl[] = "%0 dd ?";
// static char opArrDcl[] = "%0 dd #0 dup(?)";
// static char opStrDcl[] = "%0 dd @0";
static char opFun[] = "%0:\n";
static char txtSeg[] = "section '.text' code readable executable";
static char* string;
static char* strings[10]; // texts for string placeholders
static char* text; // Test to be put out.
static int nameIndices[10]; // indices for name placeholders
static int nameIndex;
static int findPointer; // index of the find in "info" by calling "FdSym"
static int buffer[12];
static int bufferIndex;
static int chr;
static int curChr;
static int curSym;
static int curTok;
static int curVal;
static int funIdx;
static int hasInit;
static int i;
static int j;
static int limit;
static int minusOne;
static int info[infoSize]; // various information (e.g. names) necessary during compilation
static int namesPointer; // index of the top-most name in "info"
static int number;
static int numbers[10]; // numbers for number placeholders
static int nxtChr;
static int nxtLab;
static int op;
static int reg;
static int stringIndex;
static int symbolPointer; // index of the newly defined symbol in "info".
static int textIndex;
static int values[valuesSize];
static int valuesPointer; // index of the top-most value in "values"
static char txtFail[] = "fail\n";

static void PutNumber()
{
  bufferIndex = 0;
  buffer[0] = number % 10;
  number = number / 10;
  while (0 < number)
  {
    bufferIndex = bufferIndex + 1;
    buffer[bufferIndex] = number % 10;
    number = number / 10;
  }
  while (0 <= bufferIndex)
  {
    putchar(buffer[bufferIndex] + 48);
    bufferIndex = bufferIndex - 1;
  }
}

static void PutString()
{
  stringIndex = 0;
  while (string[stringIndex])
  {
    putchar(string[stringIndex]);
    stringIndex = stringIndex + 1;
  }
}

static void PutName()
{
  i = 0;
  limit = info[nameIndex + 1];
  while (i < limit)
  {
    putchar(info[nameIndex + i + 2]);
    i = i + 1;
  }
}

static int ExpandPlaceholders()
{
  if (curTxtChr == chrBackslash)
  {
    textIndex = textIndex + 1;
    curTxtChr = text[textIndex];
    if (curTxtChr == 'n')
    {
      curTxtChr = 10;
    }
    putchar(curTxtChr);
    textIndex = textIndex + 1;
    return 1;
  }
  if (curTxtChr == chrHash) // number ...
  {
    number = numbers[text[textIndex + 1] - 48];
    PutNumber();
    textIndex = textIndex + 2;
    return 1;
  }
  if (curTxtChr == chrDollar) // zero-terminated string ...
  {
    string = strings[text[textIndex + 1] - 48];
    PutString();
    textIndex = textIndex + 2;
    return 1;
  }
  if (curTxtChr == chrPercent) // name ...
  {
    nameIndex = nameIndices[text[textIndex + 1] - 48];
    PutName();
    textIndex = textIndex + 2;
    return 1;
  }
  return 0;
}

// Output text to stdout. The text may contain placeholders.       
void PutTxt()
{
  textIndex = 0;
  while (1)
  {
    curTxtChr = text[textIndex];
    if (!curTxtChr)
    {
      return;
    }
    if (!ExpandPlaceholders())
    {
      putchar(curTxtChr);
      textIndex = textIndex + 1;
    }
  }
}

// exit program with failure-code 1
static void Fail()
{
  text = txtFail; PutTxt();
  exit(1);
}

static void PushValue()
{
  valuesPointer = valuesPointer + 1;
  values[valuesPointer] = nxtLab;
  nxtLab = nxtLab + 1;
}

static void PopValue()
{
  valuesPointer = valuesPointer - 1;
}

static int TopValue()
{
  return values[valuesPointer];
}

static void EmtCmp()
{
  putchar('c'); putchar('m'); putchar('p');
}

static void EmtDec()
{
  putchar('d'); putchar('e'); putchar('c');
}

static void EmtJe()
{
  putchar('j'); putchar('e'); 
}

static void EmtMov()
{
  putchar('m'); putchar('o'); putchar('v');
}

static void EmtPop()
{
  putchar('p'); putchar('o'); putchar('p');
}

static void EmtPush()
{
  putchar('p'); putchar('u'); putchar('s'); putchar('h');
}

static void EmtNam()
{
  nameIndex = findPointer; PutName();
}

static void EmtVal()
{
  number = curVal;
  PutNumber();
}

static void EmtDcl()
{
  EmtNam(); putchar(' '); putchar('d'); putchar('d'); putchar(' ');
}

static void EmtDup()
{
  EmtVal(); putchar(' '); putchar('d'); putchar('u'); putchar('p'); putchar('('); putchar('?'); putchar(')');
}

static void EmtFun()
{
  text = opFun; nameIndices[0] = findPointer; PutTxt();
}

static void EmtDatSeg()
{
  text = datSeg; PutTxt(); putchar(10);
}

static void EmtTxtSeg()
{
  text = txtSeg; PutTxt(); putchar(10);
}

static void EmtHdr()
{
  text = hdr; PutTxt();
}

static void EmtOp()
{
  if (op == tokAdd)
  {
    putchar('a'); putchar('d'); putchar('d'); 
  }
  if (op == tokSub)
  {
    putchar('s'); putchar('u'); putchar('b'); 
  }
  if (op == tokMul)
  {
    putchar('m'); putchar('u'); putchar('l'); 
  }
  if (op == tokDiv)
  {
    putchar('d'); putchar('i'); putchar('v'); 
  }
  if (op == tokMod)
  {
    putchar('d'); putchar('i'); putchar('v'); 
  }
  if (op == tokAnd)
  {
    putchar('a'); putchar('n'); putchar('d'); 
  }
  if (op == tokOr)
  {
    putchar('o'); putchar('r');
  }
}

static void EmtReg()
{
  putchar('e'); putchar(reg); putchar('x'); 
}

static void EmtPushNam()
{
  // EmtPush(); putchar(' '); EmtNam(); putchar(10);
  text = opPushAddress; nameIndices[0] = findPointer; PutTxt();
}

static void EmtPushReg()
{
  EmtPush(); putchar(' '); EmtReg(); putchar(10);
}

static void EmtPushVal()
{
  EmtPush(); putchar(' '); EmtVal(); putchar(10);
}

static void EmtPopReg()
{
  EmtPop(); putchar(' '); EmtReg(); putchar(10);
}

static void EmtRet()
{
  putchar('r'); putchar('e'); putchar('t'); putchar(10);
}

static void EmtAsgn()
{
  text = opWrite; PutTxt();
}

static void EmtProcCall()
{
  text = opCall; nameIndices[0] = funIdx; PutTxt();
}

static void EmtFunCall()
{
  EmtProcCall();
  reg = 'a'; EmtPushReg();
}

static void EmtOperation()
{
  reg = 'b'; EmtPopReg();
  reg = 'a'; EmtPopReg();
  if ((op == tokDiv) || (op == tokMod))
  {
    EmtMov(); putchar(' '); reg = 'd'; EmtReg(); putchar(','); curVal = 0; EmtVal(); putchar(10);
  }
  EmtOp(); putchar(' '); 
  if ((op == tokAdd) || (op == tokSub) || (op == tokAnd) || (op == tokOr))
  {
    reg = 'a'; EmtReg(); putchar(',');
  }
  reg = 'b'; EmtReg(); putchar(10);
  if ((op == tokAdd) || (op == tokSub) || (op == tokMul) || (op == tokDiv) || (op == tokAnd) || (op == tokOr))
  {
    reg = 'a';
  }
  if (op == tokMod)
  {
    reg = 'd';
  }
  EmtPushReg();
}

static void EmtNot()
{
  EmtMov(); putchar(' '); reg = 'c'; EmtReg(); putchar(','); curVal = 1; EmtVal(); putchar(10);
  EmtMov(); putchar(' '); reg = 'b'; EmtReg(); putchar(','); curVal = 0; EmtVal(); putchar(10);
  reg = 'a'; EmtPopReg();
  EmtCmp(); putchar(' '); reg = 'a'; EmtReg(); putchar(','); reg = 'b'; EmtReg(); putchar(10);
  EmtJe(); putchar(' '); putchar('@'); putchar('f'); putchar(10);
  EmtDec(); putchar(' '); reg = 'c'; EmtReg(); putchar(10);
  putchar('@'); putchar('@'); putchar(':'); putchar(10);
  reg = 'c'; EmtPushReg();
}

static void EmtNeg()
{
  EmtMov(); putchar(' '); reg = 'a'; EmtReg(); putchar(','); curVal = 1; EmtVal(); putchar(10);
  reg = 'b'; EmtPopReg();
  op = tokSub; EmtOp(); putchar(' '); reg = 'a'; EmtReg(); putchar(','); reg = 'b'; EmtReg(); putchar(10);
  reg = 'a'; EmtPushReg();
}

static void EmtAddIdx()
{
  // text = addIdx; PutTxt();
  text = opAdi; PutTxt();
}

static void EmtBegin()
{
  putchar('b'); curVal = TopValue(); EmtVal(); putchar(':'); putchar(10);
}

static void EmtJmpBegin()
{
  text = jmpBegin; numbers[0] = TopValue(); PutTxt();
}

static void EmtEnd()
{
  putchar('e'); curVal = TopValue(); EmtVal(); putchar(':'); putchar(10);
}

static void EmtJmpEnd()
{
  text = jmpEnd; numbers[0] = TopValue(); PutTxt();
}

static void EmtRead()
{
  text = opRead; PutTxt();
}

static void EmtStrInt()
{
  i = 0;
  limit = info[namesPointer + 1];
  while (i < limit)
  {
    curVal = info[namesPointer + i + 2];
    EmtVal();
    putchar(',');
    i = i + 1;
  }
  putchar('0');
}

static void EmtArrAddr()
{
  putchar('$'); putchar('+'); putchar('4'); putchar(',');
}

static void AddChr()
{
  info[namesPointer + 1] = info[namesPointer + 1] + 1;
  info[namesPointer + info[namesPointer + 1] + 1] = chr;
}

static void AddCurChr()
{
  chr = curChr; AddChr();
}

static int IsCtrl()
{
  return (curChr < chrSpace) || (chrDel <= curChr);
}

static int IsWhitespace()
{
  return IsCtrl() || (curChr == ' ');
}

static int IsDigit()
{
  return (chrZero <= curChr) && (curChr <= chrNine);
}

static int IsLetter()
{
  return ((chrLowerA <= curChr) && (curChr <= chrLowerZ)) || ((chrUpperA <= curChr) && (curChr <= chrUpperZ));
}

static void SetSym()
{
  symbolPointer = namesPointer + info[namesPointer + 1];
  info[symbolPointer + 2] = curSym;
  info[symbolPointer + 3] = curVal;
  info[symbolPointer + 4] = namesPointer;
  namesPointer = symbolPointer + 4;
}

static int FndSym()
{
  i = info[namesPointer];
  while (1)
  {
    limit = info[i + 1];
    if (limit == info[namesPointer + 1])
    {
      j = 0;
      while (j < limit)
      {
        if (info[i + j + 2] != info[namesPointer + j + 2])
        {
          j = limit;
        }
        j = j + 1;
      }
      if (j == limit)
      {
        curSym = info[i + info[i + 1] + 2];
        curVal = info[i + info[i + 1] + 3];
        return 1;
      }
    }
    if (i == minusOne)
    {
      return 0;
    }
    i = info[i];
  }
}

static void GetNxtChr()
{
  curChr = nxtChr;
  nxtChr = getchar();
  if (nxtChr < 0)
  {
    nxtChr = 0;
  }
}

static void GetChr()
{
  GetNxtChr();
  while ((curChr == '/') && (nxtChr == '/'))
  {
    while ((curChr != eof) && (curChr != 10))
    {
      GetNxtChr();
    }
    GetNxtChr();
  }
}

static void RdTok()
{
  info[namesPointer + 1] = 0;
  while ((curChr != eof) && IsWhitespace())
  {
    GetChr();
  }
  curTok = tokEof;
  if (curChr == eof)
  {
    return;
  }
  if (curChr == chrLBrace)
  {
    GetChr();
    curTok = tokLBrace;
    return;
  }
  if (curChr == chrLBracket)
  {
    GetChr();
    curTok = tokLBracket;
    return;
  }
  if (curChr == chrLParen)
  {
    GetChr();
    curTok = tokLParen;
    return;
  }
  if (curChr == chrRBrace)
  {
    GetChr();
    curTok = tokRBrace;
    return;
  }
  if (curChr == chrRBracket)
  {
    GetChr();
    curTok = tokRBracket;
    return;
  }
  if (curChr == chrRParen)
  {
    GetChr();
    curTok = tokRParen;
    return;
  }
  if (curChr == chrSemicolon)
  {
    GetChr();
    curTok = tokSemicolon;
    return;
  }
  if (curChr == chrPlus)
  {
    GetChr();
    curTok = tokAdd;
    return;
  }
  if (curChr == chrMinus)
  {
    GetChr();
    curTok = tokSub;
    return;
  }
  if (curChr == chrAsterisk)
  {
    GetChr();
    curTok = tokMul;
    return;
  }
  if (curChr == chrSlash)
  {
    GetChr();
    curTok = tokDiv;
    return;
  }
  if (curChr == chrPercent)
  {
    GetChr();
    curTok = tokMod;
    return;
  }
  if (curChr == chrEquals)
  {
    GetChr();
    if (curChr == chrEquals)
    {
      GetChr();
      curTok = tokEqual;
      return;
    }
    curTok = tokAssign;
    return;
  }
  if (curChr == chrExclamationMark)
  {
    GetChr();
    if (curChr == chrEquals)
    {
      GetChr();
      curTok = tokNotEqual;
      return;
    }
    curTok = tokNot;
    return;
  }
  if (curChr == chrLess)
  {
    GetChr();
    if (curChr == chrEquals)
    {
      GetChr();
      curTok = tokLessOrEqual;
      return;
    }
    curTok = tokLess;
    return;
  }
  if (curChr == chrGreater)
  {
    GetChr();
    if (curChr == chrEquals)
    {
      GetChr();
      curTok = tokGreaterOrEqual;
      return;
    }
    curTok = tokGreater;
    return;
  }
  if (curChr == chrAmpersand)
  {
    GetChr();
    if (curChr == chrAmpersand)
    {
      GetChr();
      curTok = tokAnd;
      return;
    }
    return;
  }
  if (curChr == chrPipe)
  {
    GetChr();
    if (curChr == chrPipe)
    {
      GetChr();
      curTok = tokOr;
      return;
    }
    return;
  }
  if (curChr == chrHash)
  {
    GetChr();
    curTok = tokHash;
    return;
  }
  if (curChr == chrApostrophe)
  {
    GetChr();
    if (!IsCtrl())
    {
      curVal = curChr;
      GetChr();
      if (curChr == chrApostrophe)
      {
        GetChr();
        curTok = tokNumber;
        return;
      }
    }
  }
  if (curChr == chrQuote)
  {
    GetChr();
    while ((curChr != eof) && (curChr != chrQuote))
    {
      if (curChr == chrBackslash)
      {
        GetChr();
        if (curChr == 'n')
        {
          curChr = 10;
        }
      }
      AddCurChr();
      GetChr();
    }
    if (curChr == chrQuote)
    {
      GetChr();
      curTok = tokString;
      return;
    }
  }
  if (IsLetter())
  {
    while ((curChr != eof) && IsLetter())
    {
      AddCurChr();
      GetChr();
    }
    if (FndSym())
    {
      if (curSym == symKeyword)
      {
        curTok = curVal;
        return;
      }
      if (curSym == symConst)
      {
        curTok = tokNumber;
        return;
      }
      if (curSym == symFun)
      {
        curTok = tokFun;
        return;
      }
      if (curSym == symVar)
      {
        curTok = tokVar;
        return;
      }
      return;
    }
    curTok = tokName;
    return;
  }
  if (IsDigit())
  {
    curVal = 0;
    while ((curChr != eof) && IsDigit())
    {
      curVal = (curVal * 10) + (curChr - chrZero);
      AddCurChr();
      GetChr();
    }
    curTok = tokNumber;
    return;
  }
  Fail();
}

static void Init()
{
  minusOne = -1;

  EmtHdr();

  nxtLab = 0;
  valuesPointer = minusOne;
  namesPointer = minusOne;

  curChr = 'c'; AddCurChr();
  curChr = 'o'; AddCurChr();
  curChr = 'n'; AddCurChr();
  curChr = 's'; AddCurChr();
  curChr = 't'; AddCurChr();
  curSym = symKeyword;
  curVal = tokConst;
  SetSym();

  curChr = 's'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'a'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'i'; AddCurChr();
  curChr = 'c'; AddCurChr();
  curSym = symKeyword;
  curVal = tokStatic;
  SetSym();

  curChr = 'r'; AddCurChr();
  curChr = 'e'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'u'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curChr = 'n'; AddCurChr();
  curSym = symKeyword;
  curVal = tokReturn;
  SetSym();

  curChr = 'i'; AddCurChr();
  curChr = 'f'; AddCurChr();
  curSym = symKeyword;
  curVal = tokIf;
  SetSym();

  curChr = 'w'; AddCurChr();
  curChr = 'h'; AddCurChr();
  curChr = 'i'; AddCurChr();
  curChr = 'l'; AddCurChr();
  curChr = 'e'; AddCurChr();
  curSym = symKeyword;
  curVal = tokWhile;
  SetSym();

  curChr = 'i'; AddCurChr();
  curChr = 'n'; AddCurChr();
  curChr = 'c'; AddCurChr();
  curChr = 'l'; AddCurChr();
  curChr = 'u'; AddCurChr();
  curChr = 'd'; AddCurChr();
  curChr = 'e'; AddCurChr();
  curSym = symKeyword;
  curVal = tokInclude;
  SetSym();

  curChr = 'i'; AddCurChr();
  curChr = 'n'; AddCurChr();
  curChr = 't'; AddCurChr();
  curSym = symKeyword;
  curVal = tokInt;
  SetSym();

  curChr = 'c'; AddCurChr();
  curChr = 'h'; AddCurChr();
  curChr = 'a'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curSym = symKeyword;
  curVal = tokInt;
  SetSym();

  curChr = 'v'; AddCurChr();
  curChr = 'o'; AddCurChr();
  curChr = 'i'; AddCurChr();
  curChr = 'd'; AddCurChr();
  curSym = symKeyword;
  curVal = tokVoid;
  SetSym();

  curChr = 'a'; AddCurChr();
  curChr = 'b'; AddCurChr();
  curChr = 'o'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curChr = 't'; AddCurChr();
  curSym = symFun;
  curVal = tokVoid;
  SetSym();

  curChr = 'g'; AddCurChr();
  curChr = 'e'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'c'; AddCurChr();
  curChr = 'h'; AddCurChr();
  curChr = 'a'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curSym = symFun;
  curVal = tokInt;
  SetSym();

  curChr = 'p'; AddCurChr();
  curChr = 'u'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'c'; AddCurChr();
  curChr = 'h'; AddCurChr();
  curChr = 'a'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curSym = symFun;
  curVal = tokVoid;
  SetSym();

  curChr = 'e'; AddCurChr();
  curChr = 'x'; AddCurChr();
  curChr = 'i'; AddCurChr();
  curChr = 't'; AddCurChr();
  curSym = symFun;
  curVal = tokVoid;
  SetSym();

  curSym = symUnknown;
  curVal = 0;
  nxtChr = 0;
  GetNxtChr();
  GetChr();
  RdTok();
}

static void ParseNumber()
{
  if (curTok == tokSub)
  {
    RdTok();
    if (curTok != tokNumber)
    {
      Fail();
    }
    curVal = -curVal;
    RdTok();
    return;
  }
  if (curTok != tokNumber)
  {
    Fail();
  }
  RdTok();
}

static void ParseExpression()
{
  if (curTok == tokNot)
  {
    RdTok();
    ParseExpression();
    EmtNot();
  }
  if (curTok == tokSub)
  {
    RdTok();
    ParseExpression();
    EmtNeg();
  }
  if (curTok == tokNumber)
  {
    EmtPushVal();
    RdTok();
  }
  if (curTok == tokVar)
  {
    findPointer = namesPointer;
    EmtPushNam();
    RdTok();
    if (curTok == tokLBracket)
    {
      EmtRead();
      RdTok();
      ParseExpression();
      if (curTok != tokRBracket)
      {
        Fail();
      }
      EmtAddIdx();
      RdTok();
    }
    EmtRead();
  }
  if (curTok == tokFun)
  {
    funIdx = i;
    RdTok();
    if (curTok != tokLParen)
    {
      Fail();
    }
    RdTok();
    if (curTok != tokRParen)
    {
      Fail();
    }
    EmtFunCall();
    RdTok();
  }
  if (curTok == tokLParen)
  {
    RdTok();
    ParseExpression();
    if (curTok != tokRParen)
    {
      Fail();
    }
    RdTok();
  }
  if (curTok == tokAdd)
  {
    RdTok();
    ParseExpression();
    op = tokAdd;
    EmtOperation();
  }
  if (curTok == tokSub)
  {
    RdTok();
    ParseExpression();
    op = tokSub;
    EmtOperation();
  }
  if (curTok == tokMul)
  {
    RdTok();
    ParseExpression();
    op = tokMul;
    EmtOperation();
  }
  if (curTok == tokDiv)
  {
    RdTok();
    ParseExpression();
    op = tokDiv;
    EmtOperation();
  }
  if (curTok == tokMod)
  {
    RdTok();
    ParseExpression();
    op = tokMod;
    EmtOperation();
  }
  if (curTok == tokAnd)
  {
    RdTok();
    ParseExpression();
    op = tokAnd;
    EmtOperation();
  }
  if (curTok == tokOr)
  {
    RdTok();
    ParseExpression();
    op = tokOr;
    EmtOperation();
  }
  if (curTok == tokEqual)
  {
    RdTok();
    ParseExpression();
    // text = opCmp; strings[0] = opJne; PutTxt();
    text = opTeq; PutTxt();
  }
  if (curTok == tokNotEqual)
  {
    RdTok();
    ParseExpression();
    // text = opCmp; strings[0] = opJeq; PutTxt();
    text = opTne; PutTxt();
  }
  if (curTok == tokLess)
  {
    RdTok();
    ParseExpression();
    // text = opCmp; strings[0] = opJge; PutTxt();
    text = opTls; PutTxt();
  }
  if (curTok == tokLessOrEqual)
  {
    RdTok();
    ParseExpression();
    // text = opCmp; strings[0] = opJgr; PutTxt();
    text = opTle; PutTxt();
  }
  if (curTok == tokGreater)
  {
    RdTok();
    ParseExpression();
    // text = opCmp; strings[0] = opJle; PutTxt();
    text = opTgr; PutTxt();
  }
  if (curTok == tokGreaterOrEqual)
  {
    RdTok();
    ParseExpression();
    // text = opCmp; strings[0] = opJls; PutTxt();
    text = opTge; PutTxt();
  }
}

static void ParseBlock()
{
  if (curTok != tokLBrace)
  {
    Fail();
  }
  RdTok();
  while ((curTok == tokVar) || (curTok == tokFun) || (curTok == tokReturn) || (curTok == tokIf) || (curTok == tokWhile))
  {
    if (curTok == tokVar)
    {
      findPointer = namesPointer;
      EmtPushNam();
      RdTok();
      if (curTok == tokLBracket)
      {
        EmtRead();
        RdTok();
        ParseExpression();
        if (curTok != tokRBracket)
        {
          Fail();
        }
        EmtAddIdx();
        RdTok();
      }
      if (curTok != tokAssign)
      {
        Fail();
      }
      RdTok();
      ParseExpression();
      if (curTok != tokSemicolon)
      {
        Fail();
      }
      EmtAsgn();
      RdTok();
    }
    if (curTok == tokFun)
    {
      funIdx = i;
      RdTok();
      if (curTok != tokLParen)
      {
        Fail();
      }
      RdTok();
      if ((curTok == tokNot) || (curTok == tokSub) || (curTok == tokNumber) || (curTok == tokVar) || (curTok == tokFun) || (curTok == tokLParen))
      {
        ParseExpression();
      }
      if (curTok != tokRParen)
      {
        Fail();
      }
      EmtProcCall();
      RdTok();
      if (curTok != tokSemicolon)
      {
        Fail();
      }
      RdTok();
    }
    if (curTok == tokReturn)
    {
      RdTok();
      if (curTok != tokSemicolon)
      {
        ParseExpression();
        reg = 'a'; EmtPopReg();
      }
      if (curTok != tokSemicolon)
      {
        Fail();
      }
      EmtRet();
      RdTok();
    }
    if (curTok == tokIf)
    {
      RdTok();
      if (curTok != tokLParen)
      {
        Fail();
      }
      PushValue();
      RdTok();
      ParseExpression();
      if (curTok != tokRParen)
      {
        Fail();
      }
      EmtJmpEnd();
      RdTok();
      ParseBlock();
      EmtEnd();
      PopValue();
    }
    if (curTok == tokWhile)
    {
      RdTok();
      if (curTok != tokLParen)
      {
        Fail();
      }
      PushValue();
      EmtBegin();
      RdTok();
      ParseExpression();
      if (curTok != tokRParen)
      {
        Fail();
      }
      EmtJmpEnd();
      RdTok();
      ParseBlock();
      EmtJmpBegin();
      EmtEnd();
      PopValue();
    }
  }
  if (curTok != tokRBrace)
  {
    Fail();
  }
  RdTok();
}

static void ParseDefinition()
{
  if (curTok == tokHash)
  {
    RdTok();
    if (curTok != tokInclude)
    {
      Fail();
    }
    RdTok();
    if (curTok != tokLess)
    {
      Fail();
    }
    RdTok();
    if (curTok != tokName)
    {
      Fail();
    }
    RdTok();
    if (curTok != tokGreater)
    {
      Fail();
    }
    RdTok();
    return;
  }
  if (curTok == tokConst)
  {
    RdTok();
    if (curTok != tokInt)
    {
      Fail();
    }
    RdTok();
    if (curTok != tokName)
    {
      Fail();
    }
    curSym = symConst;
    SetSym();
    RdTok();
    if (curTok != tokAssign)
    {
      Fail();
    }
    RdTok();
    ParseNumber();
    info[symbolPointer + 3] = curVal;
    if (curTok != tokSemicolon)
    {
      Fail();
    }
    RdTok();
    return;
  }
  if ((curTok == tokStatic) || (curTok == tokVoid) || (curTok == tokInt))
  {
    RdTok();
    if ((curTok == tokVoid) || (curTok == tokInt))
    {
      RdTok();
      if (curTok == tokMul)
      {
        RdTok();
      }
    }
    if (curTok != tokName)
    {
      Fail();
    }
    findPointer = namesPointer;
    SetSym();
    RdTok();
    if (curTok == tokLParen)
    {
      info[symbolPointer + 2] = symFun;
      EmtTxtSeg();
      EmtFun();
      RdTok();
      if (curTok != tokRParen)
      {
        Fail();
      }
      RdTok();
      ParseBlock();
      EmtRet();
      return;
    }
    info[symbolPointer + 2] = symVar;
    EmtDatSeg();
    EmtDcl();
    curVal = 1; // default value
    if (curTok == tokLBracket)
    {
      curVal = 0;
      EmtArrAddr();
      RdTok();
      if (curTok == tokNumber)
      {
        RdTok();
      }
      if (curTok == tokRBracket)
      {
        RdTok();
      }
    }
    hasInit = 0;
    if (curTok == tokAssign)
    {
      hasInit = 1;
      RdTok();
      if (curTok != tokString)
      {
        Fail();
      }
      EmtStrInt();
      RdTok();
    }
    if (curTok != tokSemicolon)
    {
      Fail();
    }
    if (hasInit == 0)
    {
      EmtDup(); 
    }
    putchar(10);
    RdTok();
  }
}

static void ParseCpl()
{
  while ((curTok == tokHash) || (curTok == tokConst) || (curTok == tokStatic) || (curTok == tokVoid) || (curTok == tokInt))
  {
    ParseDefinition();
  }
  if (curTok != tokEof)
  {
    Fail();
  }
}

int main()
{
  Init();
  ParseCpl();
}
