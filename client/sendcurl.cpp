#include "sendcurl.h"

sendCurl::sendCurl() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    //конфиг, чтобы работал с JSON
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

};

sendCurl::~sendCurl() {
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    curl_formfree(formpost);
    curl_slist_free_all(headerlist);
}

size_t sendCurl::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}


std::string sendCurl::curlSendData(std::string& url, const char* computerName) {
    TCHAR tempDirectory[MAX_PATH];
    GetTempPath(MAX_PATH, tempDirectory);

    std::string temp = std::string(tempDirectory) + "\\_CL_ff7649205141kq.tmp";
    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());


        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "image", CURLFORM_FILE, temp.c_str(), CURLFORM_END);

        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "computerName", CURLFORM_COPYCONTENTS, computerName, CURLFORM_END);

        headerlist = curl_slist_append(headerlist, "Content-Type: multipart/form-data");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            Beep(1000, 1000);
        }
        else {
     
            std::cout << "Server response: " << response << std::endl;
        }
        return response;


       
    }
}

std::string sendCurl::curlEmptyCallBack( std::string& url, const char* computerName) {
    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());


 
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "computerName", CURLFORM_COPYCONTENTS, computerName, CURLFORM_END);

        headerlist = curl_slist_append(headerlist, "Content-Type: multipart/form-data");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            for (int i = 0; i < 3; i++) {
                Beep(700, 100);
                Sleep(100);
            }
            
        }
        else {
            
            std::cout << "Server response: " << response << std::endl;
        }
        return response;



    }
};