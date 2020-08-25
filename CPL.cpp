#include <cstdio>
#include <cstdlib>

const int Add = 10;
const int Ampersand = 38;
const int And = 15;
const int Apostrophe = 39;
const int Asterisk = 42;
const int Backslash = 92;
const int Constant = 1;
const int Delete = 127;
const int Div = 13;
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
const int ItemsSize = 5000;
const int Int = 7;
const int Keyword = 20;
const int LeftBrace = 123;
const int LeftBracket = 91;
const int LeftParenthesis = 40;
const int Less = 60;
const int LessOrEqual = 8;
const int LineFeed = 10;
const int LowerA = 97;
const int LowerN = 110;
const int LowerZ = 122;
const int Minus = 45;
const int Mod = 14;
const int Mul = 12;
const int Name = 9;
const int Nine = 57;
const int NotEqual = 22;
const int Number = 11;
const int Or = 16;
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
const int Sub = 11;
const int Teq = 17;
const int Tge = 22;
const int Tgr = 21;
const int Tle = 20;
const int Tls = 19;
const int Tne = 18;
const int Unknown = 21;
const int UpperA = 65;
const int UpperZ = 90;
const int ValuesSize = 1000;
const int Variable = 15;
const int Void = 16;
const int While = 17;
const int Zero = 48;

static char addIndex[] = "call adi\n";
static char arrayAddress[] = " \\$+4,";
static char beginLabel[] = "b#:\n";
static char callOperation[] = "call o#\n";
static char callSubroutine[] = "call %\n";
static char codes[] = "&";
static char curTxtChr;
static char dataSegment[] = "section '.data' data readable writeable";
static char declaration[] = "% dd";
static char duplicate[] = " # dup(?)\n";
static char endLabel[] = "e#:\n";
static char function[] = "%:\n";
static char header[] = "format PE Console\nentry start\nmacro opr k,o\n{\npop ecx\npop ebx\npop eax\nif o eq\nk eax,ebx\npush eax\nelse\nxor edx,edx\nk ebx\npush o\nend if\njmp ecx\n}\nmacro tst k\n{\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\nk @f\ndec ecx\n@@: push ecx\njmp edx\n}\nsection '.idata' import data readable writeable\ndd 0,0,0\ndd rva kernelName\ndd rva kernelTable\ndd 0,0,0,0,0\nkernelTable:\nExitProcess dd rva exitProcess\nReadFile dd rva readFile\nWriteFile dd rva writeFile\nGetStdHandle dd rva getStdHandle\ndd 0\nkernelName:db 'KERNEL32.DLL',0\nexitProcess:db 0,0,'ExitProcess',0\nreadFile:db 0,0,'ReadFile',0\nwriteFile:db 0,0,'WriteFile',0\ngetStdHandle:db 0,0,'GetStdHandle',0\nsection '.data' data readable writeable\nih dd ?\noh dd ?\nio db ?\nbc dd ?\nsection '.text' code readable executable\nexit:\ncall [ExitProcess]\ngetchar:\npush ebp\nmov ebp, esp\nmov [io], 0\npush 0\npush bc\npush 1\npush io\npush [ih]\ncall [ReadFile]\nmov eax, 0\nmov al, [io]\npop ebp\nret 0 \nputchar:\npush ebp\nmov ebp, esp\nmov eax, [ebp + 8]\nmov [io], al\npush 0\npush bc\npush 1\npush io\npush [oh]\ncall [WriteFile]\npop ebp\nret 4 \no10:opr add\no11:opr sub\no12:opr mul,eax\no13:opr div,eax\no14:opr div,edx\no15:opr and\no16:opr or\no17:tst jne\no18:tst je\no19:tst jge\no20:tst jg\no21:tst jle\no22:tst jl\nadi:\npop edx\npop ebx\npop eax\nlea eax,[eax+4*ebx]\npush eax\njmp edx\nlgn:\npop edx\npop eax\ntest eax,eax\nsetz al\nmovzx eax,al\npush eax\njmp edx\nstart:\npush 0FFFFFFF6h\ncall [GetStdHandle]\nmov [ih], eax \npush 0FFFFFFF5h\ncall [GetStdHandle]\nmov [oh], eax \ncall main\npush 0\ncall [ExitProcess]\n";
static char jumpToBegin[] = "jmp b#\n";
static char jumpToEnd[] = "pop eax\ncmp eax,0\nje e#\n";
static char logicalNegation[] = "call lgn\n";
static char numericalNegation[] = "neg dword[esp]\n";
static char popEax[] = "pop eax\n";
static char pushEax[] = "push eax\n";
static char pushName[] = "push %\n";
static char pushValue[] = "push #\n";
static char read[] = "pop eax\npush dword[eax]\n";
static char returnFromSubroutine[] = "ret\n";
static char fail[] = "fail\n";
static char textSegment[] = "section '.text' code readable executable";
static char write[] = "pop ebx\npop eax\nmov [eax],ebx\n";
static char* text; // text to be put out.
static int buffer[12];
static int bufferIndex;
static int curChr;
static int curSym;
static int curTok;
static int curVal;
static int findPointer; // index of the find in "items" by calling "FdSym"
static int funIdx;
static int hasInit;
static int i;
static int items[ItemsSize]; // various information (e.g. names) necessary during compilation
static int j;
static int limit;
static int minusOne;
static int nameIndex;
static int namesPointer; // index of the top-most name in "items"
static int nextLabel;
static int number;
static int nxtChr;
static int operation;
static int symbolPointer; // index of the newly defined symbol in "items".
static int textIndex;
static int value;
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

