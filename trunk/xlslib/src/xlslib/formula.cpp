/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * xlslib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xlslib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with xlslib.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2010 Ger Hobbelt
 *  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <xlsys.h>

#include <formula.h>
#include <globalrec.h>
#include <unit.h>
#include <datast.h>


using namespace xlslib_core;

/*
The ptgSheet and ptgEndSheet tokens are no longer used. The external sheet references are contained in the ptgNameX, ptgRef3d, and ptgArea3d tokens.

Name                                Ptg                             Type

ptgExp                              01h                             control
ptgTbl                              02h                             control
ptgAdd                              03h                             operator
ptgSub                              04h                             operator
ptgMul                              05h                             operator
ptgDiv                              06h                             operator
ptgPower                            07h                             operator
ptgConcat                           08h                             operator
ptgLT                               09h                             operator
ptgLE                               0Ah                             operator
ptgEQ                               0Bh                             operator
ptgGE                               0Ch                             operator
ptgGT                               0Dh                             operator
ptgNE                               0Eh                             operator
ptgIsect                            0Fh                             operator
ptgUnion                            10h                             operator
ptgRange                            11h                             operator
ptgUplus                            12h                             operator
ptgUminus                           13h                             operator
ptgPercent                          14h                             operator
ptgParen                            15h                             control
ptgMissArg                          16h                             operand
ptgStr                              17h                             operand
ptgAttr                             19h                             control
ptgSheet                            1Ah                             (ptg DELETED)
ptgEndSheet                         1Bh                             (ptg DELETED)
ptgErr                              1Ch                             operand
ptgBool                             1Dh                             operand
ptgInt                              1Eh                             operand
ptgNum                              1Fh                             operand
ptgArray                            20h                             operand, reference class
ptgFunc                             21h                             operator
ptgFuncVar                          22h                             operator
ptgName                             23h                             operand, reference class
ptgRef                              24h                             operand, reference class
ptgArea                             25h                             operand, reference class
ptgMemArea                          26h                             operand, reference class
ptgMemErr                           27h                             operand, reference class
ptgMemNoMem                         28h                             control
ptgMemFunc                          29h                             control
ptgRefErr                           2Ah                             operand, reference class
ptgAreaErr                          2Bh                             operand, reference class
ptgRefN                             2Ch                             operand, reference class
ptgAreaN                            2Dh                             operand, reference class
ptgMemAreaN                         2Eh                             control
ptgMemNoMemN                        2Fh                             control
ptgNameX                            39h                             operand, reference class
ptgRef3d                            3Ah                             operand, reference class
ptgArea3d                           3Bh                             operand, reference class
ptgRefErr3d                         3Ch                             operand, reference class
ptgAreaErr3d                        3Dh                             operand, reference class
ptgArrayV                           40h                             operand, value class
ptgFuncV                            41h                             operator
ptgFuncVarV                         42h                             operator
ptgNameV                            43h                             operand, value class
ptgRefV                             44h                             operand, value class
ptgAreaV                            45h                             operand, value class
ptgMemAreaV                         46h                             operand, value class
ptgMemErrV                          47h                             operand, value class
ptgMemNoMemV                        48h                             control
ptgMemFuncV                         49h                             control
ptgRefErrV                          4Ah                             operand, value class
ptgAreaErrV                         4Bh                             operand, value class
ptgRefNV                            4Ch                             operand, value class
ptgAreaNV                           4Dh                             operand, value class
ptgMemAreaNV                        4Eh                             control
ptgMemNoMemNV                       4Fh                             control
ptgFuncCEV                          58h                             operator
ptgNameXV                           59h                             operand, value class
ptgRef3dV                           5Ah                             operand, value class
ptgArea3dV                          5Bh                             operand, value class
ptgRefErr3dV                        5Ch                             operand, value class
ptgAreaErr3dV                       5Dh                             operand, value class
ptgArrayA                           60h                             operand, array class
ptgFuncA                            61h                             operator
ptgFuncVarA                         62h                             operator
ptgNameA                            63h                             operand, array class
ptgRefA                             64h                             operand, array class
ptgAreaA                            65h                             operand, array class
ptgMemAreaA                         66h                             operand, array class
ptgMemErrA                          67h                             operand, array class
ptgMemNoMemA                        68h                             control
ptgMemFuncA                         69h                             control
ptgRefErrA                          6Ah                             operand, array class
ptgAreaErrA                         6Bh                             operand, array class
ptgRefNA                            6Ch                             operand, array class
ptgAreaNA                           6Dh                             operand, array class
ptgMemAreaNA                        6Eh                             control
ptgMemNoMemNA                       6Fh                             control
ptgFuncCEA                          78h                             operator
ptgNameXA                           79h                             operand, array class (NEW ptg)
ptgRef3dA                           7Ah                             operand, array class (NEW ptg)
ptgArea3dA                          7Bh                             operand, array class (NEW ptg)
ptgRefErr3dA                        7Ch                             operand, array class (NEW ptg)
ptgAreaErr3dA                       7Dh                             operand, array class (NEW ptg)
                                                                




Expression Evaluation

Calculation of Excel formulas is a straightforward process. A last-in, first-out (LIFO) stack, the operand stack, is maintained during calculation. When an operand is encountered, it is pushed onto the stack. When an operator is encountered, it operates on the topmost operand or operands. Operator precedence is irrelevant at evaluation time; operators are handled as soon as they are encountered. There are three kinds of operators: unary, binary, and function. Unary operators, such as the minus sign that negates a number, operate only on the top operand. Binary operators, such as the addition operator, operate on the top two operands. Function operators, which implement Excel functions, operate on a variable number of operands, depending on how many arguments the function accepts. All operators work by popping the required operands from the stack, perf




Unary Operator Tokens

The unary operator tokens for Excel are described in the following paragraphs. These operators pop the top argument from the operand stack, perform a calculation, and then push the result back onto the operand stack.

ptgUplus: Unary Plus (ptg=12h)
Has no effect on the operand.
ptgUminus: Unary Minus (ptg=13h)
Negates the operand on the top of the stack.
ptgPercent: Percent Sign (ptg=14h)
Divides the top operand by 100.


Binary Operator Tokens

There are several binary operator ptgs. All binary operator ptgs pop the top two arguments from the operand stack, perform the associated calculation, and then push the result back onto the operand stack.

ptgAdd: Addition (ptg=03h)
Adds the top two operands.
ptgSub: Subtraction (ptg=04h)
Subtracts the top operand from the second-to-top operand.
ptgMul: Multiplication (ptg=05h)
Multiplies the top two operands.
ptgDiv: Division (ptg=06h)
Divides the top operand by the second-to-top operand.
ptgPower: Exponentiation (ptg=07h)
Raises the second-to-top operand to the power of the top operand.
ptgConcat: Concatenation (ptg=08h)
Appends the top operand to the second-to-top operand.
ptgLT: Less Than (ptg=09h)
Evaluates to TRUE if the second-to-top operand is less than the top operand; evaluates to FALSE otherwise.
ptgLE: Less Than or Equal (ptg=0Ah)
Evaluates to TRUE if the second-to-top operand is less than or equal to the top operand; evaluates to FALSE otherwise.
ptgEQ: Equal (ptg=0Bh)
Evaluates to TRUE if the top two operands are equal; evaluates to FALSE otherwise.
ptgGE: Greater Than or Equal (ptg=0Ch)
Evaluates to TRUE if the second-to-top operand is greater than or equal to the top operand; evaluates to FALSE otherwise.
ptgGT: Greater Than (ptg=0Dh)
Evaluates to TRUE if the second-to-top operand is greater than the top operand; evaluates to FALSE otherwise.
ptgNE: Not Equal (ptg=0Eh)
Evaluates to TRUE if the top two operands are not equal; evaluates to FALSE otherwise.
ptgIsect: Intersection (ptg=0Fh)
Computes the intersection of the top two operands. This is the Excel space operator.
ptgUnion: Union (ptg=10h)
Computes the union of the top two operands. This is the Excel comma operator.
ptgRange: Range (ptg=11h)
Computes the minimal bounding rectangle of the top two operands. This is the Excel colon operator.


Operand Tokens: Constant

These operand tokens push a single constant operand onto the operand stack.

ptgMissArg: Missing Argument (Operand, ptg=16h)
Indicates a missing argument to an Excel function. For example, the second (missing) argument to the function DCOUNT(Database,,Criteria) would be stored as a ptgMissArg.

ptgStr: String Constant (Operand, ptg=17h)
Indicates a string constant ptg followed by a string length field (00 to FFh) and the actual string.

Offset
Name
Size
Contents
0
cch
1
The length of the string
1
rgch
var
The string

ptgStr requires special handling when parsed expressions are scanned. For more information, see ?Scanning a Parsed Expression?.

In BIFF8, the rgch contains a unicode string. For more information, see ?Unicode Strings in BIFF8?.

ptgErr: Error Value (Operand, ptg=1Ch)
This ptg is followed by the 1-byte error value (err). For a list of error values, see ?BOOLERR?.

Offset
Name
Size
Contents
0
err
1
An error value

ptgBool: Boolean (Operand, ptg=1Dh)
This ptg is followed by a byte that represents TRUE or FALSE.
Offset
Name
Size
Contents
0
f
1
=1 for TRUE =0 for FALSE
ptgInt: Integer (Operand, ptg=1Eh)
This ptg is followed by a word that contains an unsigned integer.
Offset
Name
Size
Contents
0
w
2
An unsigned integer value
ptgNum: Number (Operand, ptg=1Fh)
This ptg is followed by an 8-byte IEEE floating-point number.
Offset
Name
Size
Contents
0
num
8
An IEEE floating-point number
Operand Tokens
Operand tokens push operand values onto the operand stack. These values fall into one of three classes � reference class, value class, or array class � depending on what type of value the formula expects from the operand. The type of value is determined by the context of the operand when the formula is parsed by Excel.
Reference Class
Some operands are required by context to evaluate to references. In this case, the term reference is a general term meaning one or more areas on an Excel worksheet. When the Excel expression evaluator encounters a reference class operand, it pushes only the reference itself onto the operand stack; it does not de-reference it to return the underlying value or values. For example, because we have a ptgRef, a reference (to the cell B5) is pushed onto the stack. This function returns the column width of cell B5; therefore, only the reference to B5 is required, and there�s no need to de-reference to the value stored in cell B5.
Value Class
This is the most common type of operand. Value class operands push a single de-referenced value onto the operand stack. Using the formula =A1+1 as an example, because we have a ptgRefV, the value (of cell A1, for example: 5) is pushed onto the stack.
Array Class
This operand pushes an array of values onto the operand stack. You can specify the values in an array constant or in a reference to cells. Using the formula =SUM({1,2,3;4,5,6}) as an example, because we have a ptgArrayA, the whole array ({1,2,3;4,5,6}) is pushed onto the stack.
ptg Values for Operand Tokens
The three classes of operand tokens are divided numerically, as shown in the following table.
Operand class
Ptg values
Reference
20h�3Fh
Value
40h�5Fh
Array
60h�7Fh
The arithmetic difference between ptg classes is 20h. This is the basis for forming the class variants of ptgs. Class variants of ptgs are formed from the reference class ptg, also known as the base ptg. To form the value class ptg from the base ptg, add 20h to the ptg and append V (for ?value?) to the ptg name. To form the array class ptg from the base ptg, add 40h to the ptg and append A (for ?array?) to the ptg name. These rules are summarized in the following table for a sample base ptg, ptgRef.
Class
Name
Ptg
Reference
ptgRef
24h
Value
ptgRefV
44h
Array
ptgRefA
64h
The following example is a suggested method for calculating the base ptg from any class variant. 
if (ptg & 40h) { (* Value class ptg. Set the 20h bit to make it Reference class, then strip off the high-order bits. *)
ptgBase = (ptg | 20h) & 3Fh; } 
else { (* Reference or Array class ptg. The 20h bit is already set, so just have to strip off the high-order bits. *)
ptgBase = ptg & 3Fh; } 

A more efficient implementation would define a macro that computes the base ptg, as in the following example. 
#define PtgBase(ptg) (((ptg & 0x40) ? (ptg | 0x20): ptg) & 0x3F)
Operand Tokens: Base
This section describes the operand tokens in their base form (also known as reference class operand tokens).
ptgArray: Array Constant (Operand, ptg=20h)
		  Array constant followed by 7 reserved bytes. The token value for ptgArray consists of the array dimensions and the array values. ptgArray differs from most other operand tokens in that the token value doesn�t follow the token type. Instead, the token value is appended to the saved parsed expression, immediately following the last token. The format of the token value is shown in the following table.
		  Offset
		  Name
		  Size
		  Contents
		  0
		  ccol
		  1
		  The number of columns in the array constant
		  1
		  crw
		  2
		  The number of rows in the array constant
		  3
		  rgval
		  var
		  The array values
		  An array with 256 columns is stored with a ccol=0, because a single byte cannot store the integer 256. This is unambiguous, because a 0-column array constant is meaningless.
		  The number of values in the array constant is equal to the product of the array dimensions, crw*ccol. Each value is either an 8-byte IEEE floating-point number or a string. The two formats for these values are shown in the following tables.
		  IEEE Floating-Point Number
		  Offset
		  Name
		  Size
		  Contents
		  0
		  grbit
		  1
		  =01h
		  1
		  num
		  8
		  IEEE floating-point number
		  String
		  Offset
		  Name
		  Size
		  Contents
		  0
		  grbit
		  1
		  =02h
		  1
		  cch
		  1
		  The length of the string
		  2
		  rgch
		  var
		  The string
		  If a formula contains more than one array constant, the token values for the array constants are appended to the saved parsed expression in order: first the values for the first array constant, then the values for the second array constant, and so on. If a formula contains very long array constants, the FORMULA, ARRAY, or NAME record containing the parsed expression may overflow into CONTINUE records (to accommodate all of the array values). In such cases, an individual array value is never split between records, but record boundaries are established between adjacent array values. The reference class ptgArray never appears in an Excel formula; only the ptgArrayV and ptgArrayA classes are used.
ptgName: Name (Operand, ptg=23h) � BIFF8
		 This ptg stores the index to a name. The ilbl field is a 1-based index to the table of NAME records in the workbook.
		 Offset
		 Name
		 Size
		 Contents
		 0
		 ilbl
		 2
		 Index to the NAME table
		 2
		 (Reserved)
		 2
		 Reserved; must be 0 (zero)
ptgName: Name (Operand, ptg=23h) � BIFF7 and earlier
		 This ptg stores the index to a name. The ilbl field is a 1-based index to the table of NAME records in the workbook.
		 Offset
		 Name
		 Size
		 Contents
		 0
		 ilbl
		 2
		 Index to the NAME table
		 2
		 (Reserved)
		 12
		 Reserved; must be 0 (zero)
ptgRef: Cell Reference (Operand, ptg=24h) � BIFF8
		This ptg specifies a reference to a single cell. It is followed by references for the row and column that contain the cell. The column number is encoded.
		Offset
		Name
		Size
		Contents
		0
		rw
		2
		The column of the reference
		2
		grbitCol
		2
		(See the following table)
		Only the low-order 14 bits of the grbitCol field store the column number of the reference. The 2 MSBs specify whether the row and column references are relative or absolute. The following table shows the bit structure of the grbitCol field.
		Bits
		Mask
		Name
		Contents
		15
		8000h
		fRwRel
		=1 if the row offset is relative =0 otherwise
		14
		4000h
		fColRel
		=1 if the column offset is relative =0 otherwise
		13�0
		3FFFh
		col
		The column number or column offset (0-based)
		For example, cell C5 is row number 4, column number 2 (Excel stores 0-based cell references). Therefore, the absolute reference $C$5 is stored in a ptgRef, as shown in the following file fragment. 24 04 00 02 00 In this case, rw=0004h and grbitCol=0002h. Note: bits 14 and 15 of grbitCol are both 0 (zero). The relative reference C5 is stored in a ptgRef, as shown in the following file fragment. 24 04 00 02 C0 In this case, where grbitCol=C004h and col=02h, bits 14 and 15 of grbitCol are both 1. Mixed references are stored in the same way, with appropriate coding in grbitCol.
ptgRef: Cell Reference (Operand, ptg=24h) � BIFF7 and earlier
		This ptg specifies a reference to a single cell. It is followed by references for the row and column that contain the cell. The row number is encoded.
		Offset
		Name
		Size
		Contents
		0
		grbitRw
		2
		(See the following table)
		2
		col
		1
		The column of the reference
		Only the low-order 14 bits of the grbitRw field store the row number of the reference. The 2 MSBs specify whether the row and column references are relative or absolute. The following table shows the bit structure of the grbitRw field.
		Bits
		Mask
		Name
		Contents
		15
		8000h
		fRwRel
		=1 if the row offset is relative =0 otherwise
		14
		4000h
		fColRel
		=1 if the column offset is relative =0 otherwise
		13�0
		3FFFh
		rw
		The row number or row offset (0-based)
		For example, cell C5 is row number 4, column number 2 (Excel stores 0-based cell references). Therefore, the absolute reference $C$5 is stored in a ptgRef, as shown in the following file fragment. 24 04 00 02
		In this case, grbitRw=0004h and col=02h. Note: bits 14 and 15 of grbitRw are both 0 (zero). The relative reference C5 is stored in a ptgRef, as shown in the following file fragment. 24 04 C0 02 In this case, where grbitRw=C004h and col=02h, bits 14 and 15 of grbitRw are both 1. Mixed references are stored in the same way, with appropriate coding in grbitRw.
ptgArea: Area Reference (Operand, ptg=25h) � BIFF8
		 This ptg specifies a reference to a rectangle (range) of cells. ptgArea is followed by 8 bytes that define the first row, last row, first column, and last column of the rectangle. The numbers of the first and last columns are encoded.
		 Offset
		 Name
		 Size
		 Contents
		 0
		 rwFirst
		 2
		 The first row of the reference
		 2
		 rwLast
		 2
		 The last row of the reference
		 4
		 grbitColFirst
		 2
		 (See the following table)
		 6
		 grbitColLast
		 2
		 (See the following table)
		 Only the low-order 14 bits of the grbitColFirst and grbitColLast fields store the column offsets of the reference. The 2 MSBs of each field specify whether the row and column offset are relative or absolute. The following table shows the bit structure of the grbitColFirst and grbitColLast fields.
		 Bits
		 Mask
		 Name
		 Contents
		 15
		 8000h
		 fRwRel
		 =1 if the row offset is relative =0 otherwise
		 14
		 4000h
		 fColRel
		 =1 if the column offset is relative =0 otherwise
		 13�0
		 3FFFh
		 col
		 The column number or column offset (0-based)
ptgArea: Area Reference (Operand, ptg=25h) � BIFF7 and earlier
		 This ptg specifies a reference to a rectangle (range) of cells. ptgArea is followed by 6 bytes that define the first row, last row, first column, and last column of the rectangle. The numbers of the first and last rows are encoded.
		 Offset
		 Name
		 Size
		 Contents
		 0
		 grbitRwFirst
		 2
		 (See the following table)
		 2
		 grbitRwLast
		 2
		 (See the following table)
		 4
		 colFirst
		 1
		 The first column of the reference
		 5
		 colLast
		 1
		 The last column of the reference
		 Only the low-order 14 bits of the grbitRwFirst and grbitRwLast fields store the row offsets of the reference. The 2 MSBs of each field specify whether the row and column offset are relative or absolute. The following table shows the bit structure of the grbitRwFirst and grbitRwLast fields.
		 Bits
		 Mask
		 Name
		 Contents
		 15
		 8000h
		 fRwRel
		 =1 if the row offset is relative =0 otherwise
		 14
		 4000h
		 fColRel
		 =1 if the column offset is relative =0 otherwise
		 13�0
		 3FFFh
		 rw
		 The row number or row offset (0-based)
ptgMemArea: Constant Reference Subexpression (Operand, ptg=26h)
			This ptg is used to optimize reference expressions. A reference expression consists of operands � usually references to cells or areas � joined by reference operators (intersection, union, and range). Three examples of reference expressions are given in the following table.
			Reference expression
			Evaluates to
			(A1,C3,D3:D5)
			Two single cells and a 3x1 area
			(A1:C3) (B2:D4)
			A 2x2 area (the space character is the intersection operator)
			(Name C3)
			The smallest area that contains both C3 and all the cells referenced in Name (the space character is the intersection operator)
			Many reference expressions evaluate to constant references. In the preceding examples, the first two expressions always evaluate to the same reference. The third example doesn�t evaluate to a constant reference because the name�s definition may change, which might cause the reference expression to evaluate differently. When a reference expression evaluates to a constant reference, Excel stores the constant reference in the parsed formula through a ptgMemArea token. This saves time during expression evaluation, because the constant part of the expression is pre-evaluated. This part of the expression is known as a reference subexpression. The token value for ptgMemArea consists of two parts: the length of the reference subexpression, and the value of the reference subexpression. The length is stored immediately following the ptgMemArea, whereas the value is appended to the saved parsed expression, immediately following the last token. The format of the length is shown in the following table.
			Offset
			Name
			Size
			Contents
			0
			(Reserved)
			4
			4
			cce
			2
			The length of the reference subexpression
			Immediately following this part of the token value is the reference subexpression itself. The rest of the token value (that is, the value of the reference subexpression) is appended to the parsed expression in the format shown in the following table.
			Offset
			Name
			Size
			Contents
			0
			cref
			2
			The number of rectangles to follow
			2
			rgref
			var
			An array of rectangles
			Each rgref rectangle is 6 bytes long and contains the fields listed in the following table.
			Offset
			Name
			Size
			Contents
			0
			rwFirst
			2
			The first row
			2
			rwLast
			2
			The last row
			4
			colFirst
			1
			The first column
			5
			colLast
			1
			The last column
			If a formula contains more than one ptgMemArea, the token values are appended to the saved parsed expression in order: first the values for the first ptgMemArea, then the values for the second ptgMemArea, and so on.
			If a formula contains very long reference expressions, the BIFF record containing the parsed expression may be too long to fit in a single record. Excel uses CONTINUE records to store long formulas. However, an individual rgref rectangle is never split between records; record boundaries occur between successive rectangles. For more information about the CONTINUE records, see ?CONTINUE?.
ptgMemErr: Erroneous Constant Reference Subexpression (Operand, ptg=27h)
		   This ptg is closely related to ptgMemArea. It is used for pre-evaluating reference subexpressions that do not evaluate to references. For example, consider the formula =SUM(C:C 3:3), which is the sum of the intersection of column C and row 3 (the space between C:C and 3:3 is the intersection operator). The argument to the SUM function is a valid reference subexpression that generates a ptgMemArea for pre-evaluation. However, if you delete column C, the formula adjusts to =SUM(#REF! 3:3). In this case, the argument to SUM is still a constant reference subexpression, but it doesn�t evaluate to a reference. Therefore, a ptgMemErr is used for pre-evaluation. The token value consists of the error value and the length of the reference subexpression. Its format is shown in the following table.
		   Offset
		   Name
		   Size
		   Contents
		   0
		   (Reserved)
		   4
		   4
		   cce
		   2
		   The length of the reference subexpression
		   The reference subexpression will contain a ptgRefErr or ptgAreaErr.
ptgRefErr: Deleted Cell Reference (Operand, ptg=2Ah) � BIFF8
		   This ptg specifies a cell reference adjusted to #REF! as a result of worksheet editing (such as cutting, pasting, and deleting). The ptgRefErr is followed by4 unused bytes.
		   Offset
		   Name
		   Size
		   Contents
		   0
		   (Reserved)
		   4
		   The original base type of the adjusted ptg is ptgRef or ptgRefN.
ptgRefErr: Deleted Cell Reference (Operand, ptg=2Ah) � BIFF7 and earlier
		   This ptg specifies a cell reference adjusted to #REF! as a result of worksheet editing (such as cutting, pasting, and deleting). The ptgRefErr is followed by 3 unused bytes.
		   Offset
		   Name
		   Size
		   Contents
		   0
		   (Reserved)
		   3
		   The original base type of the adjusted ptg is ptgRef or ptgRefN.
ptgAreaErr: Deleted Area Reference (Operand, ptg=2Bh) � BIFF8
			This ptg specifies an area reference adjusted to #REF! as a result of worksheet editing (such as cutting, pasting, and deleting). The ptgAreaErr is followed by 8 unused bytes.
			Offset
			Name
			Size
			Contents
			0
			(Reserved)
			8
			The original base type of the adjusted ptg is ptgArea or ptgAreaN.
ptgAreaErr: Deleted Area Reference (Operand, ptg=2Bh) � BIFF7 and earlier
			This ptg specifies an area reference that was adjusted to #REF! as a result of worksheet editing (such as cutting, pasting, and deleting). The ptgAreaErr is followed by 6 unused bytes.
			Offset
			Name
			Size
			Contents
			0
			(Reserved)
			6
			The original base type of the adjusted ptg is ptgArea or ptgAreaN.
ptgRefN: Cell Reference Within a Shared Formula (Operand, ptg=2Ch) � BIFF8
		 Similar to its ptgRef counterpart, the ptgRefN specifies a reference to a single cell. It is followed by references for the row and column that contain the cell; the row number of the cell is encoded as bit fields. In BIFF5 and later, ptgRefN is used only in shared formulas. In earlier versions of Excel, ptgRefN was used in names.
		 Offset
		 Name
		 Size
		 Contents
		 0
		 rw
		 2
		 The row (or row offset) of the reference
		 2
		 grbitCol
		 2
		 (See the following table)
		 Only the low-order 14 bits of the grbitCol field store the column number of the reference. The 2 MSBs specify whether the row and column references are relative or absolute. The following table shows the bit structure of the grbitCol field.
		 Bits
		 Mask
		 Name
		 Contents
		 15
		 8000h
		 fRwRel
		 =1 if the row offset is relative =0 otherwise
		 14
		 4000h
		 fColRel
		 =1 if the column offset is relative =0 otherwise
		 13�0
		 3FFFh
		 col
		 The column number or column offset (0-based)
		 The only difference between ptgRefN and ptgRef is in the way relative references are stored. Relative references in shared formulas are stored as offsets, not as row and column numbers (as in ptgRef). For more information, see ?SHRFMLA?.
ptgRefN: Cell Reference Within a Shared Formula (Operand, ptg=2Ch) � BIFF7 and earlier
		 Similar to its ptgRef counterpart, the ptgRefN specifies a reference to a single cell. It is followed by references for the row and column that contain the cell; the row number of the cell is encoded as bit fields. In BIFF5 and later, ptgRefN is used only in shared formulas. In earlier versions of Excel, ptgRefN was used in names.
		 Offset
		 Name
		 Size
		 Contents
		 0
		 grbitRw
		 2
		 (See the following table)
		 2
		 col
		 1
		 The column (or column offset) of the reference
		 Only the low-order 14 bits of the grbitRw field store the row number of the reference. The 2 MSBs specify whether the row and column references are relative or absolute. The following table shows the bit structure of the grbitRw field.
		 Bits
		 Mask
		 Name
		 Contents
		 15
		 8000h
		 fRwRel
		 =1 if the row offset is relative =0 otherwise
		 14
		 4000h
		 fColRel
		 =1 if the column offset is relative =0 otherwise
		 13�0
		 3FFFh
		 rw
		 The row number or row offset (0-based)
		 The only difference between ptgRefN and ptgRef is in the way relative references are stored. Relative references in shared formulas are stored as offsets, not as row and column numbers (as in ptgRef). For more information, see ?SHRFMLA?.
ptgAreaN: Area Reference Within a Shared Formula (Operand, ptg=2Dh) � BIFF8
		  The ptgAreaN token specifies a reference to a rectangle of cells. Both the first column and last column are encoded. In BIFF5 and later, ptgAreaN is used only in shared formulas. In earlier versions, it was used in names.
		  Offset
		  Name
		  Size
		  Contents
		  0
		  rwFirst
		  2
		  The first row of the absolute reference or relative reference
		  2
		  rwLast
		  2
		  The last row of the absolute reference or relative reference
		  4
		  grbitColFirst
		  2
		  (See the following table)
		  6
		  grbitColLast
		  2
		  (See the following table)
		  Only the low-order 14 bits of the grbitColFirst and grbitColLast fields store the column offsets of the reference. The 2 MSBs of each field specify whether the row and column offset are relative or absolute. The following table shows the bit structure of the grbitColFirst and grbitColLast fields.
		  Bits
		  Mask
		  Name
		  Contents
		  15
		  8000h
		  fRwRel
		  =1 if the row offset is relative =0 otherwise
		  Bits
		  Mask
		  Name
		  Contents
		  14
		  4000h
		  fColRel
		  =1 if the column offset is relative =0 otherwise
		  13�0
		  3FFFh
		  col
		  The column number or column offset (0-based)
		  The only difference between ptgAreaN and ptgArea is in the way relative references are stored.
ptgAreaN: Area Reference Within a Shared Formula (Operand, ptg=2Dh) � BIFF7 and earlier
		  The ptgAreaN token specifies a reference to a rectangle of cells. Both the first row and last row are stored as bit fields. In BIFF5 and later, ptgAreaN is used only in shared formulas. In earlier versions, it was used in names.
		  Offset
		  Name
		  Size
		  Contents
		  0
		  grbitRwFirst
		  2
		  The first row of the absolute reference or relative reference offset bit fields
		  2
		  grbitRwLast
		  2
		  The last row of the absolute reference or relative reference offset bit fields
		  4
		  colFirst
		  1
		  The first column of the reference or column offset
		  5
		  colLast
		  1
		  The last column of the reference or column offset
		  Only the low-order 14 bits of the grbitRwFirst and grbitRwLast fields store the row offsets of the reference. The 2 MSBs of each field specify whether the row and column offset are relative or absolute. The following table shows the bit structure of the grbitRwFirst and grbitRwLast fields.
		  Bits
		  Mask
		  Name
		  Contents
		  15
		  8000h
		  fRwRel
		  =1 if the row offset is relative =0 otherwise
		  14
		  4000h
		  fColRel
		  =1 if the column offset is relative =0 otherwise
		  13�0
		  3FFFh
		  rw
		  The row number or row offset (0-based)
		  The only difference between ptgAreaN and ptgArea is in the way relative references are stored.
ptgNameX: Name or External Name (Operand, ptg=39h) � BIFF8
		  This ptg stores the index to a name.
		  Offset
		  Name
		  Size
		  Contents
		  0
		  ixti
		  2
		  Index into the EXTERNSHEET record
		  2
		  ilbl
		  2
		  The index to the NAME or EXTERNNAME table (1-based)
		  4
		  (Reserved)
		  2
		  Reserved; must be 0 (zero)
ptgNameX: Name or External Name (Operand, ptg=39h) � BIFF7 and earlier
		  This ptg stores the index to a name. If the name is in the current workbook (in which case ixals is negative), the ilbl field is a 1-based index to the table of NAME records. If the name is in another workbook (that is, if it is an external name), the ilbl field is a 1-based index to the table of EXTERNNAME records.
		  Offset
		  Name
		  Size
		  Contents
		  0
		  ixals
		  2
		  The index to the EXTERNSHEET records. If ixals is negative (for example, FFFFh), the name is in the current workbook.
		  2
		  (Reserved)
		  8
		  10
		  ilbl
		  2
		  The index to the NAME or EXTERNNAME table (1-based).
		  12
		  (Reserved)
		  12
ptgRef3d: 3-D Cell Reference (Operand, ptg=3Ah) � BIFF8
		  This ptg stores a 3-D cell reference (for example, Sheet1:Sheet3!$A$1).
		  Offset
		  Name
		  Size
		  Contents
		  0
		  ixti
		  2
		  Index into the EXTERNSHEET record.
		  2
		  rw
		  2
		  The row of the reference, or the row offset.
		  4
		  grbitCol
		  2
		  (See the following table.)
		  Only the low-order 8 bits of the grbitCol field store the column number of the reference. The 2 MSBs specify whether the row and column references are relative or absolute. The following table shows the bit structure of the grbitCol field.
		  Bits
		  Mask
		  Name
		  Contents
		  15
		  8000h
		  fRwRel
		  =1 if the row offset is relative =0 otherwise
		  14
		  4000h
		  fColRel
		  =1 if the column offset is relative =0 otherwise
		  13 � 8
		  3F00h
		  (Reserved)
		  7 � 0
		  00FFh
		  col
		  The column number or column offset (0-based)
ptgRef3d: 3-D Cell Reference (Operand, ptg=3Ah) � BIFF7 and earlier
		  This ptg stores a 3-D cell reference (for example, Sheet1:Sheet3!$A$1). If the reference is to another workbook (in which case ixals is positive), itabFirst is not used (it will be 0000h), and itabLast is the ixals for the last sheet in the 3-D reference. If either itabFirst or itabLast is equal to FFFFh, that sheet is a deleted sheet.
		  Offset
		  Name
		  Size
		  Contents
		  0
		  ixals
		  2
		  The index to the EXTERNSHEET records. If ixals is negative (for example, FFFFh), the reference is in the current workbook.
		  2
		  (Reserved)
		  8
		  Offset
		  Name
		  Size
		  Contents
		  10
		  itabFirst
		  2
		  The index to the first sheet in the 3-D reference (0-based); see the text.
		  12
		  itabLast
		  2
		  The index to the last sheet in the 3-D reference (0-based); see the text.
		  14
		  grbitRw
		  2
		  (See the following table.)
		  16
		  col
		  1
		  The column of the reference, or the column offset.
		  Only the low-order 14 bits of the grbitRw field store the row number of the reference. The 2 MSBs specify whether the row and column references are relative or absolute. The following table shows the bit structure of the grbitRw field.
		  Bits
		  Mask
		  Name
		  Contents
		  15
		  8000h
		  fRwRel
		  =1 if the row offset is relative =0 otherwise
		  14
		  4000h
		  fColRel
		  =1 if the column offset is relative =0 otherwise
		  13�0
		  3FFFh
		  rw
		  The row number or row offset (0-based)
ptgArea3d: 3-D Area Reference (Operand, ptg=3Bh) � BIFF8
		   This ptg stores a 3-D area reference (for example, Sheet1:Sheet3!A1:E9).
		   Offset
		   Name
		   Size
		   Contents
		   0
		   ixti
		   2
		   Index into the EXTERNSHEET record.
		   2
		   rwFirst
		   2
		   The first row in the area.
		   4
		   rwLast
		   2
		   The last row in the area.
		   6
		   grbitColFirst
		   2
		   The first column of the reference, or the column offset; see following table.
		   8
		   grbitColLast
		   2
		   The last column of the reference, or the column offset; see following table.
		   Only the low-order 8 bits of the grbitColFirst and grbitColLast fields store the column number of the reference. The 2 MSBs specify whether the row and column references are relative or absolute. The following table shows the bit structure of the grbitCol field.
		   Bits
		   Mask
		   Name
		   Contents
		   15
		   8000h
		   fRwRel
		   =1 if the row offset is relative =0 otherwise
		   14
		   4000h
		   fColRel
		   =1 if the column offset is relative =0 otherwise
		   13�8
		   3F00h
		   (Reserved)
		   7�0
		   00FFh
		   col
		   The column number or column offset (0-based)
ptgArea3d: 3-D Area Reference (Operand, ptg=3Bh) � BIFF7 and earlier
		   This ptg stores a 3-D area reference (for example, Sheet1:Sheet3!A1:E9).
		   Offset
		   Name
		   Size
		   Contents
		   0
		   ixals
		   2
		   The index to the EXTERNSHEET records. If ixals is negative (for example, FFFFh), the reference is on another sheet in the same workbook.
		   2
		   (Reserved)
		   8
		   10
		   itabFirst
		   2
		   The index to the first sheet in the 3-D reference (0-based).
		   12
		   itabLast
		   2
		   The index to the last sheet in the 3-D reference (0-based).
		   14
		   grbitRwFirst
		   2
		   The first row in the area; see the following table.
		   16
		   grbitRwLast
		   2
		   The last row in the area; see the following table.
		   18
		   colFirst
		   1
		   The first column of the reference, or the column offset.
		   19
		   colLast
		   1
		   The last column of the reference, or the column offset.
		   Only the low-order 14 bits of the grbitRwFirst and grbitRwLast fields store the row offsets of the reference. The 2 MSBs of each field specify whether the row and column offset are relative or absolute. The following table shows the bit structure of the grbitRwFirst and grbitRwLast fields.
		   Bits
		   Mask
		   Name
		   Contents
		   15
		   8000h
		   fRwRel
		   =1 if the row offset is relative =0 otherwise
		   14
		   4000h
		   fColRel
		   =1 if the column offset is relative =0 otherwise
		   13�0
		   3FFFh
		   rw
		   The row number or row offset (0-based)
ptgRefErr3d: Deleted 3-D Cell Reference (Operand, ptg=3Ch)
			 This ptg stores a 3-D cell reference adjusted to #REF! as a result of worksheet editing (such as cutting, pasting, and deleting). The ptgRefErr3d is identical to ptgRef3d.
ptgAreaErr3d: Deleted 3-D Area Reference (Operand, ptg=3Dh)
			  This ptg stores a 3-D area reference adjusted to #REF! as a result of worksheet editing (such as cutting, pasting, and deleting). The ptgAreaErr3d is identical to ptgArea3d.
			  Control Tokens
ptgExp: Array Formula or Shared Formula (ptg=01h)
		This ptg indicates an array formula or a shared formula. When ptgExp occurs in a formula, it is the only token in the formula. This indicates the cell containing the formula is part of an array or part of a shared formula. The actual formula is found in an ARRAY record.
		The token value for ptgExp consists of the row and column of the upper-left corner of the array formula.
		Offset
		Name
		Size
		Contents
		0
		rwFirst
		2
		The row number of the upper-left corner
		2
		colFirst
		2
		The column number of the upper-left corner
ptgTbl: Data Table (ptg=02h)
		This ptg indicates a data table. When ptgTbl occurs in a formula, it is the only token in the formula. This indicates the cell containing the formula is an interior cell in a data table; the table description is found in a TABLE record. Rows and columns that contain input values to be substituted in the table do not contain ptgTbl. The token value for ptgTbl consists of the row and column of the upper-left corner of the table�s interior.
		Offset
		Name
		Size
		Contents
		0
		rwFirst
		2
		The row number of the upper-left corner
		2
		colFirst
		2
		The column number of the upper-left corner
ptgParen: Parenthesis (ptg=15h)
		  This ptg is used only when Excel unparses a parsed expression (for example, to display it in the formula bar). This ptg is not used to evaluate parsed expressions. It indicates that the previous token in the parsed expression should be in parentheses. If the previous token is an operand, only that operand is in parentheses. If the previous token is an operator, the operator and all of its operands are in parentheses. For example, the formula =1+(2) is stored as follows: ptgInt 0001h ptgInt 0002h ptgParen ptgAdd In this case, only the integer operand 2 is in parentheses. The formula =(1+2) is stored as follows: ptgInt 0001h ptgInt 0002h ptgAdd ptgParen In this example, the parenthesized quantity consists of the ptgAdd operator and both of its operands.
ptgAttr: Special Attribute (ptg=19h)
		 This ptg is used for several different purposes. In all cases, the token value consists of a group of flag bits and a data word. BIFF3 and BIFF4
		 Offset
		 Name
		 Size
		 Contents
		 0
		 grbit
		 1
		 Option flags
		 1
		 w
		 2
		 Data word
		 BIFF4 when bifFAttrSpace = 1
		 Offset
		 Name
		 Size
		 Contents
		 0
		 grbit
		 1
		 Option flags
		 1
		 bAttrSpace
		 1
		 Spacing attribute
		 2
		 bSpace
		 1
		 Number of spaces
		 The grbit field contains the following option flags:
		 Bits
			 Mask
			 Name
			 Contents
			 0
			 01h
			 bitFAttrSemi
			 =1 if the formula contains a volatile function
			 1
			 02h
			 bitFAttrIf
			 =1 to implement an optimized IF function
			 2
			 04h
			 bitFAttrChoose
			 =1 to implement an optimized CHOOSE function
			 3
			 08h
			 bitFAttrGoto
			 =1 to jump to another location within the parsed expression
			 4
			 10h
			 bitFAttrSum
			 =1 to implement an optimized SUM function
			 5
			 20h
			 bitFAttrBaxcel
			 =1 if the formula is a BASIC-style assignment statement
			 6
			 40h
			 bifFAttrSpace
			 =1 if the macro formula contains spaces after the equal sign (BIFF3 and BIFF4 only)
			 7
			 80
			 (unused)
			 ptgAttr requires special handling when parsed expressions are scanned. For more information, see ?Scanning a Parsed Expression?.
			 bitFAttrSemi
			 Set to 1 if the formula contains a volatile function � that is, a function that is calculated in every recalculation. If ptgAttr is used to indicate a volatile function, it must be the first token in the parsed expression. If grbit=bitFAttrSemi, then the b (or w) field is don�t-care.
			 bitFAttrIf
			 Indicates an optimized IF function. An IF function contains three parts: a condition, a TRUE subexpression, and a FALSE subexpression. The syntax of an associated Excel formula would be IF(condition, TRUE subexpression, FALSE subexpression). bitFAttrIf immediately follows the condition portion of the parsed expression. The b (or w) field specifies the offset to the FALSE subexpression; the TRUE subexpression is found immediately following the ptgAttr token. At the end of the TRUE subexpression, there is a bitFAttrGoto token that causes a jump to beyond the FALSE subexpression. In this way, Excel evaluates only the correct subexpression instead of evaluating both of them and discarding the wrong one. The FALSE subexpression is optional in Excel. If it is missing, the b (or w) field specifies an offset to beyond the TRUE subexpression.
			 bitFAttrChoose
			 Indicates an optimized CHOOSE function. The cCases (or wCases) field specifies the number of cases in the CHOOSE function. It is followed by an array of word offsets to those cases. The format of this complex token value is shown in the following table.
			 Offset
			 Name
			 Size
			 Contents
			 0
			 grbit
			 1
			 bitFAttrChoose (04h).
			 1
			 wCases
			 2
			 The number of cases in the CHOOSE function.
			 3
			 rgw
			 var
			 A sequence of word offsets to the CHOOSE cases. The number of words in this field is equal to wCases+1.
			 bitFAttrGoto
			 Instructs the expression evaluator to skip part of the parsed expression during evaluation. The b (or w) field specifies the number of bytes (or words) to skip, minus 1.
			 bitFAttrSum
			 Indicates an optimized SUM function (a SUM that has a single argument). For example, the sum of the cells in a 3-D reference � which has the formula =SUM(Sheet1:Sheet3!C11) � generates a ptgAttr with bitFAttrSum TRUE. The b (or w) field is don�t-care.
			 bifFAttrSpace
			 Indicates a formula (macro sheet or worksheet) contains spaces or carriage returns. Excel retains spaces and returns in macro sheet and worksheet formulas (in version 3.0 and earlier, spaces and returns would have been eliminated when the formula was parsed). The bAttrSpace field contains an attribute code, and the bSpace field contains the number of spaces or returns. The attribute codes are listed in the following table.
			 Attribute
			 Value
			 bitFSpace
			 00h
			 bitFEnter
			 01h
			 bitFPreSpace
			 02h
			 bitFPreEnter
			 03h
			 bitFPostSpace
			 04h
			 bitFPostEnter
			 05h
			 bitFPreFmlaSpace
			 06h
			 The bitFSpace and bitFEnter attributes indicate that bSpace contains the number of spaces or returns before the next ptg in the formula. The bitFPreSpace, bitFPreEnter, bitFPostSpace, and bitFPostEnter attributes occur with a ptgParen. Because one ptgParen represents two matched parentheses, the ptgAttr must encode the position of the space or return if it occurs before either parenthesis. For example, the ptgs that express the worksheet formula = ("spaces" ), which contains four spaces before the opening and closing parentheses, would appear in a formula record as shown in the following table.
			 Hex dump
			 Ptg type
			 Decodes to
			 17 06 73 70 61 63 65 73
			 ptgStr
			 The string ?spaces? (operand)
			 19 40 02 04
			 ptgAttr
			 Four spaces before the opening parenthesis
			 19 40 04 04
			 ptgAttr
			 Four spaces before the closing parenthesis
			 Hex dump
			 Ptg type
			 Decodes to
			 15
			 ptgParen
			 The enclose operand (ptgStr) in parentheses
			 The bitFPreFmlaSpace attribute provides compatibility with BIFF3, where spaces can occur only after the equal sign (before the formula) in macro formulas. If the spaces in a BIFF5/BIFF7 formula are also acceptable in a BIFF3 formula, Excel writes a bitFPreFmlaSpace attribute to indicate as much.
ptgMemNoMem: Incomplete Constant Reference Subexpression (ptg=28h)
			 This ptg is closely related to ptgMemArea. It is used to indicate a constant reference subexpression that could not be pre-evaluated because of insufficient memory. The token value consists of the length of the reference subexpression, as shown in the following table.
			 Offset
			 Name
			 Size
			 Contents
			 0
			 (Reserved)
			 4
			 4
			 cce
			 2
			 The length of the reference subexpression
ptgMemFunc: Variable Reference Subexpression (ptg=29h)
			This ptg indicates a reference subexpression that does not evaluate to a constant reference. Any reference subexpression that contains one or more of the following items generates a ptgMemFunc.
			Subexpression contains
			Example
			A function
			OFFSET(ACTIVE.CELL(),1,1):$C$2
			A name
			INDEX(first_cell:$D$2,1,1)
			An external reference
			SALES.XLS!$A$1:SALES.XLS!$C$3
			The token value consists of the length of the reference subexpression.
			Offset
			Name
			Size
			Contents
			0
			cce
			2
			The length of the reference subexpression
ptgMemAreaN: Reference Subexpression Within a Name (ptg=2Eh)
			 This ptg contains a constant reference subexpression within a name definition. Unlike ptgMemArea, ptgMemAreaN is not used to pre-evaluate the reference subexpression. The token value consists of the length of the reference subexpression.
			 Offset
			 Name
			 Size
			 Contents
			 0
			 cce
			 2
			 The length of the reference subexpression
ptgMemNoMemN: Incomplete Reference Subexpression Within a Name (ptg=2Fh)
			  This ptg is closely related to ptgMemAreaN. It is used to indicate a constant reference subexpression within a name that could not be evaluated because of insufficient memory.
			  The token value consists of the length of the reference subexpression, as shown in the following table.
			  Offset
			  Name
			  Size
			  Contents
			  0
			  cce
			  2
			  The length of the reference subexpression
			  Function Operators
			  The following paragraphs describe the function operator ptgs. All of these operators pop arguments from the operand stack, compute a function, and then push the result back onto the operand stack. The number of operands popped from the stack is equal to the number of arguments passed to the Excel function. Some Excel functions always require a fixed number of arguments, whereas others accept a variable number of arguments. The SUM function, for example, accepts a variable number of arguments. Although they�re operators, function tokens also behave like operands in that they can occur in any of the three ptg classes: reference, value, or array.
ptgFunc: Function, Fixed Number of Arguments (Operator, ptg=21h)
		 This ptg indicates an Excel function with a fixed number of arguments. The ptgFunc is followed by the index to the function table.
		 Offset
		 Name
		 Size
		 Contents
		 0
		 iftab
		 2
		 The index to the function table;
ptgFuncVar: Function, Variable Number of Arguments (Operator, ptg=22h)
				This ptg indicates an Excel function with a variable number of arguments. The ptgFuncVar is followed by the number of arguments (1 byte) and then the index to the function table (2 bytes).
				Offset
				Bits
				Mask
				Name
				Contents
				0
				6�0
				7Fh
				cargs
				The number of arguments to the function.
				7
				80h
				fPrompt
				=1, function prompts the user (macro functions that end with a question mark).
				1
				14�0
				7FFFh
				iftab
				The index to the function table;
			15
				8000h
				fCE
				The function is a command-equivalent.
*/






