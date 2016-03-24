#include <SocketProtocol.h>
#include <SocketUtil.h>


#define GET_ARRAY_LENGTH(arr)  sizeof(arr)/sizeof(arr[0])
#define SAFE_RELEASE(ptr) \
{\
	delete ptr;\
	ptr=NULL;\
}\


//		char* test="1234";
//
//		int len;
//		char* str1=SocketProtocol::transferLocalStreamToNetWorkStream(test,len);
//
//		int size=SocketProtocol::getNetWorkStreamLengthWidthHead(str1);
//
//		cout<<len<<endl;
//		cout<<size<<endl;
//		cout<<SocketProtocol::getNetWorkStreamLength(str1)<<endl;
//
//		char* str2=SocketProtocol::transferNetWorkStreamToLocalStream(str1,size);
//
//		cout<<str2<<endl;
//
//		SAFE_RELEASE(str1);
//		SAFE_RELEASE(str2);
