#include <cstdio>
#include <cstdlib>

const int chrAmpersand = '&';
const int chrApostrophe = 39;
const int chrAsterisk = '*';
const int chrDel = 127;
const int chrEquals = '=';
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
const int chrPercent = '%';
const int chrPipe = 124;
const int chrPlus = 43;
const int chrQuote = 34;
const int chrRBrace = 125;
const int chrRBracket = 93;
const int chrRParen = 41;
const int chrSemicolon = 59;
const int chrSlash = '/';
const int chrSpace = 32;
const int chrUpperA = 65;
const int chrUpperZ = 90;
const int chrZero = 48;
const int eof = 0;
const int labStackLen = 1000;
const int namTabLen = 5000;
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

static int buf[12];
static int bufIdx;
static int chr;
static int curChr;
static int curSym;
static int curTok;
static int curVal;
static int funIdx;
static int hasInit;
static int i;
static int j;
static int labStack[labStackLen];
static int labTos;
static int limit;
static int namIdx;
static int namTab[namTabLen];
static int namTos;
static int num;
static int nxtLab;
static int minusOne;
static int op;
static int reg;
static int result;
static char* txt;
static char strDatSeg[] = "section '.data' data readable writeable";
static char strTxtSeg[] = "section '.text' code readable executable";

static int Fail()
{
  putchar('f'); putchar('a'); putchar('i'); putchar('l'); putchar(10); 
  exit(1);
}

void NumToBuf()
{
  bufIdx = 0;
  buf[0] = num % 10;
  num = num / 10;
  while (0 < num)
  {
    bufIdx = bufIdx + 1;
    buf[bufIdx] = num % 10;
    num = num / 10;
  }
}

static void PushLab()
{
  labTos = labTos + 1;
  labStack[labTos] = nxtLab;
  nxtLab = nxtLab + 1;
}

static void PopLab()
{
  labTos = labTos - 1;
}

static int LabTop()
{
  return labStack[labTos];
}

static void EmtTxt()
{
  i = 0;
  while (txt[i] != 0)
  {
    putchar(txt[i]);
    i = i + 1;
  }
}

