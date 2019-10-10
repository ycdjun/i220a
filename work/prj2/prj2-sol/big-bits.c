#include "big-bits.h"
#include "hex-util.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/** Provide concrete definition for struct BigBits to flesh out
 *  abstract BigBits data type.
 */
struct BigBits {
  //@TODO
	char *bits;
	size_t size;
};



/** Return a pointer to a representation of a big integer with value
 *  corresponding to the non-empty hexadecimal string hex.  Note that
 *  hex will only contain hexadecimal characters '0' - '9', 'a' - 'f'
 *  and 'A' - 'F' terminated by a NUL '\0' char.
 *
 *  The string hex may not remain valid after this function returns.
 *
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *
newBigBits(const char *hex)
{
  assert(CHAR_BIT == 8);
  BigBits *nBigBits = (BigBits *)malloc(sizeof(BigBits));
  int i = 0;
  nBigBits->size = 0;
  while(hex[i] != '\0'){
	nBigBits->size++;
	i++;
  }
  nBigBits->bits = (char *)malloc(sizeof(char)*nBigBits->size);
  for(size_t i = 0; i < nBigBits->size; i++){
	nBigBits->bits[i] = hex[i];
  }
  return nBigBits;
}

/** Frees all resources used by currently valid bigBits.  bigBits
 *  is not valid after this function returns.
 */
void
freeBigBits(BigBits *bigBits)
{
  free(bigBits->bits);
  free(bigBits);
}


/** Return a lower-case hex string representing bigBits. Returned
 *  string should not contain any non-significant leading zeros.
 *  Returns NULL on error with errno set "appropriately".  (Note that
 *  there is no call to free the corresponding string).
 */
const char *
stringBigBits(const BigBits *bigBits)
{
  char *sBigBits = (char *)calloc(1 + bigBits->size, sizeof(char));
  int numZeros = 0;
  if(bigBits->bits[0] == '0'){
	while(bigBits->bits[numZeros] == '0'){
		numZeros++;
	}
  }
  for(size_t i = numZeros; i < bigBits->size; i++){
	sBigBits[i - numZeros] = bigBits->bits[i];
  }
  return sBigBits;
}


/** Return a new BigBits which is the bitwise-& of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *
andBigBits(const BigBits *bigBits1, const BigBits *bigBits2)
{
  BigBits *aBigBits = (BigBits *)malloc(sizeof(BigBits));
  if(bigBits1->size >= bigBits2->size){
	aBigBits->size = bigBits1->size;
	size_t offset = bigBits1->size - bigBits2->size;
	aBigBits->bits = (char *)calloc(bigBits1->size, sizeof(char));
	for(size_t i = 0; i < aBigBits->size;i++){
		int bigger,smaller, temp;
		bigger = charToHexet(bigBits1->bits[i]);
		if(i < offset){
			smaller = 0;
		}
		else{
			smaller = charToHexet(bigBits2->bits[i-offset]);
		}
		temp = bigger & smaller;
		aBigBits->bits[i] = hexetToChar(temp);
	}
  }
  else{
	aBigBits->size = bigBits2->size;
	size_t offset = bigBits2->size - bigBits1->size;
	aBigBits->bits = (char *)calloc(bigBits2->size, sizeof(char));
	for(size_t i = 0; i < aBigBits->size;i++){
		int bigger,smaller, temp;
		bigger = charToHexet(bigBits2->bits[i]);
		if(i < offset){
			smaller = 0;
		}
		else{
			smaller = charToHexet(bigBits1->bits[i-offset]);
		}
		temp = bigger & smaller;
		aBigBits->bits[i] = hexetToChar(temp);
	}
  }
  return aBigBits;
}

/** Return a new BigBits which is the bitwise-| of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *
orBigBits(const BigBits *bigBits1, const BigBits *bigBits2)
{
  
  BigBits *oBigBits = (BigBits *)malloc(sizeof(BigBits));
  if(bigBits1->size >= bigBits2->size){
	oBigBits->size = bigBits1->size;
	size_t offset = bigBits1->size - bigBits2->size;
	oBigBits->bits = (char *)calloc(bigBits1->size, sizeof(char));
	for(size_t i = 0; i < oBigBits->size;i++){
		int bigger,smaller, temp;
		bigger = charToHexet(bigBits1->bits[i]);
		if(i < offset){
			smaller = 0;
		}
		else{
			smaller = charToHexet(bigBits2->bits[i-offset]);
		}
		temp = bigger | smaller;
		oBigBits->bits[i] = hexetToChar(temp);
	}
  }
  else{
	oBigBits->size = bigBits2->size;
	size_t offset = bigBits2->size - bigBits1->size;
	oBigBits->bits = (char *)calloc(bigBits2->size, sizeof(char));
	for(size_t i = 0; i < oBigBits->size;i++){
		int bigger,smaller, temp;
		bigger = charToHexet(bigBits2->bits[i]);
		if(i < offset){
			smaller = 0;
		}
		else{
			smaller = charToHexet(bigBits1->bits[i-offset]);
		}
		temp = bigger | smaller;
		oBigBits->bits[i] = hexetToChar(temp);
	}
  }
  return oBigBits;
}

/** Return a new BigBits which is the bitwise-^ of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *
xorBigBits(const BigBits *bigBits1, const BigBits *bigBits2)
{
  BigBits *xBigBits = (BigBits *)malloc(sizeof(BigBits));
  if(bigBits1->size >= bigBits2->size){
	xBigBits->size = bigBits1->size;
	size_t offset = bigBits1->size - bigBits2->size;
	xBigBits->bits = (char *)calloc(bigBits1->size, sizeof(char));
	for(size_t i = 0; i < xBigBits->size;i++){
		int bigger,smaller, temp;
		bigger = charToHexet(bigBits1->bits[i]);
		if(i < offset){
			smaller = 0;
		}
		else{
			smaller = charToHexet(bigBits2->bits[i-offset]);
		}
		temp = bigger ^ smaller;
		xBigBits->bits[i] = hexetToChar(temp);
	}
  }
  else{
	xBigBits->size = bigBits2->size;
	size_t offset = bigBits2->size - bigBits1->size;
	xBigBits->bits = (char *)calloc(bigBits2->size, sizeof(char));
	for(size_t i = 0; i < xBigBits->size;i++){
		int bigger,smaller, temp;
		bigger = charToHexet(bigBits2->bits[i]);
		if(i < offset){
			smaller = 0;
		}
		else{
			smaller = charToHexet(bigBits1->bits[i-offset]);
		}
		temp = bigger ^ smaller;
		xBigBits->bits[i] = hexetToChar(temp);
	}
  }
  return xBigBits;
}