expression_node_t::expression_node_t(CGlobalRecords& gRecords)
{

}
expression_node_t::~expression_node_t()
{

}

expression_node_t* expression_node_t::GetChild(unsigned16_t index) const 
{
	return NULL;
}
unsigned16_t expression_node_t::GetNumberOfChilds(void) const 
{
	return 0;
}

void expression_node_t::DestroyAST(void)
{
	// first destroy the children, bottom-up recursively.
	unsigned16_t chn = GetNumberOfChilds();
	while (chn-- > 0)
	{
		expression_node_t* ch = GetChild(chn);

		XL_ASSERT(ch);
		if (ch)
		{
			ch->DestroyAST();
		}
	}

	// then destroy this instance itself
	delete this;
}


void expression_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	XL_ASSERT(!"Should never get here! MUST be handled by the derived class' method of similar name!");

	// do nothing: propagate the currently set expected value. 
}

size_t expression_node_t::GetSize(bool include_subtree) const
{
	XL_ASSERT(!"Should never get here! MUST be handled by the derived class' method of similar name!");
	return 0;
}
signed8_t expression_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	XL_ASSERT(!"Should never get here! MUST be handled by the derived class' method of similar name!");
	return NO_ERRORS;
}



terminal_node_t::terminal_node_t(CGlobalRecords& gRecords) :
		expression_node_t(gRecords)
{

}
terminal_node_t::~terminal_node_t()
{

}


