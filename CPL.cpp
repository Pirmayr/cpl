#include <cstdio>
#include <cstdlib>

const int Add = 10;
const int Ampersand = 38;
const int And = 15;
const int Apostrophe = 39;
const int Assign = 23;
const int Asterisk = 42;
const int Backslash = 92;
const int Char = 25;
const int CharacterVariable = 24;
const int Constant = 1;
const int DataSegment = 1;
const int Delete = 127;
const int Div = 13;
const int Else = 26;
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
const int Int = 7;
const int infosSize = 10000;
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
const int NoSegment = 0;
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
const int TextSegment = 2;
const int Tge = 22;
const int Tgr = 21;
const int Tle = 20;
const int Tls = 19;
const int Tne = 18;
const int Unknown = 21;
const int UpperA = 65;
const int UpperZ = 90;
const int Variable = 15;
const int Void = 16;
const int While = 17;
const int Zero = 48;

static char addCharacterIndex[] = "call o10\n";
static char addIntegerIndex[] = "call adi\n";
static char beginLabel[] = "begin#:\n";
static char elseLabel[] = "else#:\n";
static char callOperation[] = "call o#\n";
static char callSubroutine[] = "call %\npush eax\n";
static char characterArrayAddress[] = " \\$+4\ndb";
static char codes[] = " &";
static char curTxtChr;
static char dataSegment[] = "section '.data' data readable writeable\n";
static char declaration[] = "% dd";
static char duplicates[] = " # dup(?)";
static char endLabel[] = "end#:\n";
static char fail[] = "fail\n";
static char function[] = "%:\n";
static char header[] = "format PE Console\nentry start\nmacro opr k,o\n{\npop ecx\npop ebx\npop eax\nif o eq\nk eax,ebx\npush eax\nelse\nxor edx,edx\nk ebx\npush o\nend if\njmp ecx\n}\nmacro tst k\n{\npop edx\nxor ecx,ecx\npop ebx\npop eax\ncmp eax,ebx\nk @f\ndec ecx\n@@: push ecx\njmp edx\n}\nsection '.idata' import data readable writeable\ndd 0,0,0\ndd rva kernelName\ndd rva kernelTable\ndd 0,0,0,0,0\nkernelTable:\nExitProcess dd rva exitProcess\nReadFile dd rva readFile\nWriteFile dd rva writeFile\nGetStdHandle dd rva getStdHandle\ndd 0\nkernelName:db 'KERNEL32.DLL',0\nexitProcess:db 0,0,'ExitProcess',0\nreadFile:db 0,0,'ReadFile',0\nwriteFile:db 0,0,'WriteFile',0\ngetStdHandle:db 0,0,'GetStdHandle',0\nsection '.data' data readable writeable\nih dd ?\noh dd ?\nio db ?\nbc dd ?\nsection '.text' code readable executable\nexit:\ncall [ExitProcess]\ngetchar:\npush ebp\nmov ebp, esp\nmov [io], 0\npush 0\npush bc\npush 1\npush io\npush [ih]\ncall [ReadFile]\nmov eax, 0\nmov al, [io]\npop ebp\nret 0 \nputchar:\npush ebp\nmov ebp, esp\nmov eax, [ebp + 8]\nmov [io], al\npush 0\npush bc\npush 1\npush io\npush [oh]\ncall [WriteFile]\npop ebp\nret 4 \no10:opr add\no11:opr sub\no12:opr mul,eax\no13:opr div,eax\no14:opr div,edx\no15:opr and\no16:opr or\no17:tst jne\no18:tst je\no19:tst jge\no20:tst jg\no21:tst jle\no22:tst jl\no23:\npop ecx\npop ebx\npop eax\nmov [eax],ebx\npush ebx\njmp ecx\nadi:\npop edx\npop ebx\npop eax\nlea eax,[eax+4*ebx]\npush eax\njmp edx\nlgn:\npop edx\npop eax\ntest eax,eax\nsetz al\nmovzx eax,al\npush eax\njmp edx\nstart:\npush 0FFFFFFF6h\ncall [GetStdHandle]\nmov [ih], eax \npush 0FFFFFFF5h\ncall [GetStdHandle]\nmov [oh], eax \ncall main\npush 0\ncall [ExitProcess]\n";
static char integerArrayAddress[] = " \\$+4\ndd";
static char jumpToBegin[] = "jmp begin#\n";
static char jumpToEnd[] = "jmp end#\n";
static char jumpToElse[] = "pop eax\ncmp eax,0\nje else#\n";
static char logicalNegation[] = "call lgn\n";
static char numericalNegation[] = "neg dword[esp]\n";
static char popEax[] = "pop eax\n";
static char pushName[] = "push %\n";
static char pushValue[] = "push #\n";
static char read[] = "pop eax\npush dword[eax]\n";
static char returnFromSubroutine[] = "ret\n";
static char textSegment[] = "section '.text' code readable executable\n";
static char* text; // text to be put out.
static int activeSegment;
static int character; // the current character processed in the scanner
static int kind; // symbol kind set in "DefineSymbol" or "FindSymbol"
static int token; // the token currently read by the scanner
static int value; // numerical value of the token currently read by the scanner
static int findPointer; // index of the find in "infos" by calling "FindSymbol"
static int i;
static int infos[infosSize]; // various infos (e.g. symbols)
static int j;
static int limit;
static int minusOne;
static int stringPointer; // index of the string in "infos" to be put out in "PutText"
static int namesPointer; // index of the top-most name in "infos"
static int nextLabel;
static int number;
static int nextCharacter; // the next character to be processed in the scanner
static int operation;
static int textIndex;
static int item;
static int itemsPointer; // index of the top-most item in "infos"

