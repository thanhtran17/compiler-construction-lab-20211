/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/
void skipBlank() {
  // skip liên tục đến khi hết dấu cách
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE))
    readChar();
}

/******************************************************************/
void skipComment() {
  int state = 0;
  while ((currentChar != EOF) && (state < 2)) {
    switch (charCodes[currentChar]) {
      // check xem dấu * đã xuất hiện lần 2 chưa
      case CHAR_TIMES:
        state = 1;
        break;
      // nếu là dấu ) và đã xuất hiện 1 dấu *
      case CHAR_RPAR:
        if (state == 1) 
          state = 2;
        else state = 0;
        break;
      default:
        state = 0;
    }
    readChar();
  }
  if (state != 2) 
    error(ERR_ENDOFCOMMENT, lineNo, colNo);
}

/******************************************************************/
Token* readIdentKeyword(void) {
  Token *token = makeToken(TK_NONE, lineNo, colNo);
  int count = 1;
  token->string[0] = (char) currentChar;
  readChar();

  while ((currentChar != EOF) &&
        ((charCodes[currentChar] == CHAR_LETTER) || 
        (charCodes[currentChar] == CHAR_DIGIT))) {
    if(count <= MAX_IDENT_LEN)
      token->string[count++] = (char) currentChar;
    readChar();
  }

  if (count > MAX_IDENT_LEN){
    error(ERR_IDENTTOOLONG, token->lineNo, token->colNo);
    return token;
  }

  token->string[count] = '\0';
  token->tokenType = checkKeyword(token->string);

  if(token->tokenType == TK_NONE)
    token->tokenType = TK_IDENT;

  return token;
}

/******************************************************************/
Token* readNumber(void) {
  Token *token = makeToken(TK_NUMBER, lineNo, colNo);
  int count = 0;

  while ((currentChar != EOF) &&
         (charCodes[currentChar] == CHAR_DIGIT)){
    token->string[count++] = (char)currentChar;
    readChar();
  }

  token->string[count] = '\0';
  token->value = atoi(token->string);

  return token;
}

/******************************************************************/
Token* readConstChar(void) {
  Token *token = makeToken(TK_CHAR, lineNo, colNo);

  readChar();

  if(currentChar == EOF){
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }

  token->string[0] = currentChar;
  token->string[1] = '\0';
  token->value = currentChar;

  readChar();

  if(currentChar == EOF){
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }


  if(charCodes[currentChar] == CHAR_SINGLEQUOTE){
    readChar();
    return token;
  }
  else{
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }
}