boolean_value_node_t::boolean_value_node_t(CGlobalRecords& gRecords, bool v) :
		terminal_node_t(gRecords),
		value(v)
{

}
boolean_value_node_t::~boolean_value_node_t()
{

}

void boolean_value_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	dst.SetBoolean(value);
}

size_t boolean_value_node_t::GetSize(bool include_subtree) const
{
	return 2;
}
signed8_t boolean_value_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode = NO_ERRORS;

	errcode |= dst.AddValue8(OP_BOOL);
	errcode |= dst.AddValue8((unsigned8_t)!!value);

	return errcode;
}


integer_value_node_t::integer_value_node_t(CGlobalRecords& gRecords, signed32_t v) :
	  terminal_node_t(gRecords),
		value(v)
	  {

	  }
integer_value_node_t::~integer_value_node_t()
	{

	}

void integer_value_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	dst.SetInteger(value);
}

size_t integer_value_node_t::GetSize(bool include_subtree) const
{
	if (value >= 0 && value <= 65535) 
		return 2+1; // ptgInt
	return 8+1; // ptgNum
}
signed8_t integer_value_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode = NO_ERRORS;

	if (value >= 0 && value <= 65535) 
	{
		errcode |= dst.AddValue8(OP_INT);
		errcode |= dst.AddValue16((unsigned16_t)value);
	}
	else
	{
		errcode |= dst.AddValue8(OP_NUM);
		errcode |= dst.AddValue64((unsigned64_t)(double)value);
	}

	return errcode;
}


