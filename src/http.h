#ifndef __MINISERVER_HTTPREQUEST_H
#define __MINISERVER_HTTPREQUEST_H

#include <map>
#include <string>
#include <cstring>

using std::string;
using std::map;

// 枚举http方法类型
enum HttpMethod{
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_RESP,
    HTTP_METHOD_NONE
};

struct RequestPacket{
    // http 方法类型
    HttpMethod method;
    
    // 请求包
    char* data;
    int datalen;

    // http 报头
    char* header;
    int headerlen;

    // 内容
    char* content;
    int contentlen;

    // IP，端口
    char* host;
    int hostlen;
    int port;

    // 统一资源标识符
    char* uri;
    int urilen;
    char* obj;
    int objlen;

    // 参数对象表
    map<string, string> param_table;

    RequestPacket(const char* _data = 0):method(HTTP_METHOD_NONE), data(NULL), datalen(0),header(NULL),
            headerlen(0), content(NULL), contentlen(0), host(NULL),hostlen(0), port(0), uri(NULL), 
            urilen(0), obj(NULL), objlen(0){
        datalen = strlen(_data);
        data = new char[datalen + 1];
        strcpy(data, _data);
    }

    ~RequestPacket(){
        delete[] data;

        uri = NULL;
        
        host = NULL;
        
        content = NULL;
        header = NULL;
        
        data = NULL;
    }
};

class HttpRequest{
    public:
        HttpRequest(char* data);
        HttpRequest(const string& data);
        ~HttpRequest();
        
        string getUri();
        string getRequestObj();

        string getHost();
        int getPort();
        
        HttpMethod getHttpMethod();
                
        string getParameterValues(string key);
        map<string, string> getParameterMap();

    private:
        RequestPacket* pct;
};

#endif
