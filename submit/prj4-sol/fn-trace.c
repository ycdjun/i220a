#include "fn-trace.h"
#include "x86-64_lde.h"

#include "memalloc.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "beaengine/BeaEngine.h"	

enum {
  CALL_OP = 0xE8,
  RET_FAR_OP = 0xCB,
  RET_FAR_WITH_POP_OP = 0xCA,	
  RET_NEAR_OP = 0xC3,
  RET_NEAR_WITH_POP_OP = 0xC2
};

static inline bool is_call(unsigned op) { return op == CALL_OP; }
static inline bool is_ret(unsigned op) {
  return
    op == RET_NEAR_OP || op == RET_NEAR_WITH_POP_OP ||
    op == RET_FAR_OP || op == RET_FAR_WITH_POP_OP;
}

int isPresent(void * addr, FnsData * fnsData){
	for(int i = 0; i<fnsData->next; i++){
		if(fnsData->data[i]->address == addr){
			fnsData->data[i]->nInCalls++;
			return 1;
		}
	}
	return 0;
}
const FnsData * fn_trace(void *addr, FnsData * fnsData){
   int index = fnsData->next;
   
   if(fnsData->next >= fnsData->size){
   	fnsData->data = reallocChk(fnsData->data, sizeof(FnInfo)*2*fnsData->size);
   	if(fnsData->size == 0){
   		fnsData->size = 1;
   	}
   	else{
   		fnsData->size = 2 * fnsData->size;
   	}
   }
   
   fnsData->data[index] = mallocChk(sizeof(FnInfo));
   fnsData->data[index]->address = addr;
   fnsData->data[index]->length = 0;
   fnsData->data[index]->nInCalls = 1;
   fnsData->data[index]->nOutCalls = 0;
   fnsData->next++;
   
   unsigned char * scan_instr = addr;
   bool looper = false;
   while(!looper){
   	int op_len = get_op_length(scan_instr);
   	fnsData->data[index]->length+=op_len;
   	if(is_call(*scan_instr)){
   		fnsData->data[index]->nOutCalls++;
   		int *offset = (int *) (scan_instr + 1 );
   		void * address = *offset + scan_instr + op_len;
   		if(isPresent(address, fnsData) == 0){
   			fn_trace(address, fnsData);
   		}
   	}
   	if(is_ret(*scan_instr)){
   		looper = true;
   	}
	scan_instr += op_len;
	addr += op_len;
   }
   return fnsData;
  
}



int cmp_addr(const void * obj1, const void * obj2){
	FnInfo ** fninfo1 = (FnInfo **)obj1;
	FnInfo ** fninfo2 = (FnInfo **)obj2;
	return ((*fninfo1)->address - (*fninfo2)->address);
}

/** Return pointer to opaque data structure containing collection of
 *  FnInfo's for functions which are callable directly or indirectly
 *  from the function whose address is rootFn.
 */

const FnsData *
new_fns_data(void *rootFn)
{
  //verify assumption used when decoding call address
  assert(sizeof(int) == 4);
  FnsData *new = mallocChk(sizeof(FnsData));
  new->data = NULL;
  new->size = 0;
  new->next = 0;
  fn_trace(rootFn, new);
  qsort(new->data, new->next, sizeof(FnInfo *), cmp_addr); 
  return new;
}


/** Free all resources occupied by fnsData. fnsData must have been
 *  returned by new_fns_data().  It is not ok to use to fnsData after
 *  this call.
 */
void
free_fns_data(FnsData *fnsData)
{
  for(int i = 0; i < fnsData->next; i++){
  	free(fnsData->data[i]);
  }
  
  free(fnsData->data);
  free(fnsData);
}

/** Iterate over all FnInfo's in fnsData.  Make initial call with NULL
 *  lastFnInfo.  Keep calling with return value as lastFnInfo, until
 *  next_fn_info() returns NULL.  Values must be returned sorted in
 *  increasing order by fnInfoP->address.
 *
 *  Sample iteration:
 *
 *  for (FnInfo *fnInfoP = next_fn_info(fnsData, NULL); fnInfoP != NULL;
 *       fnInfoP = next_fn_info(fnsData, fnInfoP)) {
 *    //body of iteration
 *  }
 *
 */
const FnInfo *
next_fn_info(const FnsData *fnsData, const FnInfo *lastFnInfo)
{
  if(lastFnInfo == NULL){
  	return fnsData->data[0];
  }
  int i = 0;
  while(fnsData->data[i] != lastFnInfo){
  	i++;
  }
  return fnsData->data[i+1];
}
