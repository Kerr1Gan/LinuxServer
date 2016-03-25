/*
 * Macro.h
 *
 *  Created on: 2016年3月25日
 *      Author: xiang
 */

#ifndef SRC_MACRO_H_
#define SRC_MACRO_H_

#define GET_ARRAY_LENGTH(arr)  sizeof(arr)/sizeof(arr[0])

#define SAFE_RELEASE(pt)  \
{\
	if(pt!=NULL)\
	{\
		delete pt;\
		pt=NULL; \
	}\
}\



#endif /* SRC_MACRO_H_ */
