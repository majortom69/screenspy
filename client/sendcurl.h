#ifndef SENDCURL_CLASS_H
#define SENDCURL_CLASS_H
#define CURL_STATICLIB

#include <curl/curl.h>
#include <string>
#include <iostream>

class sendCurl
{
public:
	sendCurl();
	~sendCurl();
	


	/// <summary>
	/// Отправка данных на сервер без встроенного шифрования
	/// </summary>
	/// <param name="data"></param>
	/// <param name="url"></param>
	std::string curlSendData(std::string &url, const char* computerName);
	std::string curlEmptyCallBack(std::string& url, const char* computerName);

	

private:
	
	CURL* curl;
	CURLcode res;
	struct curl_httppost* formpost = NULL;
	struct curl_httppost* lastptr = NULL;
	struct curl_slist* headerlist = NULL;


	// Для обработки ответа с сервера
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data);
};

#endif