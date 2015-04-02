#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iconv.h>

char Hex2Char(const char *s)
{
	int value;
	int c;

	c = ((unsigned char *)s)[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = ((unsigned char *)s)[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (char)value;
}

char Hex2Char(const char* pszStr, uint32_t dwLen)
{
	int digit=0 ;
	int tmp=0 ;

	for (size_t i = 1; i < dwLen +1; i++)
	{
		tmp = ( pszStr[i] >= 'A' ? (( pszStr[i] &0xDF) - 'A') + 10 : (pszStr[i] - '0')) ;
		digit = digit * 16 + tmp;
	}

	return(char)digit;
}

int CheckHex(const char* pszHex, uint32_t dwLen)
{
	for (uint32_t i = 1; i < dwLen + 1; i++)
	{
		if (isxdigit(pszHex[i]))
			continue ;
		else
			return -1 ;
	}

	return 0 ;
}

char* Decode(char* pszUrl)
{
	assert(pszUrl);

	uint32_t iLen = strlen(pszUrl);

	char* dest = pszUrl;
	char* data = pszUrl;

	while (iLen--)
	{
		if (*data == '+')
			*dest = ' ';
		else if (*data == '%' && iLen >= 2 && isxdigit((int) *(data + 1)) && isxdigit((int) *(data + 2)))
		{
			*dest = Hex2Char(data + 1);
			data += 2;
			iLen -= 2;
		} else
			*dest = *data;

		data++;
		dest++;
	}
	*dest = '\0';

	return pszUrl ;
}

void Encode(const char* pszStr, std::string& sResult)
{
	assert(pszStr);

	static unsigned char HEXCHARS[] = "0123456789ABCDEF";
	register int x, y;

	uint32_t dwLen = strlen(pszStr); 
    if (dwLen == 0)
    {
        return;
    }
    
	//分配内存
	sResult.resize(3 * dwLen);
	//进行编码
	for (x = 0, y = 0; dwLen--; x++, y++)
	{
        sResult[y] = pszStr[x];
		if (pszStr[x] == ' ')
		{
			sResult[y] = '+';
		}
		else if ((pszStr[x] < '0' && pszStr[x] != '-' && pszStr[x] != '.') || 
			(pszStr[x] < 'A' && pszStr[x] > '9') ||
			(pszStr[x] > 'Z' && pszStr[x] < 'a' && pszStr[x] != '_') ||
			(pszStr[x] > 'z'))
		{
			sResult[y++] = '%';
			sResult[y++] = HEXCHARS[(unsigned char) pszStr[x] >> 4];
			sResult[y]   = HEXCHARS[(unsigned char) pszStr[x] & 15];
		}
	}
    
    //char chSpace = '0';
    char chSpace = 0;
	std::string::const_iterator it1, it2;

	for(it1 = sResult.begin(); it1 != sResult.end(); it1++)
	{
		if(*it1 != chSpace)
			break;
	}
	
	it2 = sResult.end() - 1;
	if(it2 == it1)
	{
		it2 = sResult.end();
	}
	else
	{
		for(; it2 >= it1; it2--)
		{
			if(*it2 != chSpace)
			{
				it2++;
				break;
			}
		}
	}
	
	sResult = std::string(it1, it2);
}

int DecodeChinese(const char* pszHexStr, char* pszStr, uint32_t dwStrLen)
{
	assert(pszHexStr);
	assert(pszStr);
	assert(dwStrLen);

	char buf[256 + 1] = {0};
	char tmp[256 + 1] = {0};
	char c ;
	int j = 0 ;

	snprintf(buf, sizeof(buf) - 1, "%s", pszHexStr);
	for (size_t i = 0; i < strlen(buf); i += j)
	{
		memset(tmp, 0x00, sizeof(tmp));
		switch (buf[i])
		{
		case '%' :
			if (buf[i + 1] == 'u')
			{
				if (CheckHex(buf + i + 1, 4)) 
					return -4;
				/*-----------------------------
				判断c的最高位是否为1,
				如果为1 ( 中文编码)，
				则做hex2char转换，否则只
				处理'(',')'的转换
				-------------------------------*/
				if (((c = Hex2Char(buf + i + 1, 4)) & 0x80) || c == 0x28 || c == 0x29)
				{
					snprintf(pszStr, dwStrLen, "%s%c", pszStr, c) ;
				} else
				{
					strncpy(tmp, buf + i + 2, 4) ; 
					snprintf(pszStr, dwStrLen, "%s%s", pszStr, tmp) ;
				}
				j = 6 ;
			} else
			{
				if (CheckHex(buf + i, 2))
					return -5;
				/*-----------------------------
				判断c的最高位是否为1,
				如果为1 ( 中文编码)，
				则做hex2char转换，否则只
				处理'(',')'的转换
				-------------------------------*/
				if (((c = Hex2Char(buf + i , 2)) & 0x80) || c== 0x28 || c == 0x29)
				{
					snprintf(pszStr, dwStrLen, "%s%c", pszStr, c) ;
				} else
				{
					strncpy(tmp, buf + i + 1, 2); 
					snprintf(pszStr, dwStrLen, "%s%s", pszStr, tmp);
				}
				j = 3 ;
			}
			break ;
		default:
			snprintf(pszStr, dwStrLen, "%s%c", pszStr, buf[i]) ;
			j = 1 ;
			break ;
		}
	}
	return 0 ;
}

int DecodeChinese(const std::string& strEncodeString,std::string& strDecodeString)
{
	strDecodeString="";
	uint32_t dwLastPos=0;
	while(dwLastPos<strEncodeString.length())
	{
		if(strEncodeString[dwLastPos]=='%')
		{
			if(dwLastPos+1>=strEncodeString.length())
			{
				return -4;
			}
			if(strEncodeString[dwLastPos+1]=='u')
			{
				if(dwLastPos+6>=strEncodeString.length())
				{
					return -4;
				}
				char szWord[5]={0};
				strncpy(szWord,strEncodeString.c_str()+dwLastPos+2,4);
				uint16_t wWord=strtoul(szWord,NULL,16);
				szWord[1]=wWord>>8;
				szWord[0]=wWord&0xFF;
				strDecodeString+=szWord[1];
				strDecodeString+=szWord[0];
				dwLastPos+=6;
			}
			else
			{
				if(dwLastPos+3>=strEncodeString.length())
				{
					return -4;
				}
				char szWord[3]={0};
				strncpy(szWord,strEncodeString.c_str()+dwLastPos+1,2);
				szWord[0]=(char)strtoul(szWord,NULL,16);
				strDecodeString+=szWord[0];
				dwLastPos+=3;
			}
		}
		else
		{
			strDecodeString+=strEncodeString[dwLastPos];
			dwLastPos++;
		}
	}
	return 0;
}

