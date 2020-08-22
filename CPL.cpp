#include <cstdio>
#include <cstdlib>

const int Ampersand = 38;
const int Apostrophe = 39;
const int Asterisk = 42;
const int AtSign = 64;
const int Backslash = 92;
const int Constant = 1;
const int Delete = 127;
const int Dollar = 36;
const int Eof = 0;
const int Equal = 2;
const int EqualsSign = 61;
const int ExclamationMark = 33;
const int Function = 3;
const int Greater = 62;
const int GreaterOrEqual = 4;
const int Hash = 35;
const int If = 5;
const int Include = 6;
const int InfoSize = 5000;
const int Int = 7;
const int Keyword = 20;
const int LeftBrace = 123;
const int LeftBracket = 91;
const int LeftParenthesis = 40;
const int Less = 60;
const int LessOrEqual = 8;
const int LowerA = 97;
const int LowerZ = 122;
const int Minus = 45;
const int Name = 9;
const int Nine = 57;
const int NotEqual = 10;
const int Number = 11;
const int Percent = 37;
const int Pipe = 124;
const int Plus = 43;
const int Quote = 34;
const int Return = 12;
const int RightBrace = 125;
const int RightBracket = 93;
const int RightParenthesis = 41;
const int Semicolon = 59;
const int Slash = 47;
const int Space = 32;
const int Static = 13;
const int String = 14;
const int Unknown = 21;
const int UpperA = 65;
const int UpperZ = 90;
const int ValuesSize = 1000;
const int Variable = 15;
const int Void = 16;
const int While = 17;
const int Zero = 48;

static char curTxtChr;
static char datSeg[] = "section '.data' data readable writeable";
static char hdr[] = "format PE Console\nentry start\nsection '.idata' import data readable writeable\ndd 0\ndd 0\ndd 0\ndd rva kernelName\ndd rva kernelTable\ndd 0\ndd 0\ndd 0\ndd 0\ndd 0\nkernelTable:\nExitProcess dd rva exitProcess\nReadFile dd rva readFile\nWriteFile dd rva writeFile\nGetStdHandle dd rva getStdHandle\ndd 0\nkernelName:\ndb 'KERNEL32.DLL',0\nexitProcess:\ndw 0\ndb 'ExitProcess',0\nreadFile:\ndw 0\ndb 'ReadFile',0\nwriteFile:\ndw 0\ndb 'WriteFile',0\ngetStdHandle:\ndw 0\ndb 'GetStdHandle',0\nsection '.data' data readable writeable\nstdInHandle dd ?\nstdOutHandle dd ?\nioChr db ?\nbytesCount dd ?\nsection '.text' code readable executable\nexit:\ncall [ExitProcess]\ngetchar:\npush ebp\nmov ebp, esp\nmov [ioChr], 0\npush 0\npush bytesCount\npush 1\npush ioChr\npush [stdInHandle]\ncall [ReadFile]\nmov eax, 0\nmov al, [ioChr]\npop ebp\nret 0 \nputchar:\npush ebp\nmov ebp, esp\nmov eax, [ebp + 8]\nmov [ioChr], al\npush 0\npush bytesCount\npush 1\npush ioChr\npush [stdOutHandle]\ncall [WriteFile]\npop ebp\nret 4 \nteq:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njne @f\ninc ecx\n@@: push ecx\njmp edx\ntne:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\nje @f\ninc ecx\n@@: push ecx\njmp edx\ntls:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njge @f\ninc ecx\n@@: push ecx\njmp edx\ntle:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njg @f\ninc ecx\n@@: push ecx\njmp edx\ntgr:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njle @f\ninc ecx\n@@: push ecx\njmp edx\ntge:\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\njl @f\ninc ecx\n@@: push ecx\njmp edx\nadi:\npop edx\npop ebx\npop eax\nlea eax,[eax+4*ebx]\npush eax\njmp edx\nstart:\npush 0FFFFFFF6h\ncall [GetStdHandle]\nmov [stdInHandle], eax \npush 0FFFFFFF5h\ncall [GetStdHandle]\nmov [stdOutHandle], eax \ncall main\npush 0\ncall [ExitProcess]\n";
static char jmpBegin[] = "jmp b#0\n";
static char jmpEnd[] = "pop eax\ncmp eax,0\nje e#0\n";
static char opAdi[] = "call adi\n";
static char opArrAddr[] = " \\$+4,";
static char opCall[] = "call %0\n";
static char opDeclaration[] = "%0 dd";
static char opDup[] = " #0 dup(?)\n";
static char opFun[] = "%0:\n";
static char opNumbers[] = "&0";
static char opPushName[] = "push %0\n";
static char opRead[] = "pop eax\npush dword[eax]\n";
static char opTeq[] = "call teq\n";
static char opTge[] = "call tge\n";
static char opTgr[] = "call tgr\n";
static char opTle[] = "call tle\n";
static char opTls[] = "call tls\n";
static char opTne[] = "call tne\n";
static char opWrite[] = "pop ebx\npop eax\nmov [eax],ebx\n";
static char txtFail[] = "fail\n";
static char txtSeg[] = "section '.text' code readable executable";
static char* string;
static char* strings[10]; // texts for string placeholders
static char* text; // Test to be put out.
static int buffer[12];
static int bufferIndex;
static int chr;
static int curChr;
static int curSym;
static int curTok;
static int curVal;
static int findPointer; // index of the find in "info" by calling "FdSym"
static int funIdx;
static int hasInit;
static int i;
static int info[InfoSize]; // various information (e.g. names) necessary during compilation
static int j;
static int limit;
static int minusOne;
static int nameIndex;
static int nameIndices[10]; // indices for name placeholders
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
static int values[ValuesSize];
static int valuesPointer; // index of the top-most value in "values"

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

