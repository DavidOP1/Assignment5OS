#include "stack.cpp"
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

int main()
{
    PUSH("hey");
    assert(TOP() == "hey");
    PUSH("hello");
    POP();
    assert(TOP() == "hey");

    for (size_t i = 0; i < 3; i++)
    {
        PUSH("jermy");
    }

    POP();
    POP();
    assert(TOP() == "jermy");
    POP();
    assert(TOP() == "hey");

    for (size_t i = 0; i < 30; i++)
    {
        PUSH("he");
        POP();
    }

    assert(TOP() == "hey");

        cout << "Test has been done successfully" << endl;
}
