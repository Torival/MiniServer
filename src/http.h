#ifndef __MINISERVER_HTTP_H
#define __MINISERVER_HTTP_H

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
        
        init();
    }

    ~RequestPacket(){
        delete[] data;

        uri = NULL;
        obj = NULL;

        host = NULL;
        
        content = NULL;
        header = NULL;
        
        data = NULL;
    }

    void init();
};

class HttpRequest{
    public:
        HttpRequest();
        HttpRequest(const char* data);
        HttpRequest(const string& data);
        ~HttpRequest();
        
        int readData(const int fd);

        string getUri();
        string getRequestObj();

        string getData();
        string getContent();

        string getHost();
        int getPort();
        
        HttpMethod getHttpMethod();
                
        string getParameterValues(string key);
        map<string, string> getParameterMap();

    private:
        RequestPacket* pct;
};

class HttpResponse{
    public:
        HttpResponse(int fd, const string& path);
        HttpResponse(int fd, const char* path);
        ~HttpResponse();

        void response_file();

    private:
        // 返回400 请求解析失败,客户端代码错误
        void response_400();

        // 返回404 文件内容, 请求文件没有找见
        void response_404();

        // 返回501 错误, 不支持的请求
        void response_501();

        // 服务器内部错误,无法处理等
        void response_500();

        // 返回200 请求成功 内容, 后面可以加上其它参数,处理文件输出
        void response_200();

        int fd;
        string path;
};

#endif
