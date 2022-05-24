#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// converts hex -> dec
int fromHex(string hex) {
    unsigned int x;
    stringstream ss;
    ss << std::hex << hex;
    ss >> x;
    return static_cast<int>(x);
}

// converts dec -> ascii
char toAscii(int x) {
    return static_cast<char>(x);
}


char* query_string_var;


main() {
    query_string_var = getenv("QUERY_STRING");

    ofstream newFile ("example.txt");

    if (newFile.is_open()) {
      newFile << query_string_var;
      newFile.close();
    } else cout << "Unable to open file";

    string inLine,storage;

    // read and save query string (Pascal program) as C++ string
    ifstream queryFile("example.txt");
    if (queryFile.is_open()) {
        while (getline(queryFile, inLine))
        {
            storage += inLine + "\n"; // should be single line
        }
        queryFile.close();
    } else cout << "Unable to open file";

    // remove query string assignment
    storage = storage.substr(2, storage.length());


    for (int i = 0; i < storage.length(); i++) {
        // replace hex with ascii equivalent in query param
        if (storage[i] == '%') {
            string hex;
            hex.push_back(storage[i + 1]); 
            hex.push_back(storage[i + 2]);
            string replacement;
            replacement.push_back(toAscii(fromHex(hex)));
            storage.replace(i, 3, replacement);
        }
        // replace space in query peram
        if (storage[i] == '+') {
            storage.replace(i, 1, " ");
        }
    }
    // remove all occurances of shift in (015) = \r
    storage.erase( std::remove(storage.begin(), storage.end(), '\r'), storage.end() );

    ofstream pascalFile ("program.pas");

    if (pascalFile.is_open()) {
      pascalFile << storage;
      pascalFile.close();
    } else cout << "Unable to open file";

    system("./p02 program.pas");

    ifstream trcFile("program.trc");
    string inLine2, inLine3;
    ifstream outFile("out.txt");
    printf("Content-type: text/html\n\n");
    printf("<html>");
    printf("<body>");
     if(outFile.is_open()){
         while(getline(outFile, inLine3)){
             cout <<"<p>"+inLine3+"</p>"<<endl;
         }
         outFile.close();
         system("echo -n > out.txt");
     }
     if(trcFile.is_open()){
        while(getline(trcFile, inLine2)){
            cout <<"<p>"+inLine2+"</p>" << endl;
        }
        trcFile.close();
    }
    printf("</body></html>\n");
}