static void PutNumbers()
{
  i = 0;
  limit = info[nameIndex + 1];
  while (i < limit)
  {
    number = info[nameIndex + i + 2]; PutNumber();
    putchar(',');
    i = i + 1;
  }
  putchar('0');
}

static int ExpandPlaceholders()
{
  if (curTxtChr == Backslash)
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
  if (curTxtChr == Hash) // number ...
  {
    number = numbers[text[textIndex + 1] - 48];
    PutNumber();
    textIndex = textIndex + 2;
    return 1;
  }
  if (curTxtChr == Dollar) // zero-terminated string ...
  {
    string = strings[text[textIndex + 1] - 48];
    PutString();
    textIndex = textIndex + 2;
    return 1;
  }
  if (curTxtChr == Percent) // name ...
  {
    nameIndex = nameIndices[text[textIndex + 1] - 48];
    PutName();
    textIndex = textIndex + 2;
    return 1;
  }
  if (curTxtChr == Ampersand) // numbers ...
  {
    nameIndex = nameIndices[text[textIndex + 1] - 48];
    PutNumbers();
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

static void EmtVal()
{
  number = curVal;
  PutNumber();
}

static void EmtDcl()
{
  // EmtNam(); putchar(' '); putchar('d'); putchar('d'); putchar(' ');
  text = opDeclaration; nameIndices[0] = findPointer; PutTxt();
}

static void EmtDup()
{
  // EmtVal(); putchar(' '); putchar('d'); putchar('u'); putchar('p'); putchar('('); putchar('?'); putchar(')');
  text = opDup; numbers[0] = curVal; PutTxt();
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
  if (op == Plus)
  {
    putchar('a'); putchar('d'); putchar('d'); 
  }
  if (op == Minus)
  {
    putchar('s'); putchar('u'); putchar('b'); 
  }
  if (op == Asterisk)
  {
    putchar('m'); putchar('u'); putchar('l'); 
  }
  if (op == Slash)
  {
    putchar('d'); putchar('i'); putchar('v'); 
  }
  if (op == Percent)
  {
    putchar('d'); putchar('i'); putchar('v'); 
  }
  if (op == Ampersand)
  {
    putchar('a'); putchar('n'); putchar('d'); 
  }
  if (op == Pipe)
  {
    putchar('o'); putchar('r');
  }
}

static void EmtReg()
{
  putchar('e'); putchar(reg); putchar('x'); 
}

static void EmtPushName()
{
  text = opPushName; nameIndices[0] = findPointer; PutTxt();
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
  if ((op == Slash) | (op == Percent))
  {
    EmtMov(); putchar(' '); reg = 'd'; EmtReg(); putchar(','); curVal = 0; EmtVal(); putchar(10);
  }
  EmtOp(); putchar(' '); 
  if ((op == Plus) | (op == Minus) | (op == Ampersand) | (op == Pipe))
  {
    reg = 'a'; EmtReg(); putchar(',');
  }
  reg = 'b'; EmtReg(); putchar(10);
  if ((op == Plus) | (op == Minus) | (op == Asterisk) | (op == Slash) | (op == Ampersand) | (op == Pipe))
  {
    reg = 'a';
  }
  if (op == Percent)
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
  op = Minus; EmtOp(); putchar(' '); reg = 'a'; EmtReg(); putchar(','); reg = 'b'; EmtReg(); putchar(10);
  reg = 'a'; EmtPushReg();
}

static void EmtAddIdx()
{
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
  text = opNumbers; nameIndices[0] = namesPointer; PutTxt(); 
}

static void EmtArrAddr()
{
  // putchar('$'); putchar('+'); putchar('4'); putchar(',');
  text = opArrAddr; PutTxt();
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
  return (curChr < Space) | (Delete <= curChr);
}

static int IsWhitespace()
{
  return IsCtrl() | (curChr == ' ');
}

static int IsDigit()
{
  return (Zero <= curChr) & (curChr <= Nine);
}

static int IsLetter()
{
  return ((LowerA <= curChr) & (curChr <= LowerZ)) | ((UpperA <= curChr) & (curChr <= UpperZ));
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
  while ((curChr == '/') & (nxtChr == '/'))
  {
    while ((curChr != Eof) & (curChr != 10))
    {
      GetNxtChr();
    }
    GetNxtChr();
  }
}

static void GetToken()
{
  info[namesPointer + 1] = 0;
  while ((curChr != Eof) & IsWhitespace())
  {
    GetChr();
  }
  curTok = Eof;
  if (curChr == Eof)
  {
    return;
  }
  if (curChr == LeftBrace)
  {
    GetChr();
    curTok = LeftBrace;
    return;
  }
  if (curChr == LeftBracket)
  {
    GetChr();
    curTok = LeftBracket;
    return;
  }
  if (curChr == LeftParenthesis)
  {
    GetChr();
    curTok = LeftParenthesis;
    return;
  }
  if (curChr == RightBrace)
  {
    GetChr();
    curTok = RightBrace;
    return;
  }
  if (curChr == RightBracket)
  {
    GetChr();
    curTok = RightBracket;
    return;
  }
  if (curChr == RightParenthesis)
  {
    GetChr();
    curTok = RightParenthesis;
    return;
  }
  if (curChr == Semicolon)
  {
    GetChr();
    curTok = Semicolon;
    return;
  }
  if (curChr == Plus)
  {
    GetChr();
    curTok = Plus;
    return;
  }
  if (curChr == Minus)
  {
    GetChr();
    curTok = Minus;
    return;
  }
  if (curChr == Asterisk)
  {
    GetChr();
    curTok = Asterisk;
    return;
  }
  if (curChr == Slash)
  {
    GetChr();
    curTok = Slash;
    return;
  }
  if (curChr == Percent)
  {
    GetChr();
    curTok = Percent;
    return;
  }
  if (curChr == EqualsSign)
  {
    GetChr();
    if (curChr == EqualsSign)
    {
      GetChr();
      curTok = Equal;
      return;
    }
    curTok = EqualsSign;
    return;
  }
  if (curChr == ExclamationMark)
  {
    GetChr();
    if (curChr == EqualsSign)
    {
      GetChr();
      curTok = NotEqual;
      return;
    }
    curTok = ExclamationMark;
    return;
  }
  if (curChr == Less)
  {
    GetChr();
    if (curChr == EqualsSign)
    {
      GetChr();
      curTok = LessOrEqual;
      return;
    }
    curTok = Less;
    return;
  }
  if (curChr == Greater)
  {
    GetChr();
    if (curChr == EqualsSign)
    {
      GetChr();
      curTok = GreaterOrEqual;
      return;
    }
    curTok = Greater;
    return;
  }
  if (curChr == Ampersand)
  {
    GetChr();
    curTok = Ampersand;
    return;
  }
  if (curChr == Pipe)
  {
    GetChr();
    curTok = Pipe;
    return;
  }
  if (curChr == Hash)
  {
    GetChr();
    curTok = Hash;
    return;
  }
  if (curChr == Apostrophe)
  {
    GetChr();
    if (!IsCtrl())
    {
      curVal = curChr;
      GetChr();
      if (curChr == Apostrophe)
      {
        GetChr();
        curTok = Number;
        return;
      }
    }
  }
  if (curChr == Quote)
  {
    GetChr();
    while ((curChr != Eof) & (curChr != Quote))
    {
      if (curChr == Backslash)
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
    if (curChr == Quote)
    {
      GetChr();
      curTok = String;
      return;
    }
  }
  if (IsLetter())
  {
    while ((curChr != Eof) & IsLetter())
    {
      AddCurChr();
      GetChr();
    }
    if (FndSym())
    {
      if (curSym == Keyword)
      {
        curTok = curVal;
        return;
      }
      if (curSym == Constant)
      {
        curTok = Number;
        return;
      }
      if (curSym == Function)
      {
        curTok = Function;
        return;
      }
      if (curSym == Variable)
      {
        curTok = Variable;
        return;
      }
      return;
    }
    curTok = Name;
    return;
  }
  if (IsDigit())
  {
    curVal = 0;
    while ((curChr != Eof) & IsDigit())
    {
      curVal = (curVal * 10) + (curChr - Zero);
      AddCurChr();
      GetChr();
    }
    curTok = Number;
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
  curSym = Keyword;
  curVal = Constant;
  SetSym();

  curChr = 's'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'a'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'i'; AddCurChr();
  curChr = 'c'; AddCurChr();
  curSym = Keyword;
  curVal = Static;
  SetSym();

  curChr = 'r'; AddCurChr();
  curChr = 'e'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'u'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curChr = 'n'; AddCurChr();
  curSym = Keyword;
  curVal = Return;
  SetSym();

  curChr = 'i'; AddCurChr();
  curChr = 'f'; AddCurChr();
  curSym = Keyword;
  curVal = If;
  SetSym();

  curChr = 'w'; AddCurChr();
  curChr = 'h'; AddCurChr();
  curChr = 'i'; AddCurChr();
  curChr = 'l'; AddCurChr();
  curChr = 'e'; AddCurChr();
  curSym = Keyword;
  curVal = While;
  SetSym();

  curChr = 'i'; AddCurChr();
  curChr = 'n'; AddCurChr();
  curChr = 'c'; AddCurChr();
  curChr = 'l'; AddCurChr();
  curChr = 'u'; AddCurChr();
  curChr = 'd'; AddCurChr();
  curChr = 'e'; AddCurChr();
  curSym = Keyword;
  curVal = Include;
  SetSym();

  curChr = 'i'; AddCurChr();
  curChr = 'n'; AddCurChr();
  curChr = 't'; AddCurChr();
  curSym = Keyword;
  curVal = Int;
  SetSym();

  curChr = 'c'; AddCurChr();
  curChr = 'h'; AddCurChr();
  curChr = 'a'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curSym = Keyword;
  curVal = Int;
  SetSym();

  curChr = 'v'; AddCurChr();
  curChr = 'o'; AddCurChr();
  curChr = 'i'; AddCurChr();
  curChr = 'd'; AddCurChr();
  curSym = Keyword;
  curVal = Void;
  SetSym();

  curChr = 'a'; AddCurChr();
  curChr = 'b'; AddCurChr();
  curChr = 'o'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curChr = 't'; AddCurChr();
  curSym = Function;
  curVal = Void;
  SetSym();

  curChr = 'g'; AddCurChr();
  curChr = 'e'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'c'; AddCurChr();
  curChr = 'h'; AddCurChr();
  curChr = 'a'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curSym = Function;
  curVal = Int;
  SetSym();

  curChr = 'p'; AddCurChr();
  curChr = 'u'; AddCurChr();
  curChr = 't'; AddCurChr();
  curChr = 'c'; AddCurChr();
  curChr = 'h'; AddCurChr();
  curChr = 'a'; AddCurChr();
  curChr = 'r'; AddCurChr();
  curSym = Function;
  curVal = Void;
  SetSym();

  curChr = 'e'; AddCurChr();
  curChr = 'x'; AddCurChr();
  curChr = 'i'; AddCurChr();
  curChr = 't'; AddCurChr();
  curSym = Function;
  curVal = Void;
  SetSym();

  curSym = Unknown;
  curVal = 0;
  nxtChr = 0;
  GetNxtChr();
  GetChr();
  GetToken();
}

static void ParseNumber()
{
  if (curTok == Minus)
  {
    GetToken();
    if (curTok != Number)
    {
      Fail();
    }
    curVal = -curVal;
    GetToken();
    return;
  }
  if (curTok != Number)
  {
    Fail();
  }
  GetToken();
}

static void ParseExpression()
{
  if (curTok == ExclamationMark)
  {
    GetToken();
    ParseExpression();
    EmtNot();
  }
  if (curTok == Minus)
  {
    GetToken();
    ParseExpression();
    EmtNeg();
  }
  if (curTok == Number)
  {
    EmtPushVal();
    GetToken();
  }
  if (curTok == Variable)
  {
    findPointer = namesPointer;
    EmtPushName();
    GetToken();
    if (curTok == LeftBracket)
    {
      EmtRead();
      GetToken();
      ParseExpression();
      if (curTok != RightBracket)
      {
        Fail();
      }
      EmtAddIdx();
      GetToken();
    }
    EmtRead();
  }
  if (curTok == Function)
  {
    funIdx = i;
    GetToken();
    if (curTok != LeftParenthesis)
    {
      Fail();
    }
    GetToken();
    if (curTok != RightParenthesis)
    {
      Fail();
    }
    EmtFunCall();
    GetToken();
  }
  if (curTok == LeftParenthesis)
  {
    GetToken();
    ParseExpression();
    if (curTok != RightParenthesis)
    {
      Fail();
    }
    GetToken();
  }
  if (curTok == Plus)
  {
    GetToken();
    ParseExpression();
    op = Plus;
    EmtOperation();
  }
  if (curTok == Minus)
  {
    GetToken();
    ParseExpression();
    op = Minus;
    EmtOperation();
  }
  if (curTok == Asterisk)
  {
    GetToken();
    ParseExpression();
    op = Asterisk;
    EmtOperation();
  }
  if (curTok == Slash)
  {
    GetToken();
    ParseExpression();
    op = Slash;
    EmtOperation();
  }
  if (curTok == Percent)
  {
    GetToken();
    ParseExpression();
    op = Percent;
    EmtOperation();
  }
  if (curTok == Ampersand)
  {
    GetToken();
    ParseExpression();
    op = Ampersand;
    EmtOperation();
  }
  if (curTok == Pipe)
  {
    GetToken();
    ParseExpression();
    op = Pipe;
    EmtOperation();
  }
  if (curTok == Equal)
  {
    GetToken();
    ParseExpression();
    // text = opCmp; strings[0] = opJne; PutTxt();
    text = opTeq; PutTxt();
  }
  if (curTok == NotEqual)
  {
    GetToken();
    ParseExpression();
    // text = opCmp; strings[0] = opJeq; PutTxt();
    text = opTne; PutTxt();
  }
  if (curTok == Less)
  {
    GetToken();
    ParseExpression();
    // text = opCmp; strings[0] = opJge; PutTxt();
    text = opTls; PutTxt();
  }
  if (curTok == LessOrEqual)
  {
    GetToken();
    ParseExpression();
    // text = opCmp; strings[0] = opJgr; PutTxt();
    text = opTle; PutTxt();
  }
  if (curTok == Greater)
  {
    GetToken();
    ParseExpression();
    // text = opCmp; strings[0] = opJle; PutTxt();
    text = opTgr; PutTxt();
  }
  if (curTok == GreaterOrEqual)
  {
    GetToken();
    ParseExpression();
    // text = opCmp; strings[0] = opJls; PutTxt();
    text = opTge; PutTxt();
  }
}

static void ParseBlock()
{
  if (curTok != LeftBrace)
  {
    Fail();
  }
  GetToken();
  while ((curTok == Variable) | (curTok == Function) | (curTok == Return) | (curTok == If) | (curTok == While))
  {
    if (curTok == Variable)
    {
      findPointer = namesPointer;
      EmtPushName();
      GetToken();
      if (curTok == LeftBracket)
      {
        EmtRead();
        GetToken();
        ParseExpression();
        if (curTok != RightBracket)
        {
          Fail();
        }
        EmtAddIdx();
        GetToken();
      }
      if (curTok != EqualsSign)
      {
        Fail();
      }
      GetToken();
      ParseExpression();
      if (curTok != Semicolon)
      {
        Fail();
      }
      EmtAsgn();
      GetToken();
    }
    if (curTok == Function)
    {
      funIdx = i;
      GetToken();
      if (curTok != LeftParenthesis)
      {
        Fail();
      }
      GetToken();
      if ((curTok == ExclamationMark) | (curTok == Minus) | (curTok == Number) | (curTok == Variable) | (curTok == Function) | (curTok == LeftParenthesis))
      {
        ParseExpression();
      }
      if (curTok != RightParenthesis)
      {
        Fail();
      }
      EmtProcCall();
      GetToken();
      if (curTok != Semicolon)
      {
        Fail();
      }
      GetToken();
    }
    if (curTok == Return)
    {
      GetToken();
      if (curTok != Semicolon)
      {
        ParseExpression();
        reg = 'a'; EmtPopReg();
      }
      if (curTok != Semicolon)
      {
        Fail();
      }
      EmtRet();
      GetToken();
    }
    if (curTok == If)
    {
      GetToken();
      if (curTok != LeftParenthesis)
      {
        Fail();
      }
      PushValue();
      GetToken();
      ParseExpression();
      if (curTok != RightParenthesis)
      {
        Fail();
      }
      EmtJmpEnd();
      GetToken();
      ParseBlock();
      EmtEnd();
      PopValue();
    }
    if (curTok == While)
    {
      GetToken();
      if (curTok != LeftParenthesis)
      {
        Fail();
      }
      PushValue();
      EmtBegin();
      GetToken();
      ParseExpression();
      if (curTok != RightParenthesis)
      {
        Fail();
      }
      EmtJmpEnd();
      GetToken();
      ParseBlock();
      EmtJmpBegin();
      EmtEnd();
      PopValue();
    }
  }
  if (curTok != RightBrace)
  {
    Fail();
  }
  GetToken();
}

static void ParseDefinition()
{
  if (curTok == Hash)
  {
    GetToken();
    if (curTok != Include)
    {
      Fail();
    }
    GetToken();
    if (curTok != Less)
    {
      Fail();
    }
    GetToken();
    if (curTok != Name)
    {
      Fail();
    }
    GetToken();
    if (curTok != Greater)
    {
      Fail();
    }
    GetToken();
    return;
  }
  if (curTok == Constant)
  {
    GetToken();
    if (curTok != Int)
    {
      Fail();
    }
    GetToken();
    if (curTok != Name)
    {
      Fail();
    }
    curSym = Constant;
    SetSym();
    GetToken();
    if (curTok != EqualsSign)
    {
      Fail();
    }
    GetToken();
    ParseNumber();
    info[symbolPointer + 3] = curVal;
    if (curTok != Semicolon)
    {
      Fail();
    }
    GetToken();
    return;
  }
  if ((curTok == Static) | (curTok == Void) | (curTok == Int))
  {
    GetToken();
    if ((curTok == Void) | (curTok == Int))
    {
      GetToken();
      if (curTok == Asterisk)
      {
        GetToken();
      }
    }
    if (curTok != Name)
    {
      Fail();
    }
    findPointer = namesPointer;
    SetSym();
    GetToken();
    if (curTok == LeftParenthesis)
    {
      info[symbolPointer + 2] = Function;
      EmtTxtSeg();
      EmtFun();
      GetToken();
      if (curTok != RightParenthesis)
      {
        Fail();
      }
      GetToken();
      ParseBlock();
      EmtRet();
      return;
    }
    info[symbolPointer + 2] = Variable;
    EmtDatSeg();
    EmtDcl();
    curVal = 1; // default value
    if (curTok == LeftBracket)
    {
      curVal = 0;
      EmtArrAddr();
      GetToken();
      if (curTok == Number)
      {
        GetToken();
      }
      if (curTok == RightBracket)
      {
        GetToken();
      }
    }
    hasInit = 0;
    if (curTok == EqualsSign)
    {
      hasInit = 1;
      GetToken();
      if (curTok != String)
      {
        Fail();
      }
      EmtStrInt();
      GetToken();
    }
    if (curTok != Semicolon)
    {
      Fail();
    }
    if (hasInit == 0)
    {
      EmtDup(); 
    }
    putchar(10);
    GetToken();
  }
}

static void ParseCpl()
{
  while ((curTok == Hash) | (curTok == Constant) | (curTok == Static) | (curTok == Void) | (curTok == Int))
  {
    ParseDefinition();
  }
  if (curTok != Eof)
  {
    Fail();
  }
}

int main()
{
  Init();
  ParseCpl();
}

