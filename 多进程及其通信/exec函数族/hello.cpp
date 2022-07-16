#include <iostream>
using namespace std;

int main(int argn, char *argv[]) {

    if(argn <= 1) {
        cout << "Usage: hello name" << endl;
        return -1;
    }

    cout << "Hello " << argv[1] << "!" << endl;
    
    return 0;
}