float_value_node_t::float_value_node_t(CGlobalRecords& gRecords, double v) :
        terminal_node_t(gRecords),
        value(v)
{
}
float_value_node_t::~float_value_node_t()
{
}

void float_value_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	dst.SetFloatingPoint(value);
}

size_t float_value_node_t::GetSize(bool include_subtree) const
{
	return 8+1;
}
signed8_t float_value_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode = NO_ERRORS;

	errcode |= dst.AddValue8(OP_NUM);
	errcode |= dst.AddValue64((unsigned64_t)value);

	return errcode;
}




error_value_node_t::error_value_node_t(CGlobalRecords& gRecords, errcode_t v) :
		terminal_node_t(gRecords),
			value(v)
{

}
error_value_node_t::~error_value_node_t()
{

}

void error_value_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	dst.SetErrorCode(value);
}

size_t error_value_node_t::GetSize(bool include_subtree) const
{
	return 1+1;
}
signed8_t error_value_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode = NO_ERRORS;

	errcode |= dst.AddValue8(OP_ERR);
	errcode |= dst.AddValue8((unsigned8_t)value);

	return errcode;
}




missing_arg_node_t::missing_arg_node_t(CGlobalRecords& gRecords) :
terminal_node_t(gRecords)
{

}
missing_arg_node_t::~missing_arg_node_t()
{

}

void missing_arg_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	dst.SetErrorCode(XLERR_N_A);
}

size_t missing_arg_node_t::GetSize(bool include_subtree) const
{
	return 1;
}
signed8_t missing_arg_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode;

	errcode = dst.AddValue8(OP_MISSARG);

	return errcode;
}



text_value_node_t::text_value_node_t(CGlobalRecords& gRecords, const std::string& v) :
	terminal_node_t(gRecords),
	m_GlobalRecords(gRecords)
{
	m_GlobalRecords.char2str16(v, this->value);
}
text_value_node_t::text_value_node_t(CGlobalRecords& gRecords, const std::ustring& v) :
terminal_node_t(gRecords),
m_GlobalRecords(gRecords)
{
	m_GlobalRecords.wide2str16(v, this->value);
}
text_value_node_t::text_value_node_t(CGlobalRecords& gRecords, const u16string& v) :
terminal_node_t(gRecords),
value(v),
m_GlobalRecords(gRecords)
{
}
text_value_node_t::~text_value_node_t()
{

}

text_value_node_t& text_value_node_t::operator =(const text_value_node_t &src)
{
	throw std::string("Should never have invoked the text_value_node_t copy operator!");
}

void text_value_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	dst.SetText(value);
}

size_t text_value_node_t::GetSize(bool include_subtree) const
{
	return 1 + 1 + CGlobalRecords::IsASCII(value) * value.length() + value.length();
}
signed8_t text_value_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode;

	errcode = dst.AddValue8(OP_STR);
	// TODO: clip string to 255 chars max!
	errcode |= dst.AddUnicodeString(m_GlobalRecords, value, CUnit::LEN1_FLAGS_UNICODE);

	return errcode;
}


cell_deref_node_t::cell_deref_node_t(CGlobalRecords& gRecords, const cell_t& v, cell_addr_mode_t opt, cell_op_class_t opclass) :
		terminal_node_t(gRecords),
		value(&v),
		worksheet_ref(NULL),
		attr(opt),
		operand_class(opclass)
{

}
cell_deref_node_t::cell_deref_node_t(CGlobalRecords& gRecords, const cell_t& v, const worksheet* ws, cell_addr_mode_t opt, cell_op_class_t opclass) :
		terminal_node_t(gRecords),
		value(&v),
		worksheet_ref(ws),
		attr(opt),
		operand_class(opclass)
{

}
cell_deref_node_t::~cell_deref_node_t()
{

}

