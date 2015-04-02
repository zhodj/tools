/** 
* @file 
* @brief url����������
* 
* ��Ҫ�ṩurl�������صĲ���
* 
* @author 
* @version 0.1
* @date 
* @bug 
* @warning 
*/ 
#define __ENCODING_H__

#include <stdlib.h>
#include <string>

#include <stdint.h>

static void Encode(const char* pszStr, std::string& sResult);
static char* Decode(char* pszUrl);
static int DecodeChinese(const char* pszHexStr, char* pszStr, uint32_t dwStrLen);
static int DecodeChinese(const std::string& strEncodeString,std::string& strDecodeString);
#endif


