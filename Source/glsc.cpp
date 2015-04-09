#ifndef _GLSC_SOURCE_GLSC_CPP_
#define _GLSC_SOURCE_GLSC_CPP_

#include <algorithm> 
#include <climits>
#include <cctype>
#include <functional> 

#include "glsc.h"

using namespace std;

GLSC::GLSC() {
    SearchStarts = { ' ', '(', '{' };
    SearchEnds = {
        { ' ', ' ' },
        { '{', '}' },
        { '(', ')' }
    };
}

string GLSC::ParseCommands(const vector<string>& commandsRaw) const {
    string output;
    pair<string, int> command;
    int numTabs = 0;
    size_t i;

    // Minimizes the amount of resizes the string will have to do later on...
    output.resize(commandsRaw.size() * 7);

    for (i = 0; i < commandsRaw.size(); i += 1) {
        if (commandsRaw[i].size() == 1) {
            output += "\n";
            continue;
        }

        command = ParseCommand(commandsRaw[i]);

        if (command.second == INT_MIN) {
            output += " " + command.first;
        }
        else if (command.second < 0) {
            numTabs += command.second;
            output += command.first;
        }
        else {
            output += command.first;
            numTabs += command.second;
        }
    }

    return output;
}

pair<string, int> GLSC::ParseCommand(const string& commandRaw) const {
    pair<string, int> output, result;
    string function, arguments;
    size_t colonIndex;

    colonIndex = commandRaw.find(':');

    if (colonIndex == string::npos) {
        throw "Command does not include colon: " + commandRaw;
    }

    function = trim(commandRaw.substr(0, colonIndex));
    arguments = trim(commandRaw.substr(colonIndex + 1));

    // parse arguments
    // return result of calling Function, with Function's wrapper's indent change

    return output;
}

vector<string> GLSC::ParseArguments(const string& argumentsRaw) const {
    vector<string> arguments;
    string argument;
    size_t i, end;

    for (i = 0; i < argumentsRaw.size(); i += 1) {
        const char& starter = argumentsRaw[i];

        if (isspace(starter)) {
            continue;
        }

        if (starter == '{' || starter == '(') {
            end = FindSearchEnd(argumentsRaw, starter, i);
            i += 1;
        } 
        else {
            end = FindNextSpace(argumentsRaw, i);
        }

        argument = argumentsRaw.substr(i, end - i);

        // Not yet tested!
        // if (starter == '{') {
        //     argument = ParseCommand(argument);
        // }

        arguments.push_back(argument);
        i = end;
    }

    return arguments;
}

size_t GLSC::FindNextSpace(const string& haystack, const size_t start) const {
    for (size_t i = start + 1; i < haystack.size(); i += 1) {
        if (isspace(haystack[i])) {
            return i;
        }
    }

    return haystack.size();
}

size_t GLSC::FindSearchEnd(const string& haystack, const char& searcher, const size_t start) const {
    size_t i;
    size_t numStarts = 1;
    const char& ender = SearchEnds.find(searcher)->second;

    for (i = start + 1; i < haystack.size(); i += 1) {
        const char& current = haystack[i];

        if (current == searcher) {
            numStarts += 1;
        }
        else if (current == ender) {
            numStarts -= 1;
            if (numStarts < 1) {
                return i;
            }
        }
    }

    return string::npos;
}

// Trim commands graciously donated by http://stackoverflow.com/a/217605/1830407

string GLSC::ltrim(string s) const {
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    return s;
}

string GLSC::rtrim(string s) const {
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

string GLSC::trim(string s) const {
    return ltrim(rtrim(s));
}

size_t GLSC::IndexOfAny(const string& haystack, const vector<char>& needles, size_t start = 0) const {
    size_t i, j;

    for (i = start; i < haystack.size(); i += 1) {
        for (j = 0; j < needles.size(); j += 1) {
            if (haystack[i] == needles[j]) {
                return i;
            }
        }
    }

    return string::npos;
}

#endif