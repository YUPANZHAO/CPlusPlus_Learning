#include <iostream>
#include <unistd.h>

using namespace std;

extern char **environ;

int main() {

    char **p;
    for(p=environ; *p != NULL; ++p) {
        cout << *p << endl;
    }
    
    return 0;
}