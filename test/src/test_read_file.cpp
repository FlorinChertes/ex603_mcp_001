#include <string>
#include <unordered_set>

#include <functional>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <iterator>

#include "scope_timer.h"

struct StringHash {
    using is_transparent = void; // enables heterogenous lookup

    std::size_t operator()(std::string_view sv) const {
        std::hash<std::string_view> hasher;
        return hasher(sv);
    }
};

[[nodiscard]] std::string get_file_as_string(const std::filesystem::path& filePath)
{
    ScopeTimer _t(__func__, /*store*/true);

    std::ifstream in_file{ filePath, std::ios::in | std::ios::binary };
    if (!in_file)
        throw std::runtime_error("Cannot open " + filePath.filename().string());

    std::string file_as_string(static_cast<size_t>(std::filesystem::file_size(filePath)), 0);

    in_file.read(file_as_string.data(), file_as_string.size());
    if (!in_file)
        throw std::runtime_error("Could not read the full contents from " + filePath.filename().string());

    return file_as_string;
}

std::size_t count_words_from_string(const std::string& file_as_string)
{
    ScopeTimer _t(__func__, /*store*/true);

    std::stringstream istring_stream(file_as_string);
    std::istream_iterator<std::string> it{ istring_stream };
    std::unordered_set<std::string, StringHash, std::equal_to<>> uniques;

    std::transform(it, {}, std::inserter(uniques, uniques.begin()), std::identity{});

    return  uniques.size();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int test_count_words_from_file_as_string()
{

#ifndef _MSC_VER
    std::string file_name("./data/file_of_strings.txt");
#else
    std::string file_name(".\\data\\file_of_strings.txt");
#endif

    std::cout << "input path: " << file_name << std::endl;

    try {

        const std::filesystem::path file_name_path(file_name.c_str());

        const std::string& file_as_string = get_file_as_string(file_name_path);
        std::cout << file_as_string << std::endl;

        const std::size_t word_count = count_words_from_string(file_as_string);
        std::cout << "word count: " << word_count << std::endl;

        ScopeTimer::ShowStoredResults();

    }
    catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "filesystem error! " << err.what() << '\n';
    }
    catch (const std::runtime_error& err) {
        std::cerr << "runtime error! " << err.what() << '\n';
    }

    return 0;
}

std::size_t count_words_from_file(const std::filesystem::path& filePath)
{
    ScopeTimer _t(__func__, /*store*/true);

    std::ifstream ifile(filePath);
    std::istream_iterator<std::string> it{ ifile };
    std::unordered_set<std::string, StringHash, std::equal_to<>> uniques;

    std::transform(it, {}, std::inserter(uniques, uniques.begin()), std::identity{});

    return  uniques.size();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int test_count_words_from_file()
{

#ifndef _MSC_VER
    std::string file_name("./data/file_of_strings.txt");
#else
    std::string file_name(".\\data\\file_of_strings.txt");
#endif

    std::cout << "input path: " << file_name << std::endl;

    try {

        const std::filesystem::path file_name_path(file_name.c_str());

        const std::size_t word_count = count_words_from_file(file_name_path);
        std::cout << "word count: " << word_count << std::endl;

        ScopeTimer::ShowStoredResults();

    }
    catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "filesystem error! " << err.what() << '\n';
    }
    catch (const std::runtime_error& err) {
        std::cerr << "runtime error! " << err.what() << '\n';
    }

    return 0;
}

