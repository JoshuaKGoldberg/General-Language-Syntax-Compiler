#ifndef _GLSC_MAIN_CPP_
#define _GLSC_MAIN_CPP_

#include <cstdlib>
#include <iostream>
#include <string>

#include "glsc.h"
#include "Languages/JavaScript.h"

int main(int argc, char* argv[]) {
    GLSC tester;
    
    tester.RegisterJavaScript();

    cout << tester.ParseCommands("JavaScript", {
        "print line : (\"Hello world!\")",
        "",
        "print line : \"Bye!\""
        "print line : (\"Hello world!\") {print line : \"Bye!\"}",
    });

    return EXIT_SUCCESS;
}

#endif