/******************************************************************/
Token* getToken(void) {
  Token *token;
  int ln, cn;

  // 37
  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
    // 1
    case CHAR_SPACE: 
      skipBlank(); 
      // skip dấu cách xong, lấy token tiếp theo
      return getToken();
    // 2
    case CHAR_LPAR:
      ln = lineNo;
      cn = colNo;
      readChar();
      // lỗi 40
      if (currentChar == EOF){
        token = makeToken(TK_NONE, ln, cn);
        error(ERR_INVALIDSYMBOL, ln, cn);
      }

      switch(charCodes[currentChar]){
        // 6
        case CHAR_PERIOD:
          token = makeToken(SB_LSEL, ln, cn);
          readChar();
          return token;
        // 3, 4, 5
        case CHAR_TIMES:
          readChar();
          skipComment();
          return getToken();
        // 7
        default:
          token = makeToken(SB_LPAR, lineNo, colNo);
          return token;
      }
    // 8, 9
    case CHAR_LETTER: 
      return readIdentKeyword();
    // 10, 11
    case CHAR_DIGIT: 
      return readNumber();
    // 12
    case CHAR_PLUS: 
      token = makeToken(SB_PLUS, lineNo, colNo);
      readChar(); 
      return token;
    // 13
    case CHAR_MINUS: 
      token = makeToken(SB_MINUS, lineNo, colNo);
      readChar(); 
      return token;
    // 14
    case CHAR_TIMES: 
      token = makeToken(SB_TIMES, lineNo, colNo);
      readChar(); 
      return token;
    // 15
    case CHAR_SLASH: 
      token = makeToken(SB_SLASH, lineNo, colNo);
      readChar(); 
      return token;
    // 16
    case CHAR_EQ:
      token = makeToken(SB_EQ, lineNo, colNo);
      readChar();
      return token;
    // 17
    case CHAR_COMMA:
      token = makeToken(SB_COMMA, lineNo, colNo);
      readChar();
      return token;
    // 18
    case CHAR_SEMICOLON:
      token = makeToken(SB_SEMICOLON, lineNo, colNo);
      readChar();
      return token;
    // 19
    case CHAR_PERIOD:
      ln = lineNo;
      cn = colNo;
      // 21
      token = makeToken(SB_PERIOD, ln, cn);
      readChar();
      // 20
      if (currentChar != EOF && charCodes[currentChar] == CHAR_RPAR){
        token = makeToken(SB_RSEL, ln, cn);
        readChar();
      }
      return token;
    // 22
    case CHAR_GT:
      ln = lineNo;
      cn = colNo;
      // 24
      token = makeToken(SB_GT, ln, cn);
      readChar();
      // 23 
      if (currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
        token = makeToken(SB_GE, ln, cn);
        readChar();
      }
      return token;
    // 25
    case CHAR_LT:
      ln = lineNo;
      cn = colNo;
      // 27
      token = makeToken(SB_LT, ln, cn);
      readChar();
      // 26
      if (currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
        token = makeToken(SB_LE, ln, cn);
        readChar();
      }
      return token;
    // 28
    case CHAR_EXCLAIMATION:
      ln = lineNo;
      cn = colNo;
      readChar();
      // 29
      if (currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
        token = makeToken(SB_NEQ, ln, cn);
        readChar();
      }
      // lỗi 30
      else {
        token = makeToken(TK_NONE, ln, cn);
        error(ERR_INVALIDSYMBOL, ln, cn);
      }
      return token;
    // 31
    case CHAR_COLON:
      ln = lineNo;
      cn = colNo;
      // 33
      token = makeToken(SB_COLON, ln, cn);
      readChar();
      // 32
      if (currentChar != EOF && charCodes[currentChar] == CHAR_EQ){
        token = makeToken(SB_ASSIGN, ln, cn);
        readChar();
      }
      return token;
    // 34 35 36
    case CHAR_SINGLEQUOTE: return readConstChar();
    // 39
    case CHAR_RPAR:
      token = makeToken(SB_RPAR, lineNo, colNo);
      readChar();
      return token;
    // 38
    default:
      token = makeToken(TK_NONE, lineNo, colNo);
      error(ERR_INVALIDSYMBOL, lineNo, colNo);
      readChar(); 
      return token;
  }
}

/******************************************************************/
void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

// nếu argc > 1 thì flow của scanner bắt đầu từ đây
int scan(char *fileName) {
  Token *token;

  // mở stream
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  // nhận dạng được token
  // nếu không phải EOF thì in col, row, & loại token
  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  // giải phóng & đóng stream
  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/
int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}

/******************************************************************/
// FLOW HOÀN CHỈNH CỦA MỘT SCANNER

// - Đọc input: argc <= 1 báo lỗi, > 1 thì scan
// - Không lỗi -> mở stream, check xem đã có stream chưa, chưa có báo lỗi
// - Không lỗi -> getToken()
// - Trong getToken():
//    + khai báo struct token mới
//    + kí tự hiện tại là eof thì tạo & return TK_EOF (không in token này)
//    + nếu không phải là eof: switch case cho charCodes của kí tự hiện tại
//      . SPACE: skip đến khi hết space, get token tiếp theo
//      . LETTER: 
//      . DIGIT: 
//      . PLUS: 
//      . v.v....
/******************************************************************/