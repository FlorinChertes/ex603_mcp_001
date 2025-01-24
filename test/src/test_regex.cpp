#include <iostream>
#include <regex>

#include <vector>
#include <string>
#include <cassert>

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

void check_pattern(const std::string& script)
{
    std::regex pattern(R"((\w+)\.equals\(\"(\w+)\"\))");

    // We will store the match results in 'matches'
    std::smatch matches;

    // Use regex_search to find the pattern in the entire script
    if (std::regex_search(script, matches, pattern)) {
        std::cout << "Found a match: " << matches[0] << "\n";
        std::cout << "First variable name:  " << matches[1] << "\n";
        std::cout << "Second variable name: " << matches[2] << "\n";

        const std::string::size_type start = 0;
        const std::string::size_type pos_match = script.find(matches[0], start);
        const std::string::size_type pos_first_point = script.find_first_of(".", start);

        assert(pos_match != std::string::npos);
        if (pos_first_point != std::string::npos
            && pos_first_point < pos_match)
        {
            std::cout << "Regect this match." << "\n";
        }
        else
        {
            std::cout << "Accept this match." << "\n";
        }

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

    const std::string script_reject = R"(var isEq = value3.value_1.equals("the_value_2");)";
    check_pattern(script_reject);
    const std::string script_accept = R"(var isEq = value_1.equals("the_value_2");)";
    check_pattern(script_accept);
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::vector<std::string> split_js_function(const std::string& input)
{
    std::vector<std::string> lines;
    std::string::size_type start = 0;

    while (true)
    {
        // Look for the literal "\n"
        std::string::size_type pos = input.find("\\n", start);
        if (pos == std::string::npos)
        {
            // No more "\n" found, push the remainder of the string
            lines.push_back(input.substr(start));
            break;
        }
        else
        {
            // Push the substring before "\n"
            lines.push_back(input.substr(start, pos - start));
            // Move start to the first character after the literal "\n"
            start = pos + 2;
        }
    }

    return lines;
}

void test_regex_005()
{
    std::cout << "\n*** test regex 005 ***" << std::endl;

    std::string the_input_string =
        R"(  var variable_01 = "var_01";\n  var isEq = value3.value_1.equals("the_value_2");\n  var variable_03 = variable_01 + variable_02;)";
    const std::vector<std::string>& lines = split_js_function(the_input_string);
    for (const auto& line : lines)
        std::cout << line << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::vector<std::string> select_lines_to_change(const std::string& input)
{
    std::vector<std::string> lines = split_js_function(input);

    for (const auto& line : lines)
    {
        std::cout << line << std::endl;
        check_pattern(line);
    }

    return lines;
}

void test_regex_006()
{
    std::cout << "\n*** test regex 006 ***" << std::endl;

    std::string the_input_string =
R"(
 var variable_01 = "var_01";\n var isEq = value3.value_1.equals("the_value_2");\n var variable_03 = variable_01 + variable_02;
)";
    std::vector<std::string> lines = select_lines_to_change(the_input_string);
    for (const auto& line : lines)
        std::cout << line << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void test_regex_007()
{
    std::cout << "\n*** test regex 007 ***" << std::endl;

    std::string the_input_string =
R"(
 var variable_01 = "var_01";\n var isEq = value_1.equals("the_value_2");\n var variable_03 = variable_01 + variable_02;
)";

    std::vector<std::string> lines = select_lines_to_change(the_input_string);
    for (const auto& line : lines)
        std::cout << line << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace {

    constexpr const char TOKEN_EQUALS[]{".equals"};
    constexpr const char TOKEN_EQUALS_REPLACE[]{ " === " };

    const std::string token_equals{ TOKEN_EQUALS };
    const std::string token_equals_replace{ TOKEN_EQUALS_REPLACE };

    std::string replate_text_portions(const std::string& original_text)
    {
        std::string result{ original_text };

        assert(token_equals.empty() == false);
        std::string::size_type pos{ result.find(token_equals) };
        assert(pos != std::string::npos);
        result.replace(pos, token_equals.length(), token_equals_replace);

        return result;
    }

}

std::string change_line_using_pattren(const std::string& script)
{
    std::string result{ script };
    const std::regex pattern(R"((\w+)\.equals\(\"(\w+)\"\))");

    // We will store the match results in 'matches'
    std::smatch matches;

    // Use regex_search to find the pattern in the entire script
    if (std::regex_search(script, matches, pattern)) {
        std::cout << "Found a match: " << matches[0] << "\n";
        std::cout << "First variable name:  " << matches[1] << "\n";
        std::cout << "Second variable name: " << matches[2] << "\n";

        const std::string::size_type start = 0;
        const std::string::size_type pos_match = script.find(matches[0], start);
        const std::string::size_type pos_first_point = script.find_first_of(".", start);

        assert(pos_match != std::string::npos);
        if (pos_first_point != std::string::npos
            && pos_first_point < pos_match)
        {
            std::cout << "Reject this match.\n";
        }
        else
        {
            std::cout << "Accept this match and replace.\n";
            result = replate_text_portions(result);
        }
    }
    else {
        std::cout << "No match found.\n";
    }

    return result;
}

std::vector<std::string> change_lines_by_pattern(const std::string& input)
{
    std::vector<std::string> lines = split_js_function(input);

    for (auto& line : lines)
    {
        const auto& chananged_line{ change_line_using_pattren(line) };
        if (line != chananged_line)
            line = chananged_line;

        std::cout << line << "\n";
    }

    return lines;
}

std::string build_line_from_lines(const std::vector<std::string>& lines)
{
    std::string line{};
    for (const auto& curr_line : lines)
    {
        line += curr_line + R"(\n)";
    }

    return line;
}

void test_regex_008()
{
    std::cout << "\n*** test regex 008 ***" << std::endl;

    const std::string the_input_string{
        R"( var variable_01 = "var_01";\n var isEq = value_1.equals("the_value_2");\n var variable_03 = variable_01 + variable_02;)"
    };

    // This line:
    // var isEq = value_1.equals("the_value_2");
    // must be transformed to:
    // var isEq = value_1 === ("the_value_2");
    // replace ".equals" with " === "

    std::vector<std::string> lines = change_lines_by_pattern(the_input_string);

    std::cout << "\n *** lines after replacement ***\n";
    for (const auto& line : lines)
        std::cout << line << "\n";

    const std::string& line_from_lines{ build_line_from_lines(lines) };
    std::cout << "\n *** result line ***\n";
    std::cout << line_from_lines << "\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_regex_009()
{
    std::cout << "\n*** test regex 009 ***" << std::endl;

    const std::string the_input_string{
        R"( var variable_01 = "var_01";\n var isEq = value3.value_1.equals("the_value_2");\n var variable_03 = variable_01 + variable_02;)"
    };

    // This line:
    // var isEq = value3.value_1.equals("the_value_2");
    // must not be transformed to:
    // var isEq = value3.value_1 === ("the_value_2");
    // do not replace ".equals" with " === "

    std::vector<std::string> lines = change_lines_by_pattern(the_input_string);

    std::cout << "\n *** lines after replacement ***\n";
    for (const auto& line : lines)
        std::cout << line << "\n";

    const std::string& line_from_lines{ build_line_from_lines (lines)};
    std::cout << "\n *** result line ***\n";
    std::cout << line_from_lines << "\n";
}
