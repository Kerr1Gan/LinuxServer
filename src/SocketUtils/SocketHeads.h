#include <SocketProtocol.h>
#include <SocketUtil.h>




#define GET_ARRAY_LENGTH(arr)  sizeof(arr)/sizeof(arr[0])
#define SAFE_RELEASE(ptr) \
{\
	delete ptr;\
	ptr=NULL;\
}\
