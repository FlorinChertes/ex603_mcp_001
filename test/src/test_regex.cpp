#include <iostream>
#include <regex>
#include <string>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_regex_001()
{
    std::cout << "\n*** test regex 001 ***" << std::endl;

    // The text in which we'll search
    std::string text = "Hello, World!";

    // A simple pattern that looks for the word "World"
    std::regex pattern("World");

    // Check if the pattern is found in the text
    if (std::regex_search(text, pattern)) {
        std::cout << "Found 'World'!" << std::endl;
    }
    else {
        std::cout << "'World' not found." << std::endl;
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_regex_002()
{
    std::cout << "\n*** test regex 002 ***" << std::endl;

    // Sample JavaScript code
    std::string script = R"(
        var value_1 = "ex_1";
        var value_2 = "ex_2";
        var isEq = value_1.equals(value_2);
    )";

    // Regex pattern to capture two variable names separated by .equals(...)
    // (\w+) captures the first variable name (letters, digits, underscore)
    // \.equals\( matches the literal ".equals("
    // (\w+) captures the second variable name
    // \) matches the closing parenthesis
    std::regex pattern(R"((\w+)\.equals\((\w+)\))");

    // We will store the match results in 'matches'
    std::smatch matches;

    // Use regex_search to find the pattern in the entire script
    if (std::regex_search(script, matches, pattern)) {
        // matches[0] is the entire match "value_1.equals(value_2)"
        // matches[1] is "value_1", matches[2] is "value_2"
        std::cout << "Found a match: " << matches[0] << "\n";
        std::cout << "First variable name:  " << matches[1] << "\n";
        std::cout << "Second variable name: " << matches[2] << "\n";
    }
    else {
        std::cout << "No match found.\n";
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_regex_002_1()
{
    std::cout << "\n*** test regex 002_1 ***" << std::endl;

    // Sample JavaScript code
    std::string script = R"(
        var value_1 = "ex_3.ex_1";
        var value_2 = "ex_2";
        var isEq = value_1.equals(value_2);
    )";

    std::regex pattern(R"((?:^|^.)(\w+)\.equals\((\w+)\))");

    // We will store the match results in 'matches'
    std::smatch matches;

    // Use regex_search to find the pattern in the entire script
    if (std::regex_search(script, matches, pattern)) {
        std::cout << "Found a match: " << matches[0] << "\n";
        std::cout << "First variable name:  " << matches[1] << "\n";
        std::cout << "Second variable name: " << matches[2] << "\n";
    }
    else {
        std::cout << "No match found.\n";
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_regex_003()
{
    std::cout << "\n*** test regex 003 ***" << std::endl;

    // Sample JavaScript code
    std::string script =
R"(
 cat is good
cat is bad
)";

    std::regex pattern(R"(^cat)");

    // We will store the match results in 'matches'
    std::smatch matches;

    // Use regex_search to find the pattern in the entire script
    if (std::regex_search(script, matches, pattern)) {
        std::cout << "Found a match: " << matches[0] << "\n";
    }
    else {
        std::cout << "No match found.\n";
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_regex_004()
{
    std::cout << "\n*** test regex 004 ***" << std::endl;

    // Sample JavaScript code
    std::string script =
        R"(
 cat is good
cat is bad, this is a god dog
)";

    std::regex pattern(R"(dog$)");

    // We will store the match results in 'matches'
    std::smatch matches;

    // Use regex_search to find the pattern in the entire script
    if (std::regex_search(script, matches, pattern)) {
        std::cout << "Found a match: " << matches[0] << "\n";
    }
    else {
        std::cout << "No match found.\n";
    }
}
