#include <fstream>
#include <iostream> 
#include <vector> 
#include <algorithm>
#include <iomanip>
using namespace std;


enum ByteConversion {
    
    HEX,
    OCT,
    INT32,
};

struct DisplayData {

    int columns; 
    ByteConversion display;


};



DisplayData displaySettings {16,HEX};

vector<char> readFiledata(string path);

int main(int argc, char** argv) {

   
    if(argc < 1) {
        cout << "Please provide a filename" << endl;
        return 1;
    }
    
    auto binaryDaya = readFiledata(argv[1]);
    
    for(int i = 0; i < binaryDaya.size(); i++) {
        if(i % displaySettings.columns == 0) {
            cout << endl;
        }
        cout << hex << setfill('0') << setw(2) << static_cast<unsigned int>(binaryDaya[i]) << " ";
    }

    return 0;
}


vector<char> readFiledata(string path) {
    
    vector<char> result;

    ifstream outputStream(path,ios::in|ios::binary);

    char c;
    while(outputStream.read(&c,sizeof(char))) {
        
        result.push_back(c);

    }
    return result;
}

