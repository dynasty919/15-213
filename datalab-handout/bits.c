/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
    int a = x | y;
    int b = ~ a;
    int c = ~ x;
    int d = ~ y;
    int e = b | c | d;
    int result = ~ e;

    return result;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
    int a = ( 0x000000ff << ( n << 3 ) );
    int b = x & a;
    int c = ( b >> (n << 3) );
    int result = c & 0x000000ff;

    return result;

}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
    int a = ~(0x00000001 << 31);
    int b = a & x;
    int temp1 = b >> n;
    int d = (0x00000001 << 31);
    int e = d & x; //(1000 or 0000)
    int f = e >> 31;
    int g = f & 0x00000001;
    int temp2 = g << (31 + (~n) + 1);
    int result = temp2 + temp1;

    return result;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
    int a1 = 0x00000055;
    int b1 = a1 + (a1 << 8);
    int c1 = b1 + (b1 << 16);
    int d = (x & c1) + ((x >> 1) & c1);

    int a2 = 0x00000033;
    int b2 = a2 + (a2 << 8);
    int c2 = b2 + (b2 << 16);
    int num1 = (d & c2) + ((d >> 2) & c2);

    int a3 = 0x0000000f;
    int b3 = a3 + (a3 << 8);
    int c3 = b3 + (b3 << 16);
    int num2 = (num1 & c3) + ((num1 >> 4) & c3);

    int a4 = 0x000000ff;
    int c4 = a4 + (a4 << 16);
    int num4 = (num2 & c4) + ((num2 >> 8) & c4);

    int c5 = a4 + (a4 << 8);
    int num8 = (num4 & c5) + ((num4 >> 16) & c5);

    return num8;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
    int a = x | (x >> 16);
    int b = a | (a >> 8);
    int c = b | (b >> 4);
    int d = c | (c >> 2);
    int e = d | (d >> 1);
    int result = (0x1 & (0x1 ^ e));

    return result;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    int a = 0x00000001 << 31;
    return a;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
    int b = ~0;
    int move = n + b;
    int c = b << move;
    int d = (c & x) >> move;
    int result = (!d) | (!(~d));

    return result;
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    int a = x >> 31;
    int b = x + (a & ((1 << n) + ~0));
    int result = b >> n;

    return result;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    int result = ~x + 1;
    return result;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
    int a = 1 << 31;
    int b = x & a;
    int result = (!b) & (!(!x));
    return result;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    int tmin = 1 << 31;
    int a = ~tmin;

    int cond1 = (!!(x & tmin)) & (!(y & tmin));

    int x1 = x & a;
    int y1 = y & a;
    int b = ~x1 + 1 + y1;
    int flag = !(b & tmin);
    int cond2 = (!((x^y) & tmin)) & flag;

    int result = cond1 + cond2;
    return result;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
    int a16 = !!(x >> 16);

    int temp1 = x >> 8;
    int temp2 = (a16 << 31) >> 31;
    int a8 = !!(temp1 >> ( temp2 & 16 ));

    int temp3 = x >> 4;
    int temp4 = (a8 << 31) >> 31;
    int a4 = !!(temp3 >> (temp2 & 16) >>(temp4 & 8));

    int temp5 = x >> 2;
    int temp6 = (a4 << 31) >> 31;
    int a2 = !!(temp5 >> (temp2 & 16) >>(temp4 & 8) >> (temp6 & 4));

    int temp7 = x >> 1;
    int temp8 = (a2 << 31) >> 31;
    int a1 = !!(temp7 >> (temp2 & 16) >>(temp4 & 8) >> (temp6 & 4) >> (temp8 & 2));

    int result = (a16 << 4) + (a8 << 3) + (a4 << 2) + (a2 << 1) + a1;

    return result;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
    unsigned result;
    unsigned tmin = 1 << 31;
    unsigned a = uf & (~tmin);
    if (((a >> 23) ^ 0xff) | (!(a << 9)))
        result = uf ^ tmin;
    else
        result = uf;
    return result;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
    int tmin = 1 << 31;
    int bias = 127;
    int e = 0;
    int s = 0;
    int exp;
    int frac;
    int num = 1;
    unsigned result;
    int reserve;
    int temp1;
    int temp2;
    int round;
    int lastnum;
    int temp3;
    int temp4;
    int deserted;

    if ((x == tmin)) {
        result = 0xcf << 24;
        return result;
    }
    else if (!x) {
        result = 0;
        return result;
    }
    else if (x & tmin) {
        s = 1 << 31;
        num = ~x + 1;
    }
    else {
        s = 0 << 31;
        num = x;
    }

    reserve = num;
    while (num ^ 1) {
        num = num >> 1;
        e = e + 1;
    }
    temp1 = reserve << (31 + ~e + 1);
    temp2 = temp1 & (~tmin);
    frac = temp2 >> 8;
    exp = (e + bias) << 23;

    lastnum = frac & 1;
    temp3 = frac << 8;
    temp4 = temp3 ^ temp2;
    deserted = temp4 << 24;

    if (deserted == tmin)
        round = lastnum;
    else if (deserted & tmin)
        round = 1;
    else
        round = 0;

    result = s + exp + frac + round;

    return result;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
    unsigned tmin = 1 << 31;
    unsigned negtmin = ~tmin;
    unsigned s = uf & tmin;
    unsigned exp = (uf & negtmin) >> 23;
    unsigned frac = ((uf << 8) & negtmin) >> 8;
    unsigned newfrac;

    if (exp == 0xff)
        return uf;
    else if (exp == 0xfe)
        return s + (0xff << 23);
    else if (exp)
        return s + ((exp + 1) << 23) + frac;
    else {
        newfrac = frac + frac;
        return s + newfrac;
    }
}