void cell_deref_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	/*
	We don't want to spend the effort to 'know' (~ make this code aware) which cell reference produces what result, neither in value nor in type, so we
	fake it and make it easy to ourselves: we 'guestimate' the cell/reference will return an
	error code and we mark the expression as 'calc on load' to mask our ignorance.
	*/
	dst.SetCalcOnLoad();
	dst.SetErrorCode(XLERR_VALUE);
}

size_t cell_deref_node_t::GetSize(bool include_subtree) const
{
	return 1+2+2;
}
signed8_t cell_deref_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode;

	errcode = dst.AddValue8(OP_REF | operand_class); // OP_REF, OP_REFV, OP_REFA

	// BIFF8 format!
	errcode |= dst.AddValue16((unsigned16_t)value->GetRow());

	unsigned16_t col = (unsigned16_t)value->GetCol() & 0x3FFF;

	XL_ASSERT((attr & ~0xC000) == 0);
	col |= ((unsigned16_t)attr) & 0xC000;
	errcode |= dst.AddValue16(col);

	return errcode;
}





cellarea_deref_node_t::cellarea_deref_node_t(CGlobalRecords& gRecords, const cell_t& u_l_c, const cell_t& l_r_c, cell_addr_mode_t attr, cell_op_class_t opclass) :
cell_deref_node_t(gRecords, u_l_c, attr, opclass),
lower_right_corner(&l_r_c)
{

}
cellarea_deref_node_t::cellarea_deref_node_t(CGlobalRecords& gRecords, const cell_t& u_l_c, const cell_t& l_r_c, const worksheet* ws, cell_addr_mode_t attr, cell_op_class_t opclass) :
cell_deref_node_t(gRecords, u_l_c, ws, attr, opclass),
lower_right_corner(&l_r_c)
{

}
cellarea_deref_node_t::~cellarea_deref_node_t()
{

}

size_t cellarea_deref_node_t::GetSize(bool include_subtree) const
{
	return 1+2*(2+2);
}
signed8_t cellarea_deref_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode;

	errcode = dst.AddValue8(OP_AREA | operand_class); // OP_AREA. OP_AREAV, OP_AREAA

	// BIFF8 format!
	errcode |= dst.AddValue16((unsigned16_t)upper_left_corner()->GetRow());
	errcode |= dst.AddValue16((unsigned16_t)lower_right_corner->GetRow());

	unsigned16_t col = (unsigned16_t)upper_left_corner()->GetCol() & 0x3FFF;

	XL_ASSERT((attr & ~0xC000) == 0);
	col |= ((unsigned16_t)attr) & 0xC000;
	errcode |= dst.AddValue16(col);

	col = (unsigned16_t)lower_right_corner->GetCol() & 0x3FFF;

	XL_ASSERT((attr & ~0xC000) == 0);
	col |= ((unsigned16_t)attr) & 0xC000;
	errcode |= dst.AddValue16(col);

	return errcode;
}

void cellarea_deref_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	/*
	We don't want to spend the effort to 'know' (~ make this code aware) which cell reference produces what result, neither in value nor in type, so we
	fake it and make it easy to ourselves: we 'guestimate' the cell/reference will return an
	error code and we mark the expression as 'calc on load' to mask our ignorance.
	*/
	dst.SetCalcOnLoad();
	dst.SetErrorCode(XLERR_VALUE);
}






operator_basenode_t::operator_basenode_t(CGlobalRecords& gRecords, expr_operator_code_t o) :
	expression_node_t(gRecords),
		op(o)
	{

	}
operator_basenode_t::~operator_basenode_t()
{

}

void operator_basenode_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	/*
	We don't want to spend the effort to 'know' (~ make this code aware) which operation produces what result, neither in value nor in type, so we
	fake it and make it easy to ourselves: we 'guestimate' the operation will return an
	error code and we mark the expression as 'calc on load' to mask our ignorance.

	Remark: there's just a few operations we care about: those we are willing to take to the dance.
	*/
	dst.SetCalcOnLoad();
	dst.SetErrorCode(XLERR_VALUE);
}



unary_op_node_t::unary_op_node_t(CGlobalRecords& gRecords, expr_operator_code_t op, expression_node_t* a) :
operator_basenode_t(gRecords, op),
arg(a)
{
	XL_ASSERT(a);

	switch (op)
	{
	default:
		throw std::string("Not a valid unary operator");

	case OP_UPLUS: // Unary Plus 
	case OP_UMINUS: // Unary Minus 
	case OP_PERCENT: // Percent Sign 
		// okay
		break;
	}
}
unary_op_node_t::~unary_op_node_t()
{

}

expression_node_t* unary_op_node_t::GetChild(unsigned16_t index) const
{
	if (index == 0)
		return arg;
	return NULL;
}
unsigned16_t unary_op_node_t::GetNumberOfChilds(void) const
{
	return 1;
}

size_t unary_op_node_t::GetSize(bool include_subtree) const
{
	size_t len = 1;

	if (include_subtree)
	{
		XL_ASSERT(GetChild(0));
		len += GetChild(0)->GetSize(include_subtree);
	}
	return len;
}
signed8_t unary_op_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode = NO_ERRORS;

	if (include_subtree)
	{
		XL_ASSERT(GetChild(0));
		errcode |= GetChild(0)->DumpData(dst, include_subtree);
	}

	errcode |= dst.AddValue8(op);

#ifdef XL_WITH_ASSERTIONS
	switch (op)
	{
	default:
		XL_ASSERT(!"Should never get here!");

	case OP_UPLUS: // Unary Plus 
	case OP_UMINUS: // Unary Minus 
	case OP_PERCENT: // Percent Sign 
		// okay
		break;
	}
#endif

	return errcode;
}




binary_op_node_t::binary_op_node_t(CGlobalRecords& gRecords, expr_operator_code_t op, expression_node_t* arg1, expression_node_t* arg2) :
operator_basenode_t(gRecords, op)
{
	args[0] = arg1;
	args[1] = arg2;

	switch (op)
	{
	default:
		throw std::string("Not a valid binary operator");

	case OP_ADD: 
	case OP_SUB: 
	case OP_MUL: 
	case OP_DIV: 
	case OP_POWER: 
	case OP_CONCAT: 
	case OP_LE: 
	case OP_LT: 
	case OP_EQ: 
	case OP_GE: 
	case OP_GT: 
	case OP_NE: 
	case OP_ISECT:  // intersection ~ Excel 'space' operator
	case OP_UNION:  // union ~ Excel 'comma' operator
	case OP_RANGE:  // minimal bounding rectangle ~ Excel 'colon' operator
		// okay
		break;
	}
}
binary_op_node_t::~binary_op_node_t()
{

}

expression_node_t* binary_op_node_t::GetChild(unsigned16_t index) const
{
	if (index <= 1)
		return args[index];
	return NULL;
}
unsigned16_t binary_op_node_t::GetNumberOfChilds(void) const
{
	return 2;
}

size_t binary_op_node_t::GetSize(bool include_subtree) const
{
	size_t len = 1;

	if (include_subtree)
	{
		XL_ASSERT(GetChild(0));
		len += GetChild(0)->GetSize(include_subtree);
		XL_ASSERT(GetChild(1));
		len += GetChild(1)->GetSize(include_subtree);
	}
	return len;
}
signed8_t binary_op_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode = NO_ERRORS;

	if (include_subtree)
	{
		XL_ASSERT(GetChild(0));
		errcode |= GetChild(0)->DumpData(dst, include_subtree);
		XL_ASSERT(GetChild(1));
		errcode |= GetChild(1)->DumpData(dst, include_subtree);
	}

	errcode |= dst.AddValue8(op);

#ifdef XL_WITH_ASSERTIONS
	switch (op)
	{
	default:
		XL_ASSERT(!"Not a valid binary operator");

	case OP_ADD: 
	case OP_SUB: 
	case OP_MUL: 
	case OP_DIV: 
	case OP_POWER: 
	case OP_CONCAT: 
	case OP_LE: 
	case OP_LT: 
	case OP_EQ: 
	case OP_GE: 
	case OP_GT: 
	case OP_NE: 
	case OP_ISECT:  // intersection ~ Excel 'space' operator
	case OP_UNION:  // union ~ Excel 'comma' operator
	case OP_RANGE:  // minimal bounding rectangle ~ Excel 'colon' operator
		// okay
		break;
	}
#endif

	return errcode;
}







