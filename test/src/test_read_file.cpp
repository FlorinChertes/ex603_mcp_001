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
    ScopeTimer _t(__func__);

    std::ifstream in_file{ filePath, std::ios::in | std::ios::binary };
    if (!in_file)
        throw std::runtime_error("Cannot open " + filePath.filename().string());

    std::string file_as_string(static_cast<size_t>(std::filesystem::file_size(filePath)), 0);

    in_file.read(file_as_string.data(), file_as_string.size());
    if (!in_file)
        throw std::runtime_error("Could not read the full contents from " + filePath.filename().string());

    return file_as_string;
}

[[nodiscard]] std::size_t count_words_from_string(std::string&& file_as_string)
{
    ScopeTimer _t(__func__);

    std::stringstream istring_stream(std::move(file_as_string));
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
    std::string file_name("./data/big_file_of_strings.txt");
#else
    std::string file_name(".\\data\\big_file_of_strings.txt");
#endif

    std::cout << "input path: " << file_name << std::endl;

    try {
        const std::filesystem::path file_name_path(file_name.c_str());

        const std::size_t word_count {
            count_words_from_string(get_file_as_string(file_name_path)) };
        ScopeTimer::ShowStoredResults();

        std::cout << "word count: " << word_count << std::endl;
    }
    catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "filesystem error! " << err.what() << '\n';
    }
    catch (const std::runtime_error& err) {
        std::cerr << "runtime error! " << err.what() << '\n';
    }

    return 0;
}

[[nodiscard]] std::size_t count_words_from_file(const std::filesystem::path& filePath)
{
    ScopeTimer _t(__func__);

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
    std::string file_name("./data/big_file_of_strings.txt");
#else
    std::string file_name(".\\data\\big_file_of_strings.txt");
#endif

    std::cout << "input path: " << file_name << std::endl;

    try {
        const std::filesystem::path file_name_path(file_name.c_str());

        const std::size_t word_count = count_words_from_file(file_name_path);
        ScopeTimer::ShowStoredResults();

        std::cout << "word count: " << word_count << std::endl;
    }
    catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "filesystem error! " << err.what() << '\n';
    }
    catch (const std::runtime_error& err) {
        std::cerr << "runtime error! " << err.what() << '\n';
    }

    return 0;
}

void create_big_file(const std::filesystem::path& filePath)
{

#ifndef _MSC_VER
    std::string big_file_name("./data/big_file_of_strings.txt");
#else
    std::string big_file_name(".\\data\\big_file_of_strings.txt");
#endif

    std::ifstream in_file{ filePath, std::ios::in | std::ios::binary };
    if (!in_file)
        throw std::runtime_error("Cannot open " + filePath.filename().string());

    std::string file_as_string(static_cast<size_t>(std::filesystem::file_size(filePath)), 0);

    in_file.read(file_as_string.data(), file_as_string.size());
    if (!in_file)
        throw std::runtime_error("Could not read the full contents from " + filePath.filename().string());

    ScopeTimer _t(__func__);
    {
        const std::filesystem::path big_file_path(big_file_name.c_str());
        std::ofstream out_file{ big_file_path, std::ios::out | std::ios::binary };
        if (!out_file)
            throw std::runtime_error("Cannot open " + big_file_path.filename().string());

        for (int i = 0; i < 1024 * 5000; ++i)
        {
            out_file.write(file_as_string.data(), file_as_string.size());
            if (!out_file)
                throw std::runtime_error("Could not write the full contents from " + big_file_path.filename().string());
        }
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int test_create_big_file()
{
#ifndef _MSC_VER
    std::string file_name("./data/file_of_strings.txt");
#else
    std::string file_name(".\\data\\file_of_strings.txt");
#endif

    std::cout << "input path: " << file_name << std::endl;

    try {
        const std::filesystem::path file_name_path(file_name.c_str());

        create_big_file(file_name_path);
        ScopeTimer::ShowStoredResults();

        std::cout << "big file written" << std::endl;
    }
    catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "filesystem error! " << err.what() << '\n';
    }
    catch (const std::runtime_error& err) {
        std::cerr << "runtime error! " << err.what() << '\n';
    }

    return 0;
}


[[nodiscard]] std::size_t count_words_from_file_read_in_blocks(const std::filesystem::path& filePath)
{
    // Buffer size 1/32 Megabyte
    constexpr std::size_t buffer_size = 1 << 15; // 20 is 1 Megabyte
    std::cout << "buffersize: " << buffer_size << std::endl;

    std::unordered_set<std::string, StringHash, std::equal_to<>> uniques;
 
    try {

        std::ifstream in_file{ filePath, std::ios::in | std::ios::binary };
        if (!in_file)
            throw std::runtime_error("Cannot open " + filePath.filename().string());

        const auto fsize{ static_cast<size_t>(std::filesystem::file_size(filePath)) };
        const auto loops{ fsize / buffer_size };
        const auto lastChunk{ fsize % buffer_size };

        auto insert_file_block_in_set = [&in_file, &uniques]()
        {
            std::string file_as_string(buffer_size, 0);
            in_file.read(file_as_string.data(), file_as_string.size());

            std::stringstream istring_stream(std::move(file_as_string));
            std::istream_iterator<std::string> it{ istring_stream };

            std::transform(it, {}, std::inserter(uniques, uniques.begin()), std::identity{});
        };

        ScopeTimer _t(__func__);
        for (std::size_t i = 0; i < loops; ++i) 
        {
            insert_file_block_in_set();
        }
        
        if (lastChunk > 0)
        {
            insert_file_block_in_set();
        }
    }
    catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "filesystem error! " << err.what() << '\n';
    }
    catch (const std::runtime_error& err) {
        std::cerr << "runtime error! " << err.what() << '\n';
    }

    return  uniques.size();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int test_count_words_from_file_read_in_blocks()
{
#ifndef _MSC_VER
    std::string file_name("./data/big_file_of_strings.txt");
#else
    std::string file_name(".\\data\\big_file_of_strings.txt");
#endif

    std::cout << "input path: " << file_name << std::endl;

    try {
        const std::filesystem::path file_name_path(file_name.c_str());

        const std::size_t word_count =
            count_words_from_file_read_in_blocks(file_name_path);
        ScopeTimer::ShowStoredResults();

        std::cout << "word count: " << word_count << std::endl;
    }
    catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "filesystem error! " << err.what() << '\n';
    }
    catch (const std::runtime_error& err) {
        std::cerr << "runtime error! " << err.what() << '\n';
    }

    return 0;
}
