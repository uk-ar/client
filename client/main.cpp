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

#define PORT 8080
#define BUFF_SIZE 1024
using namespace std;

// > client read
// content
// > client write "hello world"
// ok
void print_usage(string file_name){
    cout << file_name << " ip read" << endl;
    cout << file_name << " ip write content" << endl;
}
int main(int argc,const char *argv[]){
    vector<string> args(argv, argv + argc);
    if(args.size() < 3){
        cerr << "Too few args" << endl;
        print_usage(args[0]);
        return EXIT_FAILURE;
    }
    // cout << argv[0] <<endl;
    struct  in_addr sin_addr;
    if (inet_aton(argv[1], &sin_addr) == 0){
            cerr << "Invalid IP address: " << argv[1] << " : " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }
    string command = args[2];
    
    if(command != "read" && command != "write"){
        cerr << "Invalid command: " << args[2] << endl;
        print_usage(args[0]);
        return EXIT_FAILURE;
    }
    
    if(command == "write" && args.size() != 4){
        cerr << "Too few args: " << endl;
        print_usage(args[0]);
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port   = htons(PORT),
        .sin_addr   = sin_addr,
    };
    // struct sockaddr_in server_addr;

    int client = socket(AF_INET, SOCK_STREAM, 0);
    if(client == -1){
        cerr << "Socket creation failed: " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }

    if(connect(client,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
        cerr << "Socket creation failed: " << strerror(errno) << endl;
        return EXIT_FAILURE;
    }
    char buf[1024];

    if(command == "read"){
        snprintf(buf,sizeof(buf),"read \r\n");
        send(client, buf, strlen(buf),0);
        ssize_t n = recv(client,buf,1024,0);
        string cont(buf,n);
        cout << cont << endl;
        // TODO large data
        /*while(n>0){
            cout << buf;
            n = recv(client,buf,1024,0);
        }*/
    }
    else if(command == "write"){
        snprintf(buf,sizeof(buf),"%s\r\n%s\r\n","write",argv[3]);
        send(client, buf, strlen(buf),0);
        //cout <<"send:"<< buf << ":send" << endl;
        ssize_t n = recv(client,buf,1024,0);
        string cont(buf,n);
        cout << cont << endl;
        //cout <<"reply:"<< cont << ":reply" << endl;
        //cout << n << endl;
    }
}