int xlslib_core::KnownNumberOfArgsForExcelFunction(expr_function_code_t func)
{
	switch (func)
	{
	default:
		XL_ASSERT(!"Shouldn't get here!");
		break;

	case FUNC_UDF :                      break;
	case FUNC_COUNT :                    break;
	case FUNC_ISNA :                     return 1;
	case FUNC_ISERROR :                  return 1;
	case FUNC_SUM :                      break;
	case FUNC_AVERAGE :                  break;
	case FUNC_MIN :                      break;
	case FUNC_MAX :                      break;
	case FUNC_ROW :                      break;
	case FUNC_COLUMN :                   break;
	case FUNC_NA :                       break;
	case FUNC_NPV :                      break;
	case FUNC_STDEV :                    break;
	case FUNC_DOLLAR :                   break;
	case FUNC_FIXED :                    break;
	case FUNC_SIN :                      return 1;
	case FUNC_COS :                      return 1;
	case FUNC_TAN :                      return 1;
	case FUNC_ATAN :                     break;
	case FUNC_PI :                       break;
	case FUNC_SQRT :                     return 1;
	case FUNC_EXP :                      return 1;
	case FUNC_LN :                       return 1;
	case FUNC_LOG10 :                    return 1;
	case FUNC_ABS :                      return 1;
	case FUNC_INT :                      return 1;
	case FUNC_SIGN :                     return 1;
	case FUNC_ROUND :                    break;
	case FUNC_LOOKUP :                   break;
	case FUNC_INDEX :                    break;
	case FUNC_REPT :                     break;
	case FUNC_MID :                      break;
	case FUNC_LEN :                      break;
	case FUNC_VALUE :                    break;
	case FUNC_TRUE :                     break;
	case FUNC_FALSE :                    break;
	case FUNC_AND :                      break;
	case FUNC_OR :                       break;
	case FUNC_NOT :                      break;
	case FUNC_MOD :                      break;
	case FUNC_DCOUNT :                   break;
	case FUNC_DSUM :                     break;
	case FUNC_DAVERAGE :                 break;
	case FUNC_DMIN :                     break;
	case FUNC_DMAX :                     break;
	case FUNC_DSTDEV :                   break;
	case FUNC_VAR :                      break;
	case FUNC_DVAR :                     break;
	case FUNC_TEXT :                     break;
	case FUNC_LINEST :                   break;
	case FUNC_TREND :                    break;
	case FUNC_LOGEST :                   break;
	case FUNC_GROWTH :                   break;
	case FUNC_GOTO :                     break;
	case FUNC_HALT :                     break;
	case FUNC_PV :                       break;
	case FUNC_FV :                       break;
	case FUNC_NPER :                     break;
	case FUNC_PMT :                      break;
	case FUNC_RATE :                     break;
	case FUNC_MIRR :                     break;
	case FUNC_IRR :                      break;
	case FUNC_RAND :                     break;
	case FUNC_MATCH :                    break;
	case FUNC_DATE :                     break;
	case FUNC_TIME :                     break;
	case FUNC_DAY :                      break;
	case FUNC_MONTH :                    break;
	case FUNC_YEAR :                     break;
	case FUNC_WEEKDAY :                  break;
	case FUNC_HOUR :                     break;
	case FUNC_MINUTE :                   break;
	case FUNC_SECOND :                   break;
	case FUNC_NOW :                      break;
	case FUNC_AREAS :                    break;
	case FUNC_ROWS :                     break;
	case FUNC_COLUMNS :                  break;
	case FUNC_OFFSET :                   break;
	case FUNC_ABSREF :                   break;
	case FUNC_RELREF :                   break;
	case FUNC_ARGUMENT :                 break;
	case FUNC_SEARCH :                   break;
	case FUNC_TRANSPOSE :                break;
	case FUNC_ERROR :                    break;
	case FUNC_STEP :                     break;
	case FUNC_TYPE :                     break;
	case FUNC_ECHO :                     break;
	case FUNC_SETNAME :                  break;
	case FUNC_CALLER :                   break;
	case FUNC_DEREF :                    break;
	case FUNC_WINDOWS :                  break;
	case FUNC_SERIES :                   break;
	case FUNC_DOCUMENTS :                break;
	case FUNC_ACTIVECELL :               break;
	case FUNC_SELECTION :                break;
	case FUNC_RESULT :                   break;
	case FUNC_ATAN2 :                    break;
	case FUNC_ASIN :                     return 1;
	case FUNC_ACOS :                     return 1;
	case FUNC_CHOOSE :                   break;
	case FUNC_HLOOKUP :                  break;
	case FUNC_VLOOKUP :                  break;
	case FUNC_LINKS :                    break;
	case FUNC_INPUT :                    break;
	case FUNC_ISREF :                    break;
	case FUNC_GETFORMULA :               break;
	case FUNC_GETNAME :                  break;
	case FUNC_SETVALUE :                 break;
	case FUNC_LOG :                      break;
	case FUNC_EXEC :                     break;
	case FUNC_CHAR :                     break;
	case FUNC_LOWER :                    break;
	case FUNC_UPPER :                    break;
	case FUNC_PROPER :                   break;
	case FUNC_LEFT :                     break;
	case FUNC_RIGHT :                    break;
	case FUNC_EXACT :                    break;
	case FUNC_TRIM :                     break;
	case FUNC_REPLACE :                  break;
	case FUNC_SUBSTITUTE :               break;
	case FUNC_CODE :                     break;
	case FUNC_NAMES :                    break;
	case FUNC_DIRECTORY :                break;
	case FUNC_FIND :                     break;
	case FUNC_CELL :                     break;
	case FUNC_ISERR :                    return 1;
	case FUNC_ISTEXT :                   return 1;
	case FUNC_ISNUMBER :                 return 1;
	case FUNC_ISBLANK :                  return 1;
	case FUNC_T :                        break;
	case FUNC_N :                        break;
	case FUNC_FOPEN :                    break;
	case FUNC_FCLOSE :                   break;
	case FUNC_FSIZE :                    break;
	case FUNC_FREADLN :                  break;
	case FUNC_FREAD :                    break;
	case FUNC_FWRITELN :                 break;
	case FUNC_FWRITE :                   break;
	case FUNC_FPOS :                     break;
	case FUNC_DATEVALUE :                break;
	case FUNC_TIMEVALUE :                break;
	case FUNC_SLN :                      break;
	case FUNC_SYD :                      break;
	case FUNC_DDB :                      break;
	case FUNC_GETDEF :                   break;
	case FUNC_REFTEXT :                  break;
	case FUNC_TEXTREF :                  break;
	case FUNC_INDIRECT :                 break;
	case FUNC_REGISTER :                 break;
	case FUNC_CALL :                     break;
	case FUNC_ADDBAR :                   break;
	case FUNC_ADDMENU :                  break;
	case FUNC_ADDCOMMAND :               break;
	case FUNC_ENABLECOMMAND :            break;
	case FUNC_CHECKCOMMAND :             break;
	case FUNC_RENAMECOMMAND :            break;
	case FUNC_SHOWBAR :                  break;
	case FUNC_DELETEMENU :               break;
	case FUNC_DELETECOMMAND :            break;
	case FUNC_GETCHARTITEM :             break;
	case FUNC_DIALOGBOX :                break;
	case FUNC_CLEAN :                    break;
	case FUNC_MDETERM :                  break;
	case FUNC_MINVERSE :                 break;
	case FUNC_MMULT :                    break;
	case FUNC_FILES :                    break;
	case FUNC_IPMT :                     break;
	case FUNC_PPMT :                     break;
	case FUNC_COUNTA :                   break;
	case FUNC_CANCELKEY :                break;
	case FUNC_INITIATE :                 break;
	case FUNC_REQUEST :                  break;
	case FUNC_POKE :                     break;
	case FUNC_EXECUTE :                  break;
	case FUNC_TERMINATE :                break;
	case FUNC_RESTART :                  break;
	case FUNC_HELP :                     break;
	case FUNC_GETBAR :                   break;
	case FUNC_PRODUCT :                  break;
	case FUNC_FACT :                     break;
	case FUNC_GETCELL :                  break;
	case FUNC_GETWORKSPACE :             break;
	case FUNC_GETWINDOW :                break;
	case FUNC_GETDOCUMENT :              break;
	case FUNC_DPRODUCT :                 break;
	case FUNC_ISNONTEXT :                break;
	case FUNC_GETNOTE :                  break;
	case FUNC_NOTE :                     break;
	case FUNC_STDEVP :                   break;
	case FUNC_VARP :                     break;
	case FUNC_DSTDEVP :                  break;
	case FUNC_DVARP :                    break;
	case FUNC_TRUNC :                    break;
	case FUNC_ISLOGICAL :                return 1;
	case FUNC_DCOUNTA :                  break;
	case FUNC_DELETEBAR :                break;
	case FUNC_UNREGISTER :               break;
	case FUNC_USDOLLAR :                 break;
	case FUNC_FINDB :                    break;
	case FUNC_SEARCHB :                  break;
	case FUNC_REPLACEB :                 break;
	case FUNC_LEFTB :                    break;
	case FUNC_RIGHTB :                   break;
	case FUNC_MIDB :                     break;
	case FUNC_LENB :                     break;
	case FUNC_ROUNDUP :                  break;
	case FUNC_ROUNDDOWN :                break;
	case FUNC_ASC :                      break;
	case FUNC_DBCS :                     break;
	case FUNC_RANK :                     break;
	case FUNC_ADDRESS :                  break;
	case FUNC_DAYS360 :                  break;
	case FUNC_TODAY :                    break;
	case FUNC_VDB :                      break;
	case FUNC_MEDIAN :                   break;
	case FUNC_SUMPRODUCT :               break;
	case FUNC_SINH :                     return 1;
	case FUNC_COSH :                     return 1;
	case FUNC_TANH :                     return 1;
	case FUNC_ASINH :                    return 1;
	case FUNC_ACOSH :                    return 1;
	case FUNC_ATANH :                    break;
	case FUNC_DGET :                     break;
	case FUNC_CREATEOBJECT :             break;
	case FUNC_VOLATILE :                 break;
	case FUNC_LASTERROR :                break;
	case FUNC_CUSTOMUNDO :               break;
	case FUNC_CUSTOMREPEAT :             break;
	case FUNC_FORMULACONVERT :           break;
	case FUNC_GETLINKINFO :              break;
	case FUNC_TEXTBOX :                  break;
	case FUNC_INFO :                     break;
	case FUNC_GROUP :                    break;
	case FUNC_GETOBJECT :                break;
	case FUNC_DB :                       break;
	case FUNC_PAUSE :                    break;
	case FUNC_RESUME :                   break;
	case FUNC_FREQUENCY :                break;
	case FUNC_ADDTOOLBAR :               break;
	case FUNC_DELETETOOLBAR :            break;
	case FUNC_RESETTOOLBAR :             break;
	case FUNC_EVALUATE :                 break;
	case FUNC_GETTOOLBAR :               break;
	case FUNC_GETTOOL :                  break;
	case FUNC_SPELLINGCHECK :            break;
	case FUNC_ERRORTYPE :                return 1;
	case FUNC_APPTITLE :                 break;
	case FUNC_WINDOWTITLE :              break;
	case FUNC_SAVETOOLBAR :              break;
	case FUNC_ENABLETOOL :               break;
	case FUNC_PRESSTOOL :                break;
	case FUNC_REGISTERID :               break;
	case FUNC_GETWORKBOOK :              break;
	case FUNC_AVEDEV :                   break;
	case FUNC_BETADIST :                 break;
	case FUNC_GAMMALN :                  break;
	case FUNC_BETAINV :                  break;
	case FUNC_BINOMDIST :                break;
	case FUNC_CHIDIST :                  break;
	case FUNC_CHIINV :                   break;
	case FUNC_COMBIN :                   break;
	case FUNC_CONFIDENCE :               break;
	case FUNC_CRITBINOM :                break;
	case FUNC_EVEN :                     break;
	case FUNC_EXPONDIST :                break;
	case FUNC_FDIST :                    break;
	case FUNC_FINV :                     break;
	case FUNC_FISHER :                   break;
	case FUNC_FISHERINV :                break;
	case FUNC_FLOOR :                    return 1;
	case FUNC_GAMMADIST :                break;
	case FUNC_GAMMAINV :                 break;
	case FUNC_CEILING :                  return 1;
	case FUNC_HYPGEOMDIST :              break;
	case FUNC_LOGNORMDIST :              break;
	case FUNC_LOGINV :                   break;
	case FUNC_NEGBINOMDIST :             break;
	case FUNC_NORMDIST :                 break;
	case FUNC_NORMSDIST :                break;
	case FUNC_NORMINV :                  break;
	case FUNC_NORMSINV :                 break;
	case FUNC_STANDARDIZE :              break;
	case FUNC_ODD :                      break;
	case FUNC_PERMUT :                   break;
	case FUNC_POISSON :                  break;
	case FUNC_TDIST :                    break;
	case FUNC_WEIBULL :                  break;
	case FUNC_SUMXMY2 :                  break;
	case FUNC_SUMX2MY2 :                 break;
	case FUNC_SUMX2PY2 :                 break;
	case FUNC_CHITEST :                  break;
	case FUNC_CORREL :                   break;
	case FUNC_COVAR :                    break;
	case FUNC_FORECAST :                 break;
	case FUNC_FTEST :                    break;
	case FUNC_INTERCEPT :                break;
	case FUNC_PEARSON :                  break;
	case FUNC_RSQ :                      break;
	case FUNC_STEYX :                    break;
	case FUNC_SLOPE :                    break;
	case FUNC_TTEST :                    break;
	case FUNC_PROB :                     break;
	case FUNC_DEVSQ :                    break;
	case FUNC_GEOMEAN :                  break;
	case FUNC_HARMEAN :                  break;
	case FUNC_SUMSQ :                    break;
	case FUNC_KURT :                     break;
	case FUNC_SKEW :                     break;
	case FUNC_ZTEST :                    break;
	case FUNC_LARGE :                    break;
	case FUNC_SMALL :                    break;
	case FUNC_QUARTILE :                 break;
	case FUNC_PERCENTILE :               break;
	case FUNC_PERCENTRANK :              break;
	case FUNC_MODE :                     break;
	case FUNC_TRIMMEAN :                 break;
	case FUNC_TINV :                     break;
	case FUNC_MOVIECOMMAND :             break;
	case FUNC_GETMOVIE :                 break;
	case FUNC_CONCATENATE :              break;
	case FUNC_POWER :                    break;
	case FUNC_PIVOTADDDATA :             break;
	case FUNC_GETPIVOTTABLE :            break;
	case FUNC_GETPIVOTFIELD :            break;
	case FUNC_GETPIVOTITEM :             break;
	case FUNC_RADIANS :                  break;
	case FUNC_DEGREES :                  break;
	case FUNC_SUBTOTAL :                 break;
	case FUNC_SUMIF :                    break;
	case FUNC_COUNTIF :                  break;
	case FUNC_COUNTBLANK :               break;
	case FUNC_SCENARIOGET :              break;
	case FUNC_OPTIONSLISTSGET :          break;
	case FUNC_ISPMT :                    break;
	case FUNC_DATEDIF :                  break;
	case FUNC_DATESTRING :               break;
	case FUNC_NUMBERSTRING :             break;
	case FUNC_ROMAN :                    break;
	case FUNC_OPENDIALOG :               break;
	case FUNC_SAVEDIALOG :               break;
	case FUNC_VIEWGET :                  break;
	case FUNC_GETPIVOTDATA :             break;
	case FUNC_HYPERLINK :                break;
	case FUNC_PHONETIC :                 break;
	case FUNC_AVERAGEA :                 break;
	case FUNC_MAXA :                     break;
	case FUNC_MINA :                     break;
	case FUNC_STDEVPA :                  break;
	case FUNC_VARPA :                    break;
	case FUNC_STDEVA :                   break;
	case FUNC_VARA :                     break;
	case FUNC_BAHTTEXT :                 break;
	case FUNC_THAIDAYOFWEEK :            break;
	case FUNC_THAIDIGIT :                break;
	case FUNC_THAIMONTHOFYEAR :          break;
	case FUNC_THAINUMSOUND :             break;
	case FUNC_THAINUMSTRING :            break;
	case FUNC_THAISTRINGLENGTH :         break;
	case FUNC_ISTHAIDIGIT :              return 1;
	case FUNC_ROUNDBAHTDOWN :            break;
	case FUNC_ROUNDBAHTUP :              break;
	case FUNC_THAIYEAR :                 break;
	case FUNC_RTD :                      break;
	case FUNC_CUBEVALUE :                break;
	case FUNC_CUBEMEMBER :               break;
	case FUNC_CUBEMEMBERPROPERTY :       break;
	case FUNC_CUBERANKEDMEMBER :         break;
	case FUNC_HEX2BIN :                  break;
	case FUNC_HEX2DEC :                  break;
	case FUNC_HEX2OCT :                  break;
	case FUNC_DEC2BIN :                  break;
	case FUNC_DEC2HEX :                  break;
	case FUNC_DEC2OCT :                  break;
	case FUNC_OCT2BIN :                  break;
	case FUNC_OCT2HEX :                  break;
	case FUNC_OCT2DEC :                  break;
	case FUNC_BIN2DEC :                  break;
	case FUNC_BIN2OCT :                  break;
	case FUNC_BIN2HEX :                  break;
	case FUNC_IMSUB :                    break;
	case FUNC_IMDIV :                    break;
	case FUNC_IMPOWER :                  break;
	case FUNC_IMABS :                    break;
	case FUNC_IMSQRT :                   break;
	case FUNC_IMLN :                     break;
	case FUNC_IMLOG2 :                   break;
	case FUNC_IMLOG10 :                  break;
	case FUNC_IMSIN :                    break;
	case FUNC_IMCOS :                    break;
	case FUNC_IMEXP :                    break;
	case FUNC_IMARGUMENT :               break;
	case FUNC_IMCONJUGATE :              break;
	case FUNC_IMAGINARY :                break;
	case FUNC_IMREAL :                   break;
	case FUNC_COMPLEX :                  break;
	case FUNC_IMSUM :                    break;
	case FUNC_IMPRODUCT :                break;
	case FUNC_SERIESSUM :                break;
	case FUNC_FACTDOUBLE :               break;
	case FUNC_SQRTPI :                   break;
	case FUNC_QUOTIENT :                 break;
	case FUNC_DELTA :                    break;
	case FUNC_GESTEP :                   break;
	case FUNC_ISEVEN :                   return 1;
	case FUNC_ISODD :                    return 1;
	case FUNC_MROUND :                   break;
	case FUNC_ERF :                      break;
	case FUNC_ERFC :                     break;
	case FUNC_BESSELJ :                  break;
	case FUNC_BESSELK :                  break;
	case FUNC_BESSELY :                  break;
	case FUNC_BESSELI :                  break;
	case FUNC_XIRR :                     break;
	case FUNC_XNPV :                     break;
	case FUNC_PRICEMAT :                 break;
	case FUNC_YIELDMAT :                 break;
	case FUNC_INTRATE :                  break;
	case FUNC_RECEIVED :                 break;
	case FUNC_DISC :                     break;
	case FUNC_PRICEDISC :                break;
	case FUNC_YIELDDISC :                break;
	case FUNC_TBILLEQ :                  break;
	case FUNC_TBILLPRICE :               break;
	case FUNC_TBILLYIELD :               break;
	case FUNC_PRICE :                    break;
	case FUNC_YIELD :                    break;
	case FUNC_DOLLARDE :                 break;
	case FUNC_DOLLARFR :                 break;
	case FUNC_NOMINAL :                  break;
	case FUNC_EFFECT :                   break;
	case FUNC_CUMPRINC :                 break;
	case FUNC_CUMIPMT :                  break;
	case FUNC_EDATE :                    break;
	case FUNC_EOMONTH :                  break;
	case FUNC_YEARFRAC :                 break;
	case FUNC_COUPDAYBS :                break;
	case FUNC_COUPDAYS :                 break;
	case FUNC_COUPDAYSNC :               break;
	case FUNC_COUPNCD :                  break;
	case FUNC_COUPNUM :                  break;
	case FUNC_COUPPCD :                  break;
	case FUNC_DURATION :                 break;
	case FUNC_MDURATION :                break;
	case FUNC_ODDLPRICE :                break;
	case FUNC_ODDLYIELD :                break;
	case FUNC_ODDFPRICE :                break;
	case FUNC_ODDFYIELD :                break;
	case FUNC_RANDBETWEEN :              break;
	case FUNC_WEEKNUM :                  break;
	case FUNC_AMORDEGRC :                break;
	case FUNC_AMORLINC :                 break;
	case FUNC_CONVERT :                  break;
	case FUNC_ACCRINT :                  break;
	case FUNC_ACCRINTM :                 break;
	case FUNC_WORKDAY :                  break;
	case FUNC_NETWORKDAYS :              break;
	case FUNC_GCD :                      break;
	case FUNC_MULTINOMIAL :              break;
	case FUNC_LCM :                      break;
	case FUNC_FVSCHEDULE :               break;
	case FUNC_CUBEKPIMEMBER :            break;
	case FUNC_CUBESET :                  break;
	case FUNC_CUBESETCOUNT :             break;
	case FUNC_IFERROR :                  break;
	case FUNC_COUNTIFS :                 break;
	case FUNC_SUMIFS :                   break;
	case FUNC_AVERAGEIF :                break;
	case FUNC_AVERAGEIFS :               break;
	case FUNC_AGGREGATE :                break;
	case FUNC_BINOM_DIST :               break;
	case FUNC_BINOM_INV :                break;
	case FUNC_CONFIDENCE_NORM :          break;
	case FUNC_CONFIDENCE_T :             break;
	case FUNC_CHISQ_TEST :               break;
	case FUNC_F_TEST :                   break;
	case FUNC_COVARIANCE_P :             break;
	case FUNC_COVARIANCE_S :             break;
	case FUNC_EXPON_DIST :               break;
	case FUNC_GAMMA_DIST :               break;
	case FUNC_GAMMA_INV :                break;
	case FUNC_MODE_MULT :                break;
	case FUNC_MODE_SNGL :                break;
	case FUNC_NORM_DIST :                break;
	case FUNC_NORM_INV :                 break;
	case FUNC_PERCENTILE_EXC :           break;
	case FUNC_PERCENTILE_INC :           break;
	case FUNC_PERCENTRANK_EXC :          break;
	case FUNC_PERCENTRANK_INC :          break;
	case FUNC_POISSON_DIST :             break;
	case FUNC_QUARTILE_EXC :             break;
	case FUNC_QUARTILE_INC :             break;
	case FUNC_RANK_AVG :                 break;
	case FUNC_RANK_EQ :                  break;
	case FUNC_STDEV_S :                  break;
	case FUNC_STDEV_P :                  break;
	case FUNC_T_DIST :                   break;
	case FUNC_T_DIST_2T :                break;
	case FUNC_T_DIST_RT :                break;
	case FUNC_T_INV :                    break;
	case FUNC_T_INV_2T :                 break;
	case FUNC_VAR_S :                    break;
	case FUNC_VAR_P :                    break;
	case FUNC_WEIBULL_DIST :             break;
	case FUNC_NETWORKDAYS_INTL :         break;
	case FUNC_WORKDAY_INTL :             break;
	case FUNC_ECMA_CEILING :             break;
	case FUNC_ISO_CEILING :              break;
	case FUNC_BETA_DIST :                break;
	case FUNC_BETA_INV :                 break;
	case FUNC_CHISQ_DIST :               break;
	case FUNC_CHISQ_DIST_RT :            break;
	case FUNC_CHISQ_INV :                break;
	case FUNC_CHISQ_INV_RT :             break;
	case FUNC_F_DIST :                   break;
	case FUNC_F_DIST_RT :                break;
	case FUNC_F_INV :                    break;
	case FUNC_F_INV_RT :                 break;
	case FUNC_HYPGEOM_DIST :             break;
	case FUNC_LOGNORM_DIST :             break;
	case FUNC_LOGNORM_INV :              break;
	case FUNC_NEGBINOM_DIST :            break;
	case FUNC_NORM_S_DIST :              break;
	case FUNC_NORM_S_INV :               break;
	case FUNC_T_TEST :                   break;
	case FUNC_Z_TEST :                   break;
	case FUNC_ERF_PRECISE :              break;
	case FUNC_ERFC_PRECISE :             break;
	case FUNC_GAMMALN_PRECISE :          break;
	case FUNC_CEILING_PRECISE :          break;
	case FUNC_FLOOR_PRECISE :            break;
	}
	return -1;
}


