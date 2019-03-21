#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include"Http.h"

//////////////////////////////////////// HttpPoster /////////////////////////////////
void HttpPoster::UpdateInfo()
{
    char szJsonData[1024];
    char sendbuf[] = "{\"abc\":\"hello\"}";
    memset(szJsonData, 0, sizeof(szJsonData));
    std::string strJson;
    strJson = "{";
    strJson += "\"user_name\" : \"test\",";
    strJson += "\"password\" : \"test123\"";
    strJson += "}";
    strcpy(szJsonData, strJson.c_str());
    try
    {
        CURL *pCurl = NULL;
        CURLcode res;
        // In windows, this will init the winsock stuff
        curl_global_init(CURL_GLOBAL_ALL);

        // get a curl handle
        pCurl = curl_easy_init();
        if (NULL != pCurl)
        {
            // 设置超时时间为1秒
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 1);

            // First set the URL that is about to receive our POST.
            // This URL can just as well be a
            // https:// URL if that is what should receive the data.
            curl_easy_setopt(pCurl, CURLOPT_URL, "http://10.10.80.11:8080");
            //curl_easy_setopt(pCurl, CURLOPT_URL, "http://192.168.0.2/posttest.cgi");

            // 设置http发送的内容类型为JSON
            curl_slist *plist = curl_slist_append(NULL,
                "Content-Type:application/json;charset=UTF-8");
            curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);

            // 设置要POST的JSON数据
            curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, sendbuf);

            // Perform the request, res will get the return code
            res = curl_easy_perform(pCurl);
            // Check for errors
            if (res != CURLE_OK)
            {
                printf("curl_easy_perform() failed:%s\n", curl_easy_strerror(res));
            }
            // always cleanup
            curl_easy_cleanup(pCurl);
        }
        curl_global_cleanup();
    }
    catch (std::exception &ex)
    {
        printf("curl exception %s.\n", ex.what());
    }

}


