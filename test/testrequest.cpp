#include "../src/http.h"
#include "../src/http.cpp"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

int main(){
	
    char data[] = {
"GET /asd/asdfds/dfg/wer/bb.html?asd=123&sdf=324 HTTP/1.1\r\n\
Host: 172.17.36.103:8081\r\n\
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:52.0) Gecko/20100101 Firefox/52.0\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
Accept-Language: en-US,en;q=0.5\r\n\
Accept-Encoding: gzip, deflate\r\n\
Connection: keep-alive\r\n\
Upgrade-Insecure-Requests: 1\r\n\
\r\n\r\n"};
	
	HttpRequest rqt(data);
	
	cout << "Request Packet:\n----------------\n"
	 	 << data <<"----------------" << endl << endl;
    
	map<string, string> mp = rqt.getParameterMap();
	
	cout << "URI: " << rqt.getUri() << endl << endl;
	
	cout << "OBJ: " << rqt.getRequestObj() << endl << endl;
	
	cout << "HOST: " << rqt.getHost() << endl << endl;
	
	cout << "PORT: " << rqt.getPort() << endl << endl;
	
	cout << "PARAM_MAP:\n";
	for(map<string, string>::iterator it = mp.begin(); it != mp.end(); ++it)
		cout << "key: " << it->first << ", value: " << it->second<<endl;
	
    return 0;

}