function_basenode_t::function_basenode_t(CGlobalRecords& gRecords, expr_function_code_t f) :
expression_node_t(gRecords),
func(f)
{

}
function_basenode_t::~function_basenode_t()
{

}

void function_basenode_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	/*
	We don't want to spend the effort to 'know' (~ make this code aware) which function produces what result, neither in value nor in type, so we
	fake it and make it easy to ourselves: we 'guestimate' the UDF will return an
	error code and we mark the expression as 'calc on load' to mask our ignorance.

	Remark: there's just a few functions we care about: RANDOM() and a few others, which we are willing to take to the dance.
	*/
	switch (func)
	{
	default:
		dst.SetCalcOnLoad();
		dst.SetErrorCode(XLERR_VALUE);
		break;

	case FUNC_UDF:
		XL_ASSERT(!"Should've been handled by the udf class!");

		case FUNC_ISNA :
		case FUNC_ISERROR :
		case FUNC_TRUE :
		case FUNC_FALSE :
		case FUNC_AND :
		case FUNC_OR :
		case FUNC_NOT :
		case FUNC_ISREF :
		case FUNC_ISERR :
		case FUNC_ISTEXT :
		case FUNC_ISNUMBER :
		case FUNC_ISBLANK :
		case FUNC_T :
		case FUNC_N :
		case FUNC_ISNONTEXT :
		case FUNC_ISLOGICAL :
		case FUNC_ISPMT :
		case FUNC_ISTHAIDIGIT :
			dst.SetCalcOnLoad();
			dst.SetBoolean(false); // faked value estimate!
			break;

		case FUNC_COUNT:
		case FUNC_ROW :
		case FUNC_COLUMN :
		case FUNC_DCOUNT :
		case FUNC_DAY :
		case FUNC_MONTH :
		case FUNC_YEAR :
		case FUNC_WEEKDAY :
		case FUNC_HOUR :
		case FUNC_MINUTE :
		case FUNC_SECOND :
		case FUNC_COUNTA :
		case FUNC_DCOUNTA :
		case FUNC_COUNTIF :
		case FUNC_COUNTBLANK :
		case FUNC_WEEKNUM :
		case FUNC_COUNTIFS :
			dst.SetCalcOnLoad();
			dst.SetInteger(42); // faked value estimate!
			break;

		case FUNC_PI :
			dst.SetCalcOnLoad();
			dst.SetFloatingPoint(3.1415); // faked value estimate!
			break;

		case FUNC_SUM :
		case FUNC_AVERAGE :
		case FUNC_MIN :
		case FUNC_MAX :
		case FUNC_STDEV :
		case FUNC_SIN :
		case FUNC_COS :
		case FUNC_TAN :
		case FUNC_ATAN :
		case FUNC_SQRT :
		case FUNC_EXP :
		case FUNC_LN :
		case FUNC_LOG10 :
		case FUNC_ABS :
		case FUNC_ROUND :
		case FUNC_MOD :
		case FUNC_DSUM :
		case FUNC_DAVERAGE :
		case FUNC_DMIN :
		case FUNC_DMAX :
		case FUNC_DSTDEV :
		case FUNC_VAR :
		case FUNC_DVAR :
		case FUNC_DATE :
		case FUNC_TIME :
		case FUNC_NOW :
		case FUNC_OFFSET :
		case FUNC_ATAN2 :
		case FUNC_ASIN :
		case FUNC_ACOS :
		case FUNC_LOG :
		case FUNC_PRODUCT :
		case FUNC_FACT :
		case FUNC_STDEVP :
		case FUNC_VARP :
		case FUNC_DSTDEVP :
		case FUNC_ROUNDUP :
		case FUNC_ROUNDDOWN :
		case FUNC_DAYS360 :
		case FUNC_TODAY :
		case FUNC_MEDIAN :
		case FUNC_SUMPRODUCT :
		case FUNC_SINH :
		case FUNC_COSH :
		case FUNC_TANH :
		case FUNC_ASINH :
		case FUNC_ACOSH :
		case FUNC_ATANH :
		case FUNC_FLOOR :
		case FUNC_KURT :
		case FUNC_SKEW :
		case FUNC_POWER :
		case FUNC_RADIANS :
		case FUNC_DEGREES :
		case FUNC_SUMIF :
		case FUNC_AVERAGEA :
		case FUNC_MAXA :
		case FUNC_MINA :
		case FUNC_STDEVPA :
		case FUNC_VARPA :
		case FUNC_STDEVA :
		case FUNC_VARA :
		case FUNC_SERIESSUM :
		case FUNC_ERF :
		case FUNC_ERFC :
		case FUNC_WORKDAY :
		case FUNC_NETWORKDAYS :
		case FUNC_GCD :
		case FUNC_SUMIFS :
		case FUNC_AVERAGEIF :
		case FUNC_AVERAGEIFS :
		case FUNC_ERF_PRECISE :
		case FUNC_ERFC_PRECISE :
		case FUNC_GAMMALN_PRECISE :
		case FUNC_CEILING_PRECISE :
		case FUNC_FLOOR_PRECISE :
			dst.SetCalcOnLoad();
			dst.SetFloatingPoint(42.0); // faked value estimate!
			break;

		case FUNC_LOWER :
		case FUNC_UPPER :
		case FUNC_LEFT :
		case FUNC_RIGHT :
		case FUNC_TRIM :
		case FUNC_REPLACE :
		case FUNC_CONCATENATE :
		case FUNC_DATESTRING :
		case FUNC_NUMBERSTRING :
		case FUNC_ROMAN :
		case FUNC_HYPERLINK :
		case FUNC_PHONETIC :
		case FUNC_BAHTTEXT :
		case FUNC_HEX2BIN :
		case FUNC_HEX2DEC :
		case FUNC_HEX2OCT :
		case FUNC_DEC2BIN :
		case FUNC_DEC2HEX :
		case FUNC_DEC2OCT :
		case FUNC_OCT2BIN :
		case FUNC_OCT2HEX :
		case FUNC_OCT2DEC :
		case FUNC_BIN2DEC :
		case FUNC_BIN2OCT :
		case FUNC_BIN2HEX :
			dst.SetCalcOnLoad();
			dst.SetText("???"); // faked value estimate!
			break;

		case FUNC_RAND :
		case FUNC_VOLATILE :
		case FUNC_RANDBETWEEN :
			dst.SetCalcAlways();
			dst.SetCalcOnLoad();
			dst.SetFloatingPoint(0.5); // faked random value estimate!
			break;
	}
}

size_t function_basenode_t::GetSize(bool include_subtree) const
{
	size_t len = 1+2; // OP_FUNC

	if (ExcelFunctionHasNumberOfFixedArgs(func))
	{
		len += 1;
	}
	
	if (include_subtree)
	{
		size_t chcnt = GetNumberOfChilds();

#if defined(XL_WITH_ASSERTIONS)
		size_t argcnt = KnownNumberOfArgsForExcelFunction(func);
		XL_ASSERT(chcnt == argcnt ? argcnt >= 0 : true); // make sure we match the known number of args
#endif

		while (chcnt-- > 0)
		{
			XL_ASSERT(GetChild(chcnt));
			len += GetChild(chcnt)->GetSize(include_subtree);
		}
	}
	return len;
}
signed8_t function_basenode_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode = NO_ERRORS;
	size_t argcnt = KnownNumberOfArgsForExcelFunction(func);

	if (include_subtree)
	{
		size_t chcnt = GetNumberOfChilds();
		size_t idx;

		for (idx = 0; idx < chcnt; idx++)
		{
			XL_ASSERT(GetChild(idx));
			errcode |= GetChild(idx)->DumpData(dst, include_subtree);
		}
	}

	if (argcnt >= 0)
	{
		XL_ASSERT(GetNumberOfChilds() == argcnt);
		errcode |= dst.AddValue8(OP_FUNC);
	}
	else
	{
		errcode |= dst.AddValue8(OP_FUNCVAR);
		errcode |= dst.AddValue8((unsigned16_t)GetNumberOfChilds() & 0x7F); // no prompt for user: 0x80 not set
	}

	errcode |= dst.AddValue16((unsigned16_t)func & 0x7FFF); // no command equivalent function: 0x8000 not set

	return errcode;
}



