#include <cctype>
#include <fstream>
#include <iostream> 
#include <vector> 
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <conio.h>
#include <cctype>
#include <windows.h>
using namespace std;


enum ByteConversion {
    
    HEX,
    OCT,
};


struct DisplayData {

    int columns; 
    ByteConversion display;
    int pageSize;
    int currentPage;
    int consoleWidth;
};

bool pageInData(int size, int page, DisplayData& dp);
void printPage(int page,vector<char> data,DisplayData& dp);
class Page {
    public: 
    vector<char> bytes;
    int size; 
    Page(int size) : size(size){}; 
        
   
    bool addByte(char b) {
        
        if(bytes.size() >= size) 
            return false;

        bytes.push_back(b);
        return true;

    }

};

class BinaryBook {
    
    vector<Page> pages;

    void fillBook(vector<char> data, DisplayData& dp) {
        Page current(dp.pageSize*dp.columns); 
       for(auto c : data) {
            if(!current.addByte(c)) {
                pages.push_back(current);
                current = Page(dp.pageSize*dp.columns);
            }
        }
    }


};

int getConsoleWidth();

DisplayData displaySettings {16,HEX};

vector<char> readFiledata(string path);

float entropy(vector<char> data);



int main(int argc, char** argv) {

    DisplayData dp; 
    if(argc < 2) {
        cout << "Please provide a filename" << endl;
        return 1;
    }
    auto binaryDaya = readFiledata(argv[1]);
     
    if(argc >= 3 ) {
        dp.pageSize = std::atoi(argv[2]);
    }
    dp.pageSize = 8;
    dp.columns = 8; 
    printPage(0, binaryDaya, dp);
    float entr = entropy(binaryDaya);
    cout << entr << endl;

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


float entropy(vector<char> data) {
    char values[255]= {0};
    for(auto val : data) {
        values[(int)val]++;
    }
    int sum =0;
    int totalWeight =0; 
    for(int value = 0; value < 255; value++) {

       sum += values[value]*value;
        totalWeight += values[value];
    }
    float avg = (float)sum/(float)totalWeight;
    
    return avg;
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;    
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 0;
}

bool pageInData(int size, int page, DisplayData& dp) {
    
    int bytesPerRow = dp.columns;
    int rowsPerPage = dp.pageSize;
    int totalBytesRequired = rowsPerPage*bytesPerRow*page;
    return size > totalBytesRequired;
}

void printPage(int page,vector<char> data,DisplayData& dp) {
    
    if(!pageInData(data.size(), page, dp)) {
        return;
    }   

    int rowCount = 0;
    string currentRow{""};
    int start = page*dp.columns*dp.pageSize;
    for(int i = start; i < start + dp.pageSize*dp.columns; i++) {
        if(i % dp.columns == 0) {
            cout << "    " << currentRow<< endl;
            currentRow = "";
            rowCount++;
            if(rowCount >= dp.pageSize) {
            rowCount = 0;
        }
        }
        char c = isprint(data[i]) ? data[i] : '.';
        currentRow += c;
        cout << hex << setfill('0') << setw(2) << static_cast<unsigned int>(255&data[i]) << " ";
    }
}