static void PushValue()
{
  itemsPointer = itemsPointer - 1;
  infos[itemsPointer] = item;
}

static void PopValue()
{
  itemsPointer = itemsPointer + 1;
}

static int TopValue()
{
  return infos[itemsPointer];
}

static void PutNumber()
{
  item = number % 10; PushValue();
  number = number / 10;
  if (number)
  {
    PutNumber();
  }
  item = TopValue();
  putchar(item + 48); PopValue();
}

static void PutName()
{
  i = 0;
  limit = infos[stringPointer + 1];
  while (i < limit)
  {
    putchar(infos[stringPointer + i + 2]);
    i = i + 1;
  }
}

static void PutNumbers()
{
  i = 0;
  limit = infos[stringPointer + 1];
  while (i < limit)
  {
    number = infos[stringPointer + i + 2]; PutNumber();
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
    curTxtChr = text[textIndex] % 256;
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

void PutTxt()
{
  textIndex = 0;
  while (1)
  {
    curTxtChr = text[textIndex] % 256;
    if (!curTxtChr)
    {
      return;
    }
    ExpandPlaceholders();
    textIndex = textIndex + 1;
  }
}

static void Fail()
{
  text = fail; PutTxt();
  exit(1);
}

static void PushLabel()
{
  item = nextLabel;
  PushValue();
  nextLabel = nextLabel + 1;
}

static void EmtDcl()
{
  text = declaration; stringPointer = findPointer; PutTxt();
}

static void EmitDuplicates()
{
  text = duplicates; number = value; PutTxt();
}

static void EmitFunction()
{
  text = function; stringPointer = findPointer; PutTxt();
}

static void EmitDataSegment()
{
  text = dataSegment; PutTxt();
}

static void EmitTextSegment()
{
  text = textSegment; PutTxt();
}

static void EmitHeader()
{
  text = header; PutTxt();
}

static void EmitPushName()
{
  text = pushName; stringPointer = namesPointer; PutTxt();
}

static void EmitPushValue()
{
  text = pushValue; number = value; PutTxt();
}

static void EmitReturn()
{
  text = returnFromSubroutine; PutTxt();
}

static void EmitFunctionCall()
{
  text = callSubroutine; PutTxt();
}

static void EmitLogicalNegation()
{
  text = logicalNegation; PutTxt();
}

static void EmitNumericalNegation()
{
  text = numericalNegation; PutTxt();
}

static void EmitAddCharacterIndex()
{
  text = addCharacterIndex; PutTxt();
}

static void EmitAddIntegerIndex()
{
  text = addIntegerIndex; PutTxt();
}

static void EmitBegin()
{
  text = beginLabel; number = TopValue(); PutTxt();
}

static void EmitJumpToBegin()
{
  text = jumpToBegin; number = TopValue(); PutTxt();
}

static void EmitElse()
{
  text = elseLabel; number = TopValue(); PutTxt();
}

static void EmitEnd()
{
  text = endLabel; number = TopValue(); PutTxt();
}

static void EmitJumpToElse()
{
  text = jumpToElse; number = TopValue(); PutTxt();
}

static void EmitJumpToEnd()
{
  text = jumpToEnd; number = TopValue(); PutTxt();
}

static void EmitRead()
{
  text = read; PutTxt();
}

static void EmitCodes()
{
  text = codes; stringPointer = namesPointer; PutTxt(); 
}

static void EmitIntegerArrayAddress()
{
  text = integerArrayAddress; PutTxt();
}

static void EmitCharacterArrayAddress()
{
  text = characterArrayAddress; PutTxt();
}

static void AddCurChr()
{
  infos[namesPointer + 1] = infos[namesPointer + 1] + 1;
  infos[namesPointer + infos[namesPointer + 1] + 1] = character;
}

static int IsCtrl()
{
  return (character < Space) | (Delete <= character);
}

static int IsWhitespace()
{
  return IsCtrl() | (character == ' ');
}

static int IsDigit()
{
  return (Zero <= character) & (character <= Nine);
}

static int IsLetter()
{
  return ((LowerA <= character) & (character <= LowerZ)) | ((UpperA <= character) & (character <= UpperZ));
}

static void DefineSymbol()
{
  findPointer = namesPointer;
  namesPointer = namesPointer + infos[namesPointer + 1];
  infos[namesPointer + 2] = kind;
  infos[namesPointer + 3] = value;
  infos[namesPointer + 4] = findPointer;
  namesPointer = namesPointer + 4;
}

static int FindSymbol()
{
  findPointer = infos[namesPointer];
  while (1)
  {
    limit = infos[findPointer + 1] + 2;
    j = 1;
    while (j < limit)
    {
      if (infos[findPointer + j] != infos[namesPointer + j])
      {
        j = limit;
      }
      j = j + 1;
    }
    if (j == limit)
    {
      kind = infos[findPointer + limit]; value = infos[findPointer + limit + 1];
      return 1;
    }
    if (findPointer == minusOne)
    {
      return 0;
    }
    findPointer = infos[findPointer];
  }
}

static void GetNxtChr()
{
  character = nextCharacter;
  nextCharacter = getchar();
  if (nextCharacter < 0)
  {
    nextCharacter = 0;
  }
}

static void GetChr()
{
  GetNxtChr();
  while ((character == '/') & (nextCharacter == '/'))
  {
    while ((character != Eof) & (character != 10))
    {
      GetNxtChr();
    }
    GetNxtChr();
  }
}

static void GetToken()
{
  infos[namesPointer + 1] = 0;
  while ((character != Eof) & IsWhitespace())
  {
    GetChr();
  }
  token = character;
  if (character == EqualsSign)
  {
    GetChr();
    if (character == EqualsSign)
    {
      GetChr();
      token = Equal;
      return;
    }
    return;
  }
  if (character == ExclamationMark)
  {
    GetChr();
    if (character == EqualsSign)
    {
      GetChr();
      token = NotEqual;
      return;
    }
    return;
  }
  if (character == Less)
  {
    GetChr();
    if (character == EqualsSign)
    {
      GetChr();
      token = LessOrEqual;
      return;
    }
    return;
  }
  if (character == Greater)
  {
    GetChr();
    if (character == EqualsSign)
    {
      GetChr();
      token = GreaterOrEqual;
      return;
    }
    return;
  }
  if (character == Apostrophe)
  {
    GetChr();
    value = character;
    GetChr();
    if (character == Apostrophe)
    {
      GetChr();
      token = Number;
      return;
    }
  }
  if (character == Quote)
  {
    GetChr();
    while ((character != Eof) & (character != Quote))
    {
      if (character == Backslash)
      {
        GetChr();
        if (character == LowerN)
        {
          character = LineFeed;
        }
      }
      AddCurChr();
      GetChr();
    }
    if (character == Quote)
    {
      GetChr();
      token = String;
      return;
    }
  }
  if (IsLetter())
  {
    while ((character != Eof) & IsLetter())
    {
      AddCurChr();
      GetChr();
    }
    if (FindSymbol())
    {
      if (kind == Keyword)
      {
        token = value;
        return;
      }
      if (kind == Constant)
      {
        token = Number;
        return;
      }
      if (kind == Function)
      {
        token = Function;
        return;
      }
      if (kind == Variable)
      {
        token = Variable;
        return;
      }
      if (kind == CharacterVariable)
      {
        token = CharacterVariable;
        return;
      }
      return;
    }
    token = Name;
    return;
  }
  if (IsDigit())
  {
    value = 0;
    while ((character != Eof) & IsDigit())
    {
      value = (value * 10) + (character - Zero);
      AddCurChr();
      GetChr();
    }
    token = Number;
    return;
  }
  GetChr();
}

static void Init()
{
  minusOne = -1;

  EmitHeader();

  nextLabel = 0;
  itemsPointer = infosSize;
  namesPointer = minusOne;

  character = 'c'; AddCurChr();
  character = 'o'; AddCurChr();
  character = 'n'; AddCurChr();
  character = 's'; AddCurChr();
  character = 't'; AddCurChr();
  kind = Keyword;
  value = Constant;
  DefineSymbol();

  character = 's'; AddCurChr();
  character = 't'; AddCurChr();
  character = 'a'; AddCurChr();
  character = 't'; AddCurChr();
  character = 'i'; AddCurChr();
  character = 'c'; AddCurChr();
  kind = Keyword;
  value = Static;
  DefineSymbol();

  character = 'r'; AddCurChr();
  character = 'e'; AddCurChr();
  character = 't'; AddCurChr();
  character = 'u'; AddCurChr();
  character = 'r'; AddCurChr();
  character = 'n'; AddCurChr();
  kind = Keyword;
  value = Return;
  DefineSymbol();

  character = 'i'; AddCurChr();
  character = 'f'; AddCurChr();
  kind = Keyword;
  value = If;
  DefineSymbol();

  character = 'e'; AddCurChr();
  character = 'l'; AddCurChr();
  character = 's'; AddCurChr();
  character = 'e'; AddCurChr();
  kind = Keyword;
  value = Else;
  DefineSymbol();

  character = 'w'; AddCurChr();
  character = 'h'; AddCurChr();
  character = 'i'; AddCurChr();
  character = 'l'; AddCurChr();
  character = 'e'; AddCurChr();
  kind = Keyword;
  value = While;
  DefineSymbol();

  character = 'i'; AddCurChr();
  character = 'n'; AddCurChr();
  character = 'c'; AddCurChr();
  character = 'l'; AddCurChr();
  character = 'u'; AddCurChr();
  character = 'd'; AddCurChr();
  character = 'e'; AddCurChr();
  kind = Keyword;
  value = Include;
  DefineSymbol();

  character = 'i'; AddCurChr();
  character = 'n'; AddCurChr();
  character = 't'; AddCurChr();
  kind = Keyword;
  value = Int;
  DefineSymbol();

  character = 'c'; AddCurChr();
  character = 'h'; AddCurChr();
  character = 'a'; AddCurChr();
  character = 'r'; AddCurChr();
  kind = Keyword;
  value = Char;
  DefineSymbol();

  character = 'v'; AddCurChr();
  character = 'o'; AddCurChr();
  character = 'i'; AddCurChr();
  character = 'd'; AddCurChr();
  kind = Keyword;
  value = Void;
  DefineSymbol();

  character = 'a'; AddCurChr();
  character = 'b'; AddCurChr();
  character = 'o'; AddCurChr();
  character = 'r'; AddCurChr();
  character = 't'; AddCurChr();
  kind = Function;
  value = Void;
  DefineSymbol();

  character = 'g'; AddCurChr();
  character = 'e'; AddCurChr();
  character = 't'; AddCurChr();
  character = 'c'; AddCurChr();
  character = 'h'; AddCurChr();
  character = 'a'; AddCurChr();
  character = 'r'; AddCurChr();
  kind = Function;
  value = Int;
  DefineSymbol();

  character = 'p'; AddCurChr();
  character = 'u'; AddCurChr();
  character = 't'; AddCurChr();
  character = 'c'; AddCurChr();
  character = 'h'; AddCurChr();
  character = 'a'; AddCurChr();
  character = 'r'; AddCurChr();
  kind = Function;
  value = Void;
  DefineSymbol();

  character = 'e'; AddCurChr();
  character = 'x'; AddCurChr();
  character = 'i'; AddCurChr();
  character = 't'; AddCurChr();
  kind = Function;
  value = Void;
  DefineSymbol();

  kind = Unknown;
  value = 0;
  nextCharacter = 0;
  GetNxtChr();
  GetChr();
  GetToken();
  activeSegment = NoSegment;
}

static void ParseNumber()
{
  if (token == Minus)
  {
    GetToken();
    if (token != Number)
    {
      Fail();
    }
    value = -value;
    GetToken();
    return;
  }
  if (token != Number)
  {
    Fail();
  }
  GetToken();
}

static void ParseExpression()
{
  if (token == ExclamationMark)
  {
    GetToken();
    ParseExpression();
    EmitLogicalNegation();
  }
  if (token == Minus)
  {
    GetToken();
    ParseExpression();
    EmitNumericalNegation();
  }
  if (token == Number)
  {
    EmitPushValue();
    GetToken();
  }
  if ((token == Variable) | (token == CharacterVariable))
  {
    item = token; PushValue();
    EmitPushName();
    GetToken();
    if (token == LeftBracket)
    {
      EmitRead();
      GetToken();
      ParseExpression();
      if (token != RightBracket)
      {
        Fail();
      }
      if (TopValue() == CharacterVariable)
      {
        EmitAddCharacterIndex();
      }
      else
      {
        EmitAddIntegerIndex();
      }
      GetToken();
    }
    if (token != EqualsSign)
    {
      EmitRead();
    }
    PopValue();
  }
  if (token == Function)
  {
    item = findPointer; PushValue();
    GetToken();
    if (token != LeftParenthesis)
    {
      Fail();
    }
    GetToken();
    if (token != RightParenthesis)
    {
      ParseExpression();
    }
    if (token != RightParenthesis)
    {
      Fail();
    }
    stringPointer = TopValue();
    EmitFunctionCall();
    PopValue();
    GetToken();
  }
  if (token == LeftParenthesis)
  {
    GetToken();
    ParseExpression();
    if (token != RightParenthesis)
    {
      Fail();
    }
    GetToken();
  }
  operation = 0;
  if (token == Plus)
  {
    operation = Add;
  }
  if (token == Minus)
  {
    operation = Sub;
  }
  if (token == Asterisk)
  {
    operation = Mul;
  }
  if (token == Slash)
  {
    operation = Div;
  }
  if (token == Percent)
  {
    operation = Mod;
  }
  if (token == Ampersand)
  {
    operation = And;
  }
  if (token == Pipe)
  {
    operation = Or;
  }
  if (token == Equal)
  {
    operation = Teq;
  }
  if (token == NotEqual)
  {
    operation = Tne;
  }
  if (token == Less)
  {
    operation = Tls;
  }
  if (token == LessOrEqual)
  {
    operation = Tle;
  }
  if (token == Greater)
  {
    operation = Tgr;
  }
  if (token == GreaterOrEqual)
  {
    operation = Tge;
  }
  if (token == EqualsSign)
  {
    operation = Assign;
  }
  if (operation)
  {
    GetToken();
    item = operation;
    PushValue();
    ParseExpression();
    operation = TopValue();
    PopValue();
    text = callOperation; number = operation; PutTxt();
  }
}

static void ParseBlock()
{
  if (token != LeftBrace)
  {
    Fail();
  }
  GetToken();
  while (token != RightBrace)
  {
    if ((token == Function) | (token == Variable) | (token == CharacterVariable))
    {
      ParseExpression();
      text = popEax; PutTxt();
      if (token != Semicolon)
      {
        Fail();
      }
      GetToken();
    }
    if (token == Return)
    {
      GetToken();
      if (token != Semicolon)
      {
        ParseExpression();
        text = popEax; PutTxt();
      }
      if (token != Semicolon)
      {
        Fail();
      }
      EmitReturn();
      GetToken();
    }
    if (token == If)
    {
      GetToken();
      if (token != LeftParenthesis)
      {
        Fail();
      }
      PushLabel();
      GetToken();
      ParseExpression();
      if (token != RightParenthesis)
      {
        Fail();
      }
      EmitJumpToElse();
      GetToken();
      ParseBlock();
      if (token == Else)
      {
        GetToken();
        EmitJumpToEnd();
        EmitElse();
        ParseBlock();
        EmitEnd();
      }
      else
      {
        EmitElse();
      }
      PopValue();
    }
    if (token == While)
    {
      GetToken();
      if (token != LeftParenthesis)
      {
        Fail();
      }
      PushLabel();
      EmitBegin();
      GetToken();
      ParseExpression();
      if (token != RightParenthesis)
      {
        Fail();
      }
      EmitJumpToElse();
      GetToken();
      ParseBlock();
      EmitJumpToBegin();
      EmitElse();
      PopValue();
    }
  }
  GetToken();
}

static void ParseDefinition()
{
  if (token == Hash)
  {
    GetToken();
    if (token != Include)
    {
      Fail();
    }
    GetToken();
    if (token != Less)
    {
      Fail();
    }
    GetToken();
    if (token != Name)
    {
      Fail();
    }
    GetToken();
    if (token != Greater)
    {
      Fail();
    }
    GetToken();
    return;
  }
  if (token == Constant)
  {
    GetToken();
    if (token != Int)
    {
      Fail();
    }
    GetToken();
    if (token != Name)
    {
      Fail();
    }
    kind = Constant;
    DefineSymbol();
    GetToken();
    if (token != EqualsSign)
    {
      Fail();
    }
    GetToken();
    ParseNumber();
    infos[namesPointer - 1] = value;
    if (token != Semicolon)
    {
      Fail();
    }
    GetToken();
    return;
  }
  if (token == Static)
  {
    GetToken();
  }
  if ((token == Void) | (token == Int) | (token == Char))
  {
    item = token; PushValue();
    GetToken();
    if (token == Asterisk)
    {
      GetToken();
    }
    if (token != Name)
    {
      Fail();
    }
    DefineSymbol();
    GetToken();
    if (token == LeftParenthesis)
    {
      infos[namesPointer - 2] = Function;
      if (activeSegment != TextSegment)
      {
        activeSegment = TextSegment;
        EmitTextSegment();
      }
      EmitFunction();
      GetToken();
      if (token != RightParenthesis)
      {
        Fail();
      }
      GetToken();
      ParseBlock();
      EmitReturn();
      return;
    }
    if (TopValue() == Char)
    {
      infos[namesPointer - 2] = CharacterVariable;
    }
    else
    {
      infos[namesPointer - 2] = Variable;
    }
    if (activeSegment != DataSegment)
    {
      activeSegment = DataSegment;
      EmitDataSegment();
    }
    EmtDcl();
    value = 1; // default value
    if (token == LeftBracket)
    {
      value = 0;
      if (TopValue() == Char)
      {
        EmitCharacterArrayAddress();
      }
      else
      {
        EmitIntegerArrayAddress();
      }
      GetToken();
      if (token == Number)
      {
        GetToken();
      }
      if (token == RightBracket)
      {
        GetToken();
      }
    }
    if (token == EqualsSign)
    {
      GetToken();
      if (token != String)
      {
        Fail();
      }
      EmitCodes();
      GetToken();
    }
    else
    {
      EmitDuplicates(); 
    }
    if (token != Semicolon)
    {
      Fail();
    }
    GetToken();
    putchar(LineFeed);
    PopValue();
  }
}

static void ParseCpl()
{
  while (token != Eof)
  {
    ParseDefinition();
  }
}

int main()
{
  Init();
  ParseCpl();
}
