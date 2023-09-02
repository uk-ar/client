//
//  client.cpp
//  server
//
//  Created by 有澤悠紀 on 2023/09/02.
//

#include <sys/socket.h>
#include <netinet/in.h> // for struct_in
#include <arpa/inet.h> // for inet_aton
#include <iostream>
#include <filesystem>

#define PORT 8080
#define BUFF_SIZE 1024
using namespace std;
namespace fs = std::filesystem;

// > client read
// content
// > client write "hello world"
// ok
void print_usage(fs::path path){
    string file_name = path.filename();
    cout << "usage:" << endl;
    cout << file_name << " ip read" << endl;
    cout << file_name << " ip write content" << endl;
    cout << file_name << " ip insert content position" << endl << endl;
    cout << "example:" << endl;
    cout << file_name << " 127.0.0.1 read              'read data from localhost'" << endl;
    cout << file_name << " 127.0.0.1 write content     'write \"content\" to the file in localhost'" << endl;
    cout << file_name << " 127.0.0.1 insert content 3  'insert \"content\" at offset 3 from the beginning" << endl;
}
int main(int argc,const char *argv[]){
    vector<string> args(argv, argv + argc);
    string command = args[2];
    if((args.size() < 3) ||
       (command == "write" && args.size()  != 4) ||
       (command == "insert" && args.size() != 5)){
        cerr << "Too few args" << endl;
        print_usage(args[0]);
        return EXIT_FAILURE;
    }

    struct  in_addr sin_addr;
    if (inet_aton(argv[1], &sin_addr) == 0){
            cerr << "Invalid IP address: " << argv[1] << " : " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }
    
    if(command != "read" && command != "write" && command != "insert"){
        cerr << "Invalid command: " << args[2] << endl;
        print_usage(args[0]);
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port   = htons(PORT),
        .sin_addr   = sin_addr,
    };

    int client = socket(AF_INET, SOCK_STREAM, 0);
    if(client == -1){
        cerr << "Socket creation failed: " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }

    if(connect(client,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
        cerr << "Socket creation failed: " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }
    char buf[BUFF_SIZE];

    if(command == "read"){
        snprintf(buf,sizeof(buf),"read \r\n");
    }else if(command == "write"){
        snprintf(buf,sizeof(buf),"%s\r\n%s","write",argv[3]);
    }else if(command == "insert"){
        try {
            stoll(args[4]);
        }catch(invalid_argument){
            cerr << "Invalid number: " << args[4] << endl;
            return EXIT_FAILURE;
        }
        // position is limited to 64bit integer
        snprintf(buf,sizeof(buf),"%s\n%20s\r\n%s","insert",argv[4],argv[3]);
    }
    send(client, buf, strlen(buf),0);
    ssize_t n = recv(client,buf,BUFF_SIZE,0);
    string cont(buf,n);
    cout << cont << endl;
    // TODO: support large data
}
