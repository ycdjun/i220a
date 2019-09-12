#include "bits-to-ints.h"
#include "errors.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

//@TODO: auxiliary definitions

/** This function should be called with inFile set to an input FILE
 *  stream corresponding to a file named inName.  This FILE should
 *  contain a stream of '0' or '1' bit-value characters optionally
 *  separated by whitespace (as determined by isspace()) characters.
 *  The function should return the next unsigned integer value given
 *  by the next next nBits bit characters read from in.
 *
 *  The nBits argument (which should be > 0) will specify the number
 *  of bits from FILE stream inFile which are to be grouped to form an
 *  unsigned integer value.
 *
 *  The bytes within the bit-stream are assumed to be in big-endian
 *  order (with the most significant byte first), and the bits within
 *  each byte are ordered little-endian with the least-significant bit
 *  first.
 *
 *  For example, assume that nBits is 16 and the least significant bit
 *  in the result is at index 0, and most significant bit in the
 *  result at index 15; i.e., the bits in the BitsValue result are
 *  indexed 15-14-...-1-0.  The order of the corresponding bits in the
 *  input stream will be 8-9-10-11-12-13-14-15 - 0-1-2-3-4-5-6-7.  So
 *  the bit-stream "1101 0101 1011 0011" will result in the BitsValue
 *  0xabcd (which has binary representation 1010_1011_1100_1101).
 *
 *  When the function returns, *isEof should be set to true if
 *  end-of-file is encountered on inFile.  If EOF is encountered
 *  within a partial value, then a suitable error message should be
 *  printed before returning with *isEof true.
 *
 *  If any character other than '0', '1' or whitespace is encountered
 *  in inFile, then a suitable error message should be printed and the
 *  function should return with *isEof set to true.
 */

int getbits(FILE *inFile, bool *isEof){
	char c = ' ';
	while(isspace(c)){
		c = fgetc(inFile);
		if(c == '1'){
			return 1;
		}
		else if(c == '0'){
			return 0;
		}
		else{
			if(!isspace(c) && c != '1' && c != '0'){
				*isEof = true;
				fatal("Invalid character '%c' , c");
				
			}
		}
	}
	*isEof = true;
	return -1;
	
}

long long getByte(FILE *inFile,bool *isEof){
	long long byte = 0;
	int temp[CHAR_BIT];
	for(int i = 0; i < CHAR_BIT; i++){
		temp[i] = getbits(inFile,isEof) << i;
		byte |= temp[i];
	}
	return byte;
}

BitsValue getWord(FILE *inFile, int nBits, bool *isEof){
	BitsValue temp = 0;
	for(int i = 0; i< nBits/8;i++){
		temp |= getByte(inFile,isEof) << ((nBits-8) - (8 * i));
	}
	return temp;
}
	

BitsValue
bits_to_ints(FILE *inFile, const char *inName, int nBits, bool *isEof)
{
  //nBits value should make sense
  assert(0 < nBits && nBits <= CHAR_BIT*sizeof(BitsValue));
  BitsValue value = getWord(inFile, nBits, isEof);
  return value;
}
