
#define HTTP_PROCOTOL_11 11
#define HTTP_PROCOTOL_10 10
#define HTTP_PROCOTOL_9 9

int SetHttpRequest()
{
    char *pEnv = NULL;

    rHttpRequest.SetRequestTime(time(NULL));
    if(NULL == (pEnv = getenv("SERVER_PROTOCOL")))
		return -1;
    if(strcasecmp(pEnv, "HTTP/1.1" ) == 0)
	SetProtocol(HTTP_PROCOTOL_11);
    else if(strcasecmp(pEnv, "HTTP/1.0" ) == 0)
        SetProtocol(HTTP_PROCOTOL_10);
    else
        SetProtocol(HTTP_PROCOTOL_09);

    if((NULL == (pEnv = getenv("HTTP_X_FORWARDED_FOR"))) && (NULL == (pEnv = getenv("REMOTE_ADDR")))) //HTTP_X_FORWARDED_FOR proxy set ip
		return -1;

    if(NULL == (pEnv = getenv("SCRIPT_URL")))
		if(NULL == (pEnv = getenv("SCRIPT_NAME")))
			return -1;

    if(NULL != (pEnv = getenv("HTTP_REFERER")))
    if(NULL != (pEnv = getenv("HTTP_ACCEPT_ENCODING")))
    if(NULL != (pEnv = getenv("HTTP_ACCEPT_LANGUAGE")))
    if(NULL != (pEnv = getenv("REMOTE_PORT")))

    if(NULL != (pEnv = getenv("PATH")))
    if(NULL != (pEnv = getenv("HTTP_ACCEPT")))
    if(NULL == (pEnv = getenv("REQUEST_METHOD")))
    if(NULL == (pEnv = getenv("SCRIPT_URL")))
	if(NULL == (pEnv = getenv("SCRIPT_NAME")))
		return -1;


    if(NULL != (pTmp = getenv("HTTP_REFERER")))
    if(NULL != (pTmp = getenv("HTTP_ACCEPT_ENCODING")))
    if(NULL != (pTmp = getenv("HTTP_ACCEPT_LANGUAGE")))
    if(NULL != (pTmp = getenv("HTTP_COOKIE")))
    if(NULL != (pTmp = getenv("HTTP_USER_AGENT")))


        if(strcmp(pEnv, "POST") == 0)
        {
            rHttpRequest.SetMethodType(web_solution::comm::CHttpRequest::POST_METHOD_TYPE);
            if(NULL == (pEnv = getenv("CONTENT_LENGTH")))
                return -1;

            std::ios::sync_with_stdio(false);
            int iLen = atoi(pEnv);
            rHttpRequest.SetContentLength(iLen);

            char *pBuf = (char*)malloc(sizeof(char) * (iLen) + 1);
            std::cin.get(pBuf, iLen);
            /*if((int)fread(pBuf,sizeof(char),iLen,stdin) != iLen) */
            //{
                //free(pBuf);
                //return -1;
            //}
            pBuf[iLen] = '\0';
            rHttpRequest.SetPostInfo(pBuf, iLen);
            free(pBuf);

            if(NULL != (pEnv = getenv("QUERY_STRING")))
                rHttpRequest.SetQueryString(pEnv);
        }else if(strcmp(pEnv, "GET") == 0)
        {
	        rHttpRequest.SetMethodType(web_solution::comm::CHttpRequest::GET_METHOD_TYPE);
		if(NULL == (pTmp = getenv("QUERY_STRING")))
			return -1;
		rHttpRequest.SetQueryString(pTmp);
        }


}