z_ary_func_node_t::z_ary_func_node_t(CGlobalRecords& gRecords, expr_function_code_t func) :
	function_basenode_t(gRecords, func)
	{

	}
z_ary_func_node_t::~z_ary_func_node_t()
{

}





unary_func_node_t::unary_func_node_t(CGlobalRecords& gRecords, expr_function_code_t op, expression_node_t* a) :
function_basenode_t(gRecords, op),
arg(a)
{

}
unary_func_node_t::~unary_func_node_t()
{

}

expression_node_t* unary_func_node_t::GetChild(unsigned16_t index) const
{
	return arg;
}
unsigned16_t unary_func_node_t::GetNumberOfChilds(void) const
{
	return 1;
}






binary_func_node_t::binary_func_node_t(CGlobalRecords& gRecords, expr_function_code_t op, expression_node_t* arg1, expression_node_t* arg2) :
function_basenode_t(gRecords, op)
{
	args[0] = arg1;
	args[1] = arg2;
}
binary_func_node_t::~binary_func_node_t()
{

}

expression_node_t* binary_func_node_t::GetChild(unsigned16_t index) const
{
return args[index];
}
unsigned16_t binary_func_node_t::GetNumberOfChilds(void) const
{
return 2;
}






n_ary_func_node_t::n_ary_func_node_t(CGlobalRecords& gRecords, expr_function_code_t func, size_t count, expression_node_t** arr) :
function_basenode_t(gRecords, func),
arg_arrsize(0), arg_count(0), arg_arr(NULL)
{
	if (count > 0)
	{
		XL_ASSERT(arr);
		arg_arr = (expression_node_t **)malloc(count * sizeof(arg_arr[0]));
		arg_arrsize = count;
		arg_count = count;

		while (count-- > 0)
		{
			arg_arr[count] = arr[count];
		}
	}
}
n_ary_func_node_t::~n_ary_func_node_t()
{
	if (arg_arr)
	{
		free((void *)arg_arr);
	}
}

expression_node_t* n_ary_func_node_t::GetChild(unsigned16_t index) const
{
	return arg_arr[index];
}
unsigned16_t n_ary_func_node_t::GetNumberOfChilds(void) const
{
	return arg_count;
}

function_basenode_t& n_ary_func_node_t::PushArg(expression_node_t* arg)
{
	if (arg_arr == NULL)
	{
		arg_arrsize = 2;
		XL_ASSERT(arg_count == 0);
		arg_arr = (expression_node_t **)malloc(2 * sizeof(arg_arr[0]));
	}
	else
	{
		while (arg_count + 1 > arg_arrsize)
		{
			arg_arrsize += 2;
		}
		arg_arr = (expression_node_t **)realloc((void *)arg_arr, arg_arrsize * sizeof(arg_arr[0]));
	}

	arg_arr[arg_count++] = arg;

	return *this;
}




userdef_func_node_t::userdef_func_node_t(CGlobalRecords& gRecords, int udf_num, size_t count, expression_node_t** arr) :
n_ary_func_node_t(gRecords, FUNC_UDF, count, arr),
expr_user_function_code(udf_num)
{

}
userdef_func_node_t::~userdef_func_node_t()
{

}

void userdef_func_node_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	/*
	We don't know what the heck a UDF will spit out as a result, not in value nor in type, so we
	take a pod shot at this and make it easy to ourselves: we 'guestimate' the UDF will return an
	error code and we mark the expression as 'calc on load' to mask our ignorance.
	*/
	dst.SetCalcOnLoad();
	dst.SetErrorCode(XLERR_VALUE);
}

size_t userdef_func_node_t::GetSize(bool include_subtree) const
{
	return 0;
}
signed8_t userdef_func_node_t::DumpData(CUnit &dst, bool include_subtree) const
{
	signed8_t errcode = NO_ERRORS;

	errcode |= dst.AddValue8(OP_STR);

	return errcode;
}






expression_node_factory_t::expression_node_factory_t(CGlobalRecords& glbl) :
m_GlobalRecords(glbl)
{

}
expression_node_factory_t::~expression_node_factory_t()
{

}

expression_node_factory_t &expression_node_factory_t::operator =(const expression_node_factory_t &src)
{
	throw std::string("Should never have invoked the expression_node_factory_t copy operator!");
}



boolean_value_node_t *expression_node_factory_t::boolean(bool value)
{
	return new boolean_value_node_t(m_GlobalRecords, value);
}
integer_value_node_t *expression_node_factory_t::integer(signed32_t value)
{
	return new integer_value_node_t(m_GlobalRecords, value);
}
float_value_node_t *expression_node_factory_t::floating_point(double value)
{
	return new float_value_node_t(m_GlobalRecords, value);
}
error_value_node_t *expression_node_factory_t::error_value(errcode_t value)
{
	return new error_value_node_t(m_GlobalRecords, value);
}
missing_arg_node_t *expression_node_factory_t::missing_arg(void)
{
	return new missing_arg_node_t(m_GlobalRecords);
}
text_value_node_t *expression_node_factory_t::text(const std::string& value)
{
	return new text_value_node_t(m_GlobalRecords, value);
}
text_value_node_t *expression_node_factory_t::text(const u16string& value)
{
	return new text_value_node_t(m_GlobalRecords, value);
}
cell_deref_node_t *expression_node_factory_t::cell(const cell_t& cellref, cell_addr_mode_t attr, cell_op_class_t opclass)
{
	return new cell_deref_node_t(m_GlobalRecords, cellref, attr, opclass);
}
cell_deref_node_t *expression_node_factory_t::cell(const cell_t& cellref, const worksheet* ws, cell_addr_mode_t attr, cell_op_class_t opclass)
{
	return new cell_deref_node_t(m_GlobalRecords, cellref, ws, attr, opclass);
}
unary_op_node_t *expression_node_factory_t::op(expr_operator_code_t op, expression_node_t* arg)
{
	return new unary_op_node_t(m_GlobalRecords, op, arg);
}
binary_op_node_t *expression_node_factory_t::op(expr_operator_code_t op, expression_node_t* arg1, expression_node_t* arg2)
{
	return new binary_op_node_t(m_GlobalRecords, op, arg1, arg2);
}
z_ary_func_node_t *expression_node_factory_t::f(expr_function_code_t func)
{
	return new z_ary_func_node_t(m_GlobalRecords, func);
}
unary_func_node_t *expression_node_factory_t::f(expr_function_code_t func, expression_node_t* arg)
{
	return new unary_func_node_t(m_GlobalRecords, func, arg);
}
binary_func_node_t *expression_node_factory_t::f(expr_function_code_t func, expression_node_t* arg1, expression_node_t* arg2)
{
	return new binary_func_node_t(m_GlobalRecords, func, arg1, arg2);
}
n_ary_func_node_t *expression_node_factory_t::f(expr_function_code_t func, size_t argcount, expression_node_t** arg_arr)
{
	return new n_ary_func_node_t(m_GlobalRecords, func, argcount, arg_arr);
}
userdef_func_node_t *expression_node_factory_t::udf(int expr_user_function, size_t argcount, expression_node_t** arg_arr)
{
	return new userdef_func_node_t(m_GlobalRecords, expr_user_function, argcount, arg_arr);
}
















estimated_formula_result_t::estimated_formula_result_t(CGlobalRecords& gRecords) :
	value_type(ESTVAL_UNKNOWN),
	always_calc(0),
	calc_on_load(0),
	m_GlobalRecords(gRecords)
{
/*
	union
	{
		bool b;
		signed32_t i;
		double f;
		u16string *s;
		errcode_t e;
	} value;
	enum
	{
		ESTVAL_UNKNOWN = 0,
		ESTVAL_BOOLEAN,
		ESTVAL_INTEGER,
		ESTVAL_FLOATINGPOINT,
		ESTVAL_STRING,
		ESTVAL_ERRORCODE,
	} value_type;

	bool always_calc : 1;
	bool calc_on_load: 1;
*/
	memset(&value, 0, sizeof(value));
}
estimated_formula_result_t::~estimated_formula_result_t()
{
	clear_value(ESTVAL_UNKNOWN);
}

estimated_formula_result_t& estimated_formula_result_t::operator =(const estimated_formula_result_t &src)
{
	throw std::string("Should never have invoked the estimated_formula_result_t copy operator!");
}

void estimated_formula_result_t::clear_value(estval_type_t incoming_type)
{
	if (value_type == ESTVAL_STRING && incoming_type != ESTVAL_STRING)
	{
		delete value.s;
	}
	else if (value_type != ESTVAL_STRING && incoming_type == ESTVAL_STRING)
	{
		value.s = new u16string;
	}
	value_type = incoming_type;
}

void estimated_formula_result_t::SetCalcOnLoad(void)
{
	calc_on_load = 1;
}
void estimated_formula_result_t::SetCalcAlways(void)
{
	always_calc = 1;
}

unsigned16_t estimated_formula_result_t::GetOptionFlags(void) const
{
	return (calc_on_load << 1 | always_calc);
}

bool estimated_formula_result_t::SetBoolean(bool v)
{
	clear_value(ESTVAL_BOOLEAN);
	return value.b = v;
}
signed32_t estimated_formula_result_t::SetInteger(signed32_t v)
{
	clear_value(ESTVAL_INTEGER);
	return value.i = v;
}
double estimated_formula_result_t::SetFloatingPoint(double v)
{
	clear_value(ESTVAL_FLOATINGPOINT);
	return value.f = v;
}
const u16string& estimated_formula_result_t::SetText(const std::string& v)
{
	clear_value(ESTVAL_STRING);
	m_GlobalRecords.char2str16(v, *value.s);
	return *value.s;
}
const u16string& estimated_formula_result_t::SetText(const std::ustring& v)
{
	clear_value(ESTVAL_STRING);
	m_GlobalRecords.wide2str16(v, *value.s);
	return *value.s;
}
const u16string& estimated_formula_result_t::SetText(const u16string& v)
{
	clear_value(ESTVAL_STRING);
	return *value.s = v;
}
errcode_t estimated_formula_result_t::SetErrorCode(errcode_t v)
{
	clear_value(ESTVAL_ERRORCODE);
	return value.e = v;
}

unsigned64_t estimated_formula_result_t::GetEncodedValue(void) const
{
	unsigned64_t rv;

	switch (value_type)
	{
	default:
	case ESTVAL_UNKNOWN:
		XL_ASSERT(!"Should never get here!");
		
	case ESTVAL_BOOLEAN:
		rv = 0xFFFF000000000001ULL;	
		rv |= ((unsigned32_t)value.b) << 16;
		break;

	case ESTVAL_INTEGER:
		rv = (unsigned64_t)(double)value.i;
		break;
		
	case ESTVAL_FLOATINGPOINT:
		rv = (unsigned64_t)value.f;
		break;

	case ESTVAL_STRING:
		rv = 0xFFFF000000000000ULL;
		break;

	case ESTVAL_ERRORCODE:
		rv = 0xFFFF000000000002ULL;
		rv |= ((unsigned32_t)value.e) << 16;
		break;
	}
	return rv;
}
bool estimated_formula_result_t::EncodedValueIsString(void) const
{
	return value_type == ESTVAL_STRING;
}
const u16string* estimated_formula_result_t::GetStringValue(void) const
{
	if (value_type == ESTVAL_STRING)
	{
		return value.s;
	}
	return NULL;
}





formula_t::formula_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, expression_node_t* ast_val, bool autodes, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval),
	ast(ast_val),
	auto_destruct_expression_tree(autodes)
{
	XL_ASSERT(ast_val);
}

formula_t::~formula_t()
{
	if (ast && auto_destruct_expression_tree)
	{
		ast->DestroyAST();
	}
}

void formula_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	if (ast)
	{
		ast->GetResultEstimate(dst);
	}
}

size_t formula_t::GetSize(void) const
{
	XL_ASSERT(GetAST());

	size_t len = 4+2+2+2+8+2+4+2 + GetAST()->GetSize();

	return len;
}

CUnit* formula_t::GetData(CDataStorage &datastore) const 
{
   return datastore.MakeCFormula(*this);	// NOTE: this pointer HAS to be deleted elsewhere.
}

/*
*********************************
CFormula class implementation
*********************************
*/

CFormula::CFormula(CDataStorage &datastore, const formula_t& expr):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_FORMULA);	 // followed by the RECTYPE_STRING record when the formula evaluates to a string!
   AddValue16((unsigned16_t)expr.GetRow());
   AddValue16((unsigned16_t)expr.GetCol());
   AddValue16(expr.GetXFIndex());

   estimated_formula_result_t estimate(expr.GetGlobalRecords());

   expr.GetResultEstimate(estimate);
   AddValue64(estimate.GetEncodedValue()); // current_value_of_formula
   AddValue16(estimate.GetOptionFlags()); // flags: grbit
   AddValue32(0); // chn

   size_t len_position = GetDataSize();
   AddValue16(0 /* expr.GetSize() */ ); // length_of_parsed_expr

   expr.GetAST()->DumpData(*this, true);
   //AddDataArray(NULL, 0); // rgce dump, length_of_parsed_expr
   
   size_t end = GetDataSize();
   SetValueAt16(end - len_position - 2, len_position);

   SetRecordLength(GetDataSize()-4);
}

CFormula::~CFormula()
{
}