static void EmtCall()
{
  putchar('c'); putchar('a'); putchar('l'); putchar('l');
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

static void EmtJmp()
{
  putchar('j'); putchar('m'); putchar('p');
}

static void EmtLea()
{
  putchar('l'); putchar('e'); putchar('a'); 
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
  i = 0;
  limit = namTab[namIdx + 1];
  while (i < limit)
  {
    putchar(namTab[namIdx + i + 2]);
    i = i + 1;
  }
}

static void EmtVal()
{
  num = curVal;
  NumToBuf();
  while (0 <= bufIdx)
  {
    putchar(buf[bufIdx] + 48);
    bufIdx = bufIdx - 1;
  }
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
  EmtNam(); putchar(':'); putchar(10);
}

static void EmtDatSeg()
{
  txt = strDatSeg; EmtTxt(); putchar(10);
}

static void EmtTxtSeg()
{
  txt = strTxtSeg; EmtTxt(); putchar(10);
}

static void EmtHdr()
{
  putchar(102);putchar(111);putchar(114);putchar(109);putchar(97);putchar(116);putchar(32);putchar(80);putchar(69);putchar(32);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(10);
  putchar(101);putchar(110);putchar(116);putchar(114);putchar(121);putchar(32);putchar(115);putchar(116);putchar(97);putchar(114);putchar(116);putchar(10);
  putchar(115);putchar(101);putchar(99);putchar(116);putchar(105);putchar(111);putchar(110);putchar(32);putchar(39);putchar(46);putchar(105);putchar(100);putchar(97);putchar(116);putchar(97);putchar(39);putchar(32);putchar(105);putchar(109);putchar(112);putchar(111);putchar(114);putchar(116);putchar(32);putchar(100);putchar(97);putchar(116);putchar(97);putchar(32);putchar(114);putchar(101);putchar(97);putchar(100);putchar(97);putchar(98);putchar(108);putchar(101);putchar(32);putchar(119);putchar(114);putchar(105);putchar(116);putchar(101);putchar(97);putchar(98);putchar(108);putchar(101);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(114);putchar(118);putchar(97);putchar(32);putchar(107);putchar(101);putchar(114);putchar(110);putchar(101);putchar(108);putchar(78);putchar(97);putchar(109);putchar(101);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(114);putchar(118);putchar(97);putchar(32);putchar(107);putchar(101);putchar(114);putchar(110);putchar(101);putchar(108);putchar(84);putchar(97);putchar(98);putchar(108);putchar(101);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(107);putchar(101);putchar(114);putchar(110);putchar(101);putchar(108);putchar(84);putchar(97);putchar(98);putchar(108);putchar(101);putchar(58);putchar(10);
  putchar(69);putchar(120);putchar(105);putchar(116);putchar(80);putchar(114);putchar(111);putchar(99);putchar(101);putchar(115);putchar(115);putchar(32);putchar(100);putchar(100);putchar(32);putchar(114);putchar(118);putchar(97);putchar(32);putchar(101);putchar(120);putchar(105);putchar(116);putchar(80);putchar(114);putchar(111);putchar(99);putchar(101);putchar(115);putchar(115);putchar(10);
  putchar(71);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(32);putchar(100);putchar(100);putchar(32);putchar(114);putchar(118);putchar(97);putchar(32);putchar(103);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(10);
  putchar(83);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(32);putchar(100);putchar(100);putchar(32);putchar(114);putchar(118);putchar(97);putchar(32);putchar(115);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(10);
  putchar(82);putchar(101);putchar(97);putchar(100);putchar(70);putchar(105);putchar(108);putchar(101);putchar(32);putchar(100);putchar(100);putchar(32);putchar(114);putchar(118);putchar(97);putchar(32);putchar(114);putchar(101);putchar(97);putchar(100);putchar(70);putchar(105);putchar(108);putchar(101);putchar(10);
  putchar(87);putchar(114);putchar(105);putchar(116);putchar(101);putchar(70);putchar(105);putchar(108);putchar(101);putchar(32);putchar(100);putchar(100);putchar(32);putchar(114);putchar(118);putchar(97);putchar(32);putchar(119);putchar(114);putchar(105);putchar(116);putchar(101);putchar(70);putchar(105);putchar(108);putchar(101);putchar(10);
  putchar(71);putchar(101);putchar(116);putchar(83);putchar(116);putchar(100);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(32);putchar(100);putchar(100);putchar(32);putchar(114);putchar(118);putchar(97);putchar(32);putchar(103);putchar(101);putchar(116);putchar(83);putchar(116);putchar(100);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(10);
  putchar(100);putchar(100);putchar(32);putchar(48);putchar(10);
  putchar(107);putchar(101);putchar(114);putchar(110);putchar(101);putchar(108);putchar(78);putchar(97);putchar(109);putchar(101);putchar(58);putchar(10);
  putchar(100);putchar(98);putchar(32);putchar(39);putchar(75);putchar(69);putchar(82);putchar(78);putchar(69);putchar(76);putchar(51);putchar(50);putchar(46);putchar(68);putchar(76);putchar(76);putchar(39);putchar(44);putchar(48);putchar(10);
  putchar(101);putchar(120);putchar(105);putchar(116);putchar(80);putchar(114);putchar(111);putchar(99);putchar(101);putchar(115);putchar(115);putchar(58);putchar(10);
  putchar(100);putchar(119);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(98);putchar(32);putchar(39);putchar(69);putchar(120);putchar(105);putchar(116);putchar(80);putchar(114);putchar(111);putchar(99);putchar(101);putchar(115);putchar(115);putchar(39);putchar(44);putchar(48);putchar(10);
  putchar(103);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(58);putchar(10);
  putchar(100);putchar(119);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(98);putchar(32);putchar(39);putchar(71);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(39);putchar(44);putchar(48);putchar(10);
  putchar(115);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(58);putchar(10);
  putchar(100);putchar(119);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(98);putchar(32);putchar(39);putchar(83);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(39);putchar(44);putchar(48);putchar(10);
  putchar(114);putchar(101);putchar(97);putchar(100);putchar(70);putchar(105);putchar(108);putchar(101);putchar(58);putchar(10);
  putchar(100);putchar(119);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(98);putchar(32);putchar(39);putchar(82);putchar(101);putchar(97);putchar(100);putchar(70);putchar(105);putchar(108);putchar(101);putchar(39);putchar(44);putchar(48);putchar(10);
  putchar(119);putchar(114);putchar(105);putchar(116);putchar(101);putchar(70);putchar(105);putchar(108);putchar(101);putchar(58);putchar(10);
  putchar(100);putchar(119);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(98);putchar(32);putchar(39);putchar(87);putchar(114);putchar(105);putchar(116);putchar(101);putchar(70);putchar(105);putchar(108);putchar(101);putchar(39);putchar(44);putchar(48);putchar(10);
  putchar(103);putchar(101);putchar(116);putchar(83);putchar(116);putchar(100);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(58);putchar(10);
  putchar(100);putchar(119);putchar(32);putchar(48);putchar(10);
  putchar(100);putchar(98);putchar(32);putchar(39);putchar(71);putchar(101);putchar(116);putchar(83);putchar(116);putchar(100);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(39);putchar(44);putchar(48);putchar(10);
  putchar(115);putchar(101);putchar(99);putchar(116);putchar(105);putchar(111);putchar(110);putchar(32);putchar(39);putchar(46);putchar(100);putchar(97);putchar(116);putchar(97);putchar(39);putchar(32);putchar(100);putchar(97);putchar(116);putchar(97);putchar(32);putchar(114);putchar(101);putchar(97);putchar(100);putchar(97);putchar(98);putchar(108);putchar(101);putchar(32);putchar(119);putchar(114);putchar(105);putchar(116);putchar(101);putchar(97);putchar(98);putchar(108);putchar(101);putchar(10);
  putchar(115);putchar(116);putchar(100);putchar(73);putchar(110);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(32);putchar(100);putchar(100);putchar(32);putchar(63);putchar(10);
  putchar(115);putchar(116);putchar(100);putchar(79);putchar(117);putchar(116);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(32);putchar(100);putchar(100);putchar(32);putchar(63);putchar(10);
  putchar(99);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(32);putchar(100);putchar(100);putchar(32);putchar(63);putchar(10);
  putchar(105);putchar(111);putchar(67);putchar(104);putchar(114);putchar(32);putchar(100);putchar(98);putchar(32);putchar(63);putchar(10);
  putchar(98);putchar(121);putchar(116);putchar(101);putchar(115);putchar(67);putchar(111);putchar(117);putchar(110);putchar(116);putchar(32);putchar(100);putchar(100);putchar(32);putchar(63);putchar(10);
  putchar(115);putchar(101);putchar(99);putchar(116);putchar(105);putchar(111);putchar(110);putchar(32);putchar(39);putchar(46);putchar(116);putchar(101);putchar(120);putchar(116);putchar(39);putchar(32);putchar(99);putchar(111);putchar(100);putchar(101);putchar(32);putchar(114);putchar(101);putchar(97);putchar(100);putchar(97);putchar(98);putchar(108);putchar(101);putchar(32);putchar(101);putchar(120);putchar(101);putchar(99);putchar(117);putchar(116);putchar(97);putchar(98);putchar(108);putchar(101);putchar(10);
  putchar(101);putchar(120);putchar(105);putchar(116);putchar(58);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(91);putchar(69);putchar(120);putchar(105);putchar(116);putchar(80);putchar(114);putchar(111);putchar(99);putchar(101);putchar(115);putchar(115);putchar(93);putchar(10);
  putchar(103);putchar(101);putchar(116);putchar(99);putchar(104);putchar(97);putchar(114);putchar(58);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(101);putchar(98);putchar(112);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(101);putchar(98);putchar(112);putchar(44);putchar(32);putchar(101);putchar(115);putchar(112);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(91);putchar(105);putchar(111);putchar(67);putchar(104);putchar(114);putchar(93);putchar(44);putchar(32);putchar(48);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(48);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(98);putchar(121);putchar(116);putchar(101);putchar(115);putchar(67);putchar(111);putchar(117);putchar(110);putchar(116);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(49);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(105);putchar(111);putchar(67);putchar(104);putchar(114);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(91);putchar(115);putchar(116);putchar(100);putchar(73);putchar(110);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(93);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(91);putchar(82);putchar(101);putchar(97);putchar(100);putchar(70);putchar(105);putchar(108);putchar(101);putchar(93);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(101);putchar(97);putchar(120);putchar(44);putchar(32);putchar(48);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(97);putchar(108);putchar(44);putchar(32);putchar(91);putchar(105);putchar(111);putchar(67);putchar(104);putchar(114);putchar(93);putchar(10);
  putchar(112);putchar(111);putchar(112);putchar(32);putchar(101);putchar(98);putchar(112);putchar(10);
  putchar(114);putchar(101);putchar(116);putchar(32);putchar(48);putchar(32);putchar(10);
  putchar(112);putchar(117);putchar(116);putchar(99);putchar(104);putchar(97);putchar(114);putchar(58);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(101);putchar(98);putchar(112);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(101);putchar(98);putchar(112);putchar(44);putchar(32);putchar(101);putchar(115);putchar(112);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(101);putchar(97);putchar(120);putchar(44);putchar(32);putchar(91);putchar(101);putchar(98);putchar(112);putchar(32);putchar(43);putchar(32);putchar(56);putchar(93);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(91);putchar(105);putchar(111);putchar(67);putchar(104);putchar(114);putchar(93);putchar(44);putchar(32);putchar(97);putchar(108);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(48);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(98);putchar(121);putchar(116);putchar(101);putchar(115);putchar(67);putchar(111);putchar(117);putchar(110);putchar(116);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(49);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(105);putchar(111);putchar(67);putchar(104);putchar(114);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(91);putchar(115);putchar(116);putchar(100);putchar(79);putchar(117);putchar(116);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(93);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(91);putchar(87);putchar(114);putchar(105);putchar(116);putchar(101);putchar(70);putchar(105);putchar(108);putchar(101);putchar(93);putchar(10);
  putchar(112);putchar(111);putchar(112);putchar(32);putchar(101);putchar(98);putchar(112);putchar(10);
  putchar(114);putchar(101);putchar(116);putchar(32);putchar(52);putchar(32);putchar(10);
  putchar(115);putchar(116);putchar(97);putchar(114);putchar(116);putchar(58);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(48);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(54);putchar(104);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(91);putchar(71);putchar(101);putchar(116);putchar(83);putchar(116);putchar(100);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(93);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(91);putchar(115);putchar(116);putchar(100);putchar(73);putchar(110);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(93);putchar(44);putchar(32);putchar(101);putchar(97);putchar(120);putchar(32);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(48);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(53);putchar(104);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(91);putchar(71);putchar(101);putchar(116);putchar(83);putchar(116);putchar(100);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(93);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(91);putchar(115);putchar(116);putchar(100);putchar(79);putchar(117);putchar(116);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(93);putchar(44);putchar(32);putchar(101);putchar(97);putchar(120);putchar(32);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(99);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(91);putchar(115);putchar(116);putchar(100);putchar(73);putchar(110);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(93);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(91);putchar(71);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(93);putchar(10);
  putchar(109);putchar(111);putchar(118);putchar(32);putchar(101);putchar(97);putchar(120);putchar(44);putchar(32);putchar(91);putchar(99);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(93);putchar(10);
  putchar(97);putchar(110);putchar(100);putchar(32);putchar(101);putchar(97);putchar(120);putchar(44);putchar(32);putchar(48);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(70);putchar(68);putchar(104);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(101);putchar(97);putchar(120);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(91);putchar(115);putchar(116);putchar(100);putchar(73);putchar(110);putchar(72);putchar(97);putchar(110);putchar(100);putchar(108);putchar(101);putchar(93);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(91);putchar(83);putchar(101);putchar(116);putchar(67);putchar(111);putchar(110);putchar(115);putchar(111);putchar(108);putchar(101);putchar(77);putchar(111);putchar(100);putchar(101);putchar(93);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(109);putchar(97);putchar(105);putchar(110);putchar(10);
  putchar(112);putchar(117);putchar(115);putchar(104);putchar(32);putchar(48);putchar(10);
  putchar(99);putchar(97);putchar(108);putchar(108);putchar(32);putchar(91);putchar(69);putchar(120);putchar(105);putchar(116);putchar(80);putchar(114);putchar(111);putchar(99);putchar(101);putchar(115);putchar(115);putchar(93);putchar(10);
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

static void EmtCndJmp()
{
  if (op == tokEqual)
  {
    putchar('j'); putchar('e');
    return;
  }
  if (op == tokNotEqual)
  {
    putchar('j'); putchar('n'); putchar('e');
    return;
  }
  if (op == tokLess)
  {
    putchar('j'); putchar('l');
    return;
  }
  if (op == tokLessOrEqual)
  {
    putchar('j'); putchar('l'); putchar('e');
    return;
  }
  if (op == tokGreater)
  {
    putchar('j'); putchar('g');
    return;
  }
  if (op == tokGreaterOrEqual)
  {
    putchar('j'); putchar('g'); putchar('e');
  }
}

static void EmtReg()
{
  putchar('e'); putchar(reg); putchar('x'); 
}

static void EmtRegInd()
{
  putchar('['); EmtReg(); putchar(']');
}

static void EmtPushNam()
{
  EmtPush(); putchar(' '); EmtNam(); putchar(10);
}

static void EmtPushReg()
{
  EmtPush(); putchar(' '); EmtReg(); putchar(10);
}

static void EmtPushRegInd()
{
  EmtPush(); putchar(' '); putchar('d'); putchar('w'); putchar('o'); putchar('r');  putchar('d'); EmtRegInd(); putchar(10);
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
  reg = 'b'; EmtPopReg();
  reg = 'a'; EmtPopReg();
  EmtMov(); putchar(' '); reg = 'a'; EmtRegInd(); putchar(','); reg = 'b'; EmtReg(); putchar(10);
}

static void EmtProcCall()
{
  EmtCall(); putchar(' '); namIdx = funIdx; EmtNam(); putchar(10);  
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

static void EmtComparison()
{
  EmtMov(); putchar(' '); reg = 'c'; EmtReg(); putchar(','); curVal = 1; EmtVal(); putchar(10);
  reg = 'b'; EmtPopReg();
  reg = 'a'; EmtPopReg();
  EmtCmp(); putchar(' '); reg = 'a'; EmtReg(); putchar(','); reg = 'b'; EmtReg(); putchar(10);
  EmtCndJmp(); putchar(' '); putchar('@'); putchar('f'); putchar(10);
  EmtDec(); putchar(' '); reg = 'c'; EmtReg(); putchar(10);
  putchar('@'); putchar('@'); putchar(':'); putchar(10);
  reg = 'c'; EmtPushReg();
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
  reg = 'b'; EmtPopReg();
  reg = 'a'; EmtPopReg();
  EmtLea(); putchar(' '); reg = 'a'; EmtReg(); putchar(','); putchar('['); reg = 'a'; EmtReg(); putchar('+'); putchar('4'); putchar('*'); reg = 'b'; EmtReg(); putchar(']'); putchar(10);
  reg = 'a'; EmtPushReg();
}

static void EmtBegin()
{
  putchar('b'); curVal = LabTop(); EmtVal(); putchar(':'); putchar(10);
}

static void EmtJmpBegin()
{
  EmtJmp(); putchar(' '); putchar('b'); curVal = LabTop(); EmtVal(); putchar(10);
}

static void EmtEnd()
{
  putchar('e'); curVal = LabTop(); EmtVal(); putchar(':'); putchar(10);
}

static void EmtJmpEnd()
{
  reg = 'a'; EmtPopReg();
  EmtCmp(); putchar(' '); reg = 'a'; EmtReg(); putchar(','); curVal = 0; EmtVal(); putchar(10);
  EmtJe(); putchar(' '); putchar('e'); curVal = LabTop(); EmtVal(); putchar(10);
}

static void EmtPushTosInd()
{
  reg = 'a'; EmtPopReg();
  EmtPushRegInd();
}

static void EmtStrInt()
{
  i = 0;
  limit = namTab[namTos + 1];
  while (i < limit)
  {
    curVal = namTab[namTos + i + 2];
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
  namTab[namTos + 1] = namTab[namTos + 1] + 1;
  namTab[namTos + namTab[namTos + 1] + 1] = chr;
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
  i = namTos + namTab[namTos + 1];
  namTab[i + 2] = curSym;
  namTab[i + 3] = curVal;
  namTab[i + 4] = namTos;
  namTos = i + 4;
}

static int FndSym()
{
  i = namTab[namTos];
  while (1)
  {
    limit = namTab[i + 1];
    if (limit == namTab[namTos + 1])
    {
      j = 0;
      while (j < limit)
      {
        if (namTab[i + j + 2] != namTab[namTos + j + 2])
        {
          j = limit;
        }
        j = j + 1;
      }
      if (j == limit)
      {
        curSym = namTab[i + namTab[i + 1] + 2];
        curVal = namTab[i + namTab[i + 1] + 3];
        return 1;
      }
    }
    if (i == minusOne)
    {
      return 0;
    }
    i = namTab[i];
  }
}

static int GetChr()
{
  result = getchar();
  if (result <= 0)
  {
    result = 0;
  }
  return result;
}

static void RdTok()
{
  namTab[namTos + 1] = 0;
  while ((curChr != eof) && IsWhitespace())
  {
    curChr = GetChr();
  }
  curTok = tokEof;
  if (curChr == eof)
  {
    return;
  }
  if (curChr == chrLBrace)
  {
    curChr = GetChr();
    curTok = tokLBrace;
    return;
  }
  if (curChr == chrLBracket)
  {
    curChr = GetChr();
    curTok = tokLBracket;
    return;
  }
  if (curChr == chrLParen)
  {
    curChr = GetChr();
    curTok = tokLParen;
    return;
  }
  if (curChr == chrRBrace)
  {
    curChr = GetChr();
    curTok = tokRBrace;
    return;
  }
  if (curChr == chrRBracket)
  {
    curChr = GetChr();
    curTok = tokRBracket;
    return;
  }
  if (curChr == chrRParen)
  {
    curChr = GetChr();
    curTok = tokRParen;
    return;
  }
  if (curChr == chrSemicolon)
  {
    curChr = GetChr();
    curTok = tokSemicolon;
    return;
  }
  if (curChr == chrPlus)
  {
    curChr = GetChr();
    curTok = tokAdd;
    return;
  }
  if (curChr == chrMinus)
  {
    curChr = GetChr();
    curTok = tokSub;
    return;
  }
  if (curChr == chrAsterisk)
  {
    curChr = GetChr();
    curTok = tokMul;
    return;
  }
  if (curChr == chrSlash)
  {
    curChr = GetChr();
    curTok = tokDiv;
    return;
  }
  if (curChr == chrPercent)
  {
    curChr = GetChr();
    curTok = tokMod;
    return;
  }
  if (curChr == chrEquals)
  {
    curChr = GetChr();
    if (curChr == chrEquals)
    {
      curChr = GetChr();
      curTok = tokEqual;
      return;
    }
    curTok = tokAssign;
    return;
  }
  if (curChr == chrExclamationMark)
  {
    curChr = GetChr();
    if (curChr == chrEquals)
    {
      curChr = GetChr();
      curTok = tokNotEqual;
      return;
    }
    curTok = tokNot;
    return;
  }
  if (curChr == chrLess)
  {
    curChr = GetChr();
    if (curChr == chrEquals)
    {
      curChr = GetChr();
      curTok = tokLessOrEqual;
      return;
    }
    curTok = tokLess;
    return;
  }
  if (curChr == chrGreater)
  {
    curChr = GetChr();
    if (curChr == chrEquals)
    {
      curChr = GetChr();
      curTok = tokGreaterOrEqual;
      return;
    }
    curTok = tokGreater;
    return;
  }
  if (curChr == chrAmpersand)
  {
    curChr = GetChr();
    if (curChr == chrAmpersand)
    {
      curChr = GetChr();
      curTok = tokAnd;
      return;
    }
    return;
  }
  if (curChr == chrPipe)
  {
    curChr = GetChr();
    if (curChr == chrPipe)
    {
      curChr = GetChr();
      curTok = tokOr;
      return;
    }
    return;
  }
  if (curChr == chrHash)
  {
    curChr = GetChr();
    curTok = tokHash;
    return;
  }
  if (curChr == chrApostrophe)
  {
    curChr = GetChr();
    if (!IsCtrl())
    {
      curVal = curChr;
      curChr = GetChr();
      if (curChr == chrApostrophe)
      {
        curChr = GetChr();
        curTok = tokNumber;
        return;
      }
    }
  }
  if (curChr == chrQuote)
  {
    curChr = GetChr();
    while ((curChr != eof) && (curChr != chrQuote))
    {
      AddCurChr();
      curChr = GetChr();
    }
    if (curChr == chrQuote)
    {
      curChr = GetChr();
      curTok = tokString;
      return;
    }
  }
  if (IsLetter())
  {
    while ((curChr != eof) && IsLetter())
    {
      AddCurChr();
      curChr = GetChr();
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
      curChr = GetChr();
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
  labTos = minusOne;
  namTos = minusOne;

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
  curChr = GetChr();
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
    namIdx = namTos;
    EmtPushNam();
    RdTok();
    if (curTok == tokLBracket)
    {
      EmtPushTosInd();
      RdTok();
      ParseExpression();
      if (curTok != tokRBracket)
      {
        Fail();
      }
      EmtAddIdx();
      RdTok();
    }
    EmtPushTosInd();
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
    op = tokEqual;
    EmtComparison();
  }
  if (curTok == tokNotEqual)
  {
    RdTok();
    ParseExpression();
    op = tokNotEqual;
    EmtComparison();
  }
  if (curTok == tokLess)
  {
    RdTok();
    ParseExpression();
    op = tokLess;
    EmtComparison();
  }
  if (curTok == tokLessOrEqual)
  {
    RdTok();
    ParseExpression();
    op = tokLessOrEqual;
    EmtComparison();
  }
  if (curTok == tokGreater)
  {
    RdTok();
    ParseExpression();
    op = tokGreater;
    EmtComparison();
  }
  if (curTok == tokGreaterOrEqual)
  {
    RdTok();
    ParseExpression();
    op = tokGreaterOrEqual;
    EmtComparison();
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
      namIdx = namTos;
      EmtPushNam();
      RdTok();
      if (curTok == tokLBracket)
      {
        EmtPushTosInd();
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
      PushLab();
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
      PopLab();
    }
    if (curTok == tokWhile)
    {
      RdTok();
      if (curTok != tokLParen)
      {
        Fail();
      }
      PushLab();
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
      PopLab();
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
    namTab[i + 3] = curVal;
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
    namIdx = namTos;
    SetSym();
    RdTok();
    if (curTok == tokLParen)
    {
      namTab[i + 2] = symFun;
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
    namTab[i + 2] = symVar;
    EmtDatSeg();
    EmtDcl();
    curVal = 1;
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