static void PutName()
{
  i = 0;
  limit = items[nameIndex + 1];
  while (i < limit)
  {
    putchar(items[nameIndex + i + 2]);
    i = i + 1;
  }
}

static void PutNumbers()
{
  i = 0;
  limit = items[nameIndex + 1];
  while (i < limit)
  {
    number = items[nameIndex + i + 2]; PutNumber();
    putchar(',');
    i = i + 1;
  }
  putchar('0');
}

static void ExpandPlaceholders()
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
    return;
  }
  if (curTxtChr == Hash) // number ...
  {
    PutNumber();
    return;
  }
  if (curTxtChr == Percent) // name ...
  {
    PutName();
    return;
  }
  if (curTxtChr == Ampersand) // numbers ...
  {
    PutNumbers();
    return;
  }
  putchar(curTxtChr);
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
    ExpandPlaceholders();
    textIndex = textIndex + 1;
  }
}

// exit program with failure-code 1
static void Fail()
{
  text = fail; PutTxt();
  exit(1);
}

static void PushValue()
{
  valuesPointer = valuesPointer + 1;
  values[valuesPointer] = value;
}

static void PushLabel()
{
  value = nextLabel;
  PushValue();
  nextLabel = nextLabel + 1;
}

static void PopValue()
{
  valuesPointer = valuesPointer - 1;
}

static int TopValue()
{
  return values[valuesPointer];
}

static void EmtDcl()
{
  text = declaration; nameIndex = findPointer; PutTxt();
}

static void EmtDup()
{
  text = duplicate; number = curVal; PutTxt();
}

static void EmtFun()
{
  text = function; nameIndex = findPointer; PutTxt();
}

static void EmtDatSeg()
{
  text = dataSegment; PutTxt(); putchar(10);
}

static void EmtTxtSeg()
{
  text = textSegment; PutTxt(); putchar(10);
}

static void EmtHdr()
{
  text = header; PutTxt();
}

static void EmtPushFind()
{
  text = pushName; nameIndex = findPointer; PutTxt();
}

static void EmtPushVal()
{
  text = pushValue; number = curVal; PutTxt();
}

static void EmtRet()
{
  text = returnFromSubroutine; PutTxt();
}

static void EmtAsgn()
{
  text = write; PutTxt();
}

static void EmtProcCall()
{
  text = callSubroutine; nameIndex = funIdx; PutTxt();
}

static void EmtFunCall()
{
  EmtProcCall();
  text = pushEax; PutTxt();
}

static void EmtNot()
{
  text = logicalNegation; PutTxt();
}

static void EmtNeg()
{
  text = numericalNegation; PutTxt();
}

static void EmtAddIdx()
{
  text = addIndex; PutTxt();
}

static void EmtBegin()
{
  text = beginLabel; number = TopValue(); PutTxt();
}

