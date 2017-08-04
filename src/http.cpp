#include <iostream>
#include "http.h"

void RequestPacket::init() {
    char* cur = data;
    
    // 初始化Header
    header = data;
    headerlen = strstr(cur, "\r\n\r\n") - header;

    // 初始化内容
    content = header + headerlen + 4;
    contentlen = datalen - (headerlen + 4);

    // 判断是GET，还是POST方法
    if(strncmp(cur, "GET ", 4) == 0){
        cur += 4;
        method = HTTP_METHOD_GET;
    } else if(strncmp(cur, "POST ", 5) == 0){
        cur += 5;
        method = HTTP_METHOD_POST;
    } else{
        return ;
    }

	
    // 初始化资源标识符
    uri = cur;
    while(*cur != '?' && *cur != ' ')
        ++cur;
    rilen = cur - uri;
	
    // 初始化参数表
    if(*cur == '?'){
        char *key, *value;
        int keylen, valuelen;
		
        while(*cur == '?' || *cur == '&'){
        	++cur;
            key = cur;
            while(*cur != '=')
                ++cur;
            keylen = cur - key;

            ++cur;
            value = cur;
            while(1){
                if(*cur == ' ' || *cur == '&')
                    break; 
                ++cur;
            } 
            valuelen = cur - value;
            param_table[string(key, keylen)] = string(value, valuelen);
        }
    }
    
    // 初始化主机IP
    cur = strstr(cur,"Host");
    cur += 6;
    host = cur;
    while(*cur != ':')
    	++cur;
    hostlen = cur - host;
    
    // 初始化主机端口
    ++cur;
    port = atoi(cur);

    // 从URI中，初始化请求对象
    if(obj == NULL){
        obj = uri + urilen;
        while(*(obj - 1) != '/'){
            ++objlen;
            --obj;
        }
    }
}

HttpRequest::HttpRequest(const string& _data):HttpRequest(_data.c_str()){

}

HttpRequest::HttpRequest(const char* _data):pct(NULL){
    if(_data == NULL)
        return ;

    // 初始化Request请求包
    pct = new RequestPacket(_data);
}

HttpRequest::~HttpRequest(){
    delete pct;
}

string HttpRequest::getUri(){
    return string(pct->uri, pct->urilen);
}

string HttpRequest::getRequestObj(){
    return string(pct->obj, pct->objlen);
}

string HttpRequest::getData(){
    return string(pct->data, pct->datalen);
}

string HttpRequest::getContent(){
    return string(pct->content, pct->contentlen);
}

string HttpRequest::getHost(){
    return string(pct->host, pct->hostlen);
}

int HttpRequest::getPort(){
    return pct->port;
}

HttpMethod HttpRequest::getHttpMethod(){
    return pct->method;
}
        
string HttpRequest::getParameterValues(string key){
    for(auto it = pct->param_table.begin(); it != pct->param_table.end(); ++it){
        if(it->first == key) {
            return it->second;
        }
    }

    return "";
}

map<string, string> HttpRequest::getParameterMap(){
    return pct->param_table;
}


HttpResponse::HttpResponse(int _fd, const string& _path){
    fd = _fd;
    path = "./web" + _path;
}

HttpResponse::HttpResponse(int _fd, const char* _path){
    fd = fd;
    path = "./web" + string(_path);
}

HttpResponse::~HttpResponse(){

}

void HttpResponse::response_file(){
    
    if(access(path, F_OK) == 0)
        response_200();
    else
        response_404();    
}


void HttpResponse::response_400(){
    
    string head, content;
    char buf[4096];
    int count = 0;

    FILE* txt = fopen("./web/404.html", "r");
    check_return(txt == NULL, "open file fail");
    

    while(!feof(txt) && fgets(buf, sizeof(buf), txt)){
        count += strlen(buf);
        content += buf;
    }
    
    head  = "HTTP/1.1 404 Not Found\r\n";
	head += "Server: Mini_Server\r\n";
	head += "Content-Type: text/html; charset=UTF-8\r\n";
	head += "Content-Length: " + count + "\r\n",
	head += "Connection: keep-alive\r\n";
	head += "\r\n";

    write(fd, head.c_str(), head.length());
    write(fd, content.c_str(), content.length());
}

void HttpResponse::response_404(){
    
}

void HttpResponse::response_501(){
    
}

void HttpResponse::response_500(){
    
}

void HttpResponse::response_200(){
    string head, content;
    char buf[4096];
    int count = 0;

    FILE* txt = fopen(path, "r");
    check_return(txt == NULL, "open file fail");
    

    while(!feof(txt) && fgets(buf, sizeof(buf), txt)){
        count += strlen(buf);
        content += buf;
    }
    
    head  = "HTTP/1.1 200 OK\r\n";
	head += "Server: Mini_Server\r\n";
	head += "Content-Type: text/html; charset=UTF-8\r\n";
	head += "Content-Length: " + count + "\r\n",
	head += "Connection: keep-alive\r\n";
	head += "\r\n";

    write(fd, head.c_str(), head.length());
    write(fd, content.c_str(), content.length());
}