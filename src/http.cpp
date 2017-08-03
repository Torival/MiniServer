#include <iostream>
#include "http.h"

HttpRequest::HttpRequest(const string& _data):HttpRequest(_data.c_str()){

}

HttpRequest::HttpRequest(char* _data):pct(NULL){
    if(_data == NULL)
        return ;

    // 初始化Request请求包
    pct = new RequestPacket(_data);
    char* cur = pct->data;
    
    // 初始化Header
    pct->header = pct->data;
    pct->headerlen = strstr(cur, "\r\n\r\n") - pct->header;

    // 初始化内容
    pct->content = pct->header + pct->headerlen + 4;
    pct->contentlen = pct->datalen - (pct->headerlen + 4);

    // 判断是GET，还是POST方法
    if(strncmp(cur, "GET ", 4) == 0){
        cur += 4;
        pct->method = HTTP_METHOD_GET;
    } else if(strncmp(cur, "POST ", 5) == 0){
        cur += 5;
        pct->method = HTTP_METHOD_POST;
    } else{
        return ;
    }

	
    // 初始化资源标识符
    pct->uri = cur;
    while(*cur != '?' && *cur != ' ')
        ++cur;
    pct->urilen = cur - pct->uri;
	
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
            pct->param_table[string(key, keylen)] = string(value, valuelen);
        }
    }
    
    // 初始化主机IP
    cur = strstr(cur,"Host");
    cur += 6;
    pct->host = cur;
    while(*cur != ':')
    	++cur;
    pct->hostlen = cur - pct->host;
    
    
    
    // 初始化主机端口
    ++cur;
    pct->port = atoi(cur);
}

HttpRequest::~HttpRequest(){
    delete pct;
}

string HttpRequest::getUri(){
    return string(pct->uri, pct->urilen);
}

string HttpRequest::getRequestObj(){
    // 从URI中，初始化请求对象
    if(pct->obj == NULL){
        pct->obj = pct->uri + pct->urilen;
        while(*(pct->obj - 1) != '/'){
            ++pct->objlen;
            --pct->obj;
        }
    }
    return string(pct->obj, pct->objlen);
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