static void EmtJmpBegin()
{
  text = jumpToBegin; number = TopValue(); PutTxt();
}

static void EmtEnd()
{
  text = endLabel; number = TopValue(); PutTxt();
}

static void EmtJmpEnd()
{
  text = jumpToEnd; number = TopValue(); PutTxt();
}

static void EmtRead()
{
  text = read; PutTxt();
}

static void EmtStrInt()
{
  text = codes; nameIndex = namesPointer; PutTxt(); 
}

static void EmtArrAddr()
{
  text = arrayAddress; PutTxt();
}

static void AddCurChr()
{
  items[namesPointer + 1] = items[namesPointer + 1] + 1;
  items[namesPointer + items[namesPointer + 1] + 1] = curChr;
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
  symbolPointer = namesPointer + items[namesPointer + 1];
  items[symbolPointer + 2] = curSym;
  items[symbolPointer + 3] = curVal;
  items[symbolPointer + 4] = namesPointer;
  namesPointer = symbolPointer + 4;
}

static int FndSym()
{
  i = items[namesPointer];
  while (1)
  {
    limit = items[i + 1] + 2;
    j = 1;
    while (j < limit)
    {
      if (items[i + j] != items[namesPointer + j])
      {
        j = limit;
      }
      j = j + 1;
    }
    if (j == limit)
    {
      curSym = items[i + limit]; curVal = items[i + limit + 1];
      return 1;
    }
    if (i == minusOne)
    {
      return 0;
    }
    i = items[i];
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
  items[namesPointer + 1] = 0;
  while ((curChr != Eof) & IsWhitespace())
  {
    GetChr();
  }
  curTok = curChr;
  if (curChr == EqualsSign)
  {
    GetChr();
    if (curChr == EqualsSign)
    {
      GetChr();
      curTok = Equal;
      return;
    }
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
    return;
  }
  if (curChr == Apostrophe)
  {
    GetChr();
    curVal = curChr;
    GetChr();
    if (curChr == Apostrophe)
    {
      GetChr();
      curTok = Number;
      return;
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
        if (curChr == LowerN)
        {
          curChr = LineFeed;
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
  GetChr();
}

static void Init()
{
  minusOne = -1;

  EmtHdr();

  nextLabel = 0;
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
    EmtPushFind();
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
      EmtRead();
    }
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
  if (curTok == EqualsSign)
  {
    GetToken();
    ParseExpression();
    EmtAsgn();
    return;
  }
  operation = 0;
  if (curTok == Plus)
  {
    operation = Add;
  }
  if (curTok == Minus)
  {
    operation = Sub;
  }
  if (curTok == Asterisk)
  {
    operation = Mul;
  }
  if (curTok == Slash)
  {
    operation = Div;
  }
  if (curTok == Percent)
  {
    operation = Mod;
  }
  if (curTok == Ampersand)
  {
    operation = And;
  }
  if (curTok == Pipe)
  {
    operation = Or;
  }
  if (curTok == Equal)
  {
    operation = Teq;
  }
  if (curTok == NotEqual)
  {
    operation = Tne;
  }
  if (curTok == Less)
  {
    operation = Tls;
  }
  if (curTok == LessOrEqual)
  {
    operation = Tle;
  }
  if (curTok == Greater)
  {
    operation = Tgr;
  }
  if (curTok == GreaterOrEqual)
  {
    operation = Tge;
  }
  if (operation)
  {
    GetToken();
    value = operation;
    PushValue();
    ParseExpression();
    operation = TopValue();
    PopValue();
    text = callOperation; number = operation; PutTxt();
  }
}

static void ParseBlock()
{
  if (curTok != LeftBrace)
  {
    Fail();
  }
  GetToken();
  while (curTok != RightBrace)
  {
    if (curTok == Variable)
    {
      findPointer = namesPointer;
      EmtPushFind();
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
      if (curTok != RightParenthesis)
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
        text = popEax; PutTxt();
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
      PushLabel();
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
      PushLabel();
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
    items[symbolPointer + 3] = curVal;
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
      items[symbolPointer + 2] = Function;
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
    items[symbolPointer + 2] = Variable;
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
  while (curTok != Eof)
  {
    ParseDefinition();
  }
}

int main()
{
  Init();
  ParseCpl();
}

