#include <array>
#include <vector>
#include <string>

#include <iomanip>
#include <sstream>
#include <iostream>

#include <atomic>

#include<shared_mutex>

#include <syncstream>
#include <thread>

#include <chrono>
#include <memory>

#include <cassert>


void test_061()
{
    std::cout << "*** test 061 ***" << std::endl;
    std::cout << '\n';
    uint32_t val_32{ 0x12345678 };
    
    std::cout << "val_32 (dec): " << val_32 << '\n';
    
    std::cout << "val_32 (hex): " << std::hex << val_32 << '\n';

    std::stringstream sstream;
    sstream << std::hex;
    sstream << "val_32 (hex via std::string): " << val_32;
    std::cout << sstream.str() << '\n';

    uint16_t val_16 = static_cast<uint16_t>(val_32);
    std::cout << "val_16: " << std::hex << val_16 << '\n';

    uint8_t val_8;

    std::cout << "val_32 & 0x000000FF: " << std::dec << (val_32 & 0x000000FF) << '\n';
    val_8 = val_32 & 0x000000FF;
    std::cout << "val_8: " /* << std::hex*/ << static_cast<uint16_t>(val_8) << '\n';

    std::cout << "(val_32 & 0x0000FF00) >> 8: " << ((val_32 & 0x0000FF00) >>  8) << '\n';
    val_8 = (val_32 & 0x0000FF00) >> 8;
    std::cout << "val_8: " << std::hex << static_cast<uint16_t>(val_8) << '\n';


    val_8 = static_cast<uint8_t>(val_32);
    std::cout << "val_8: " <<  static_cast<uint16_t>(val_8) << '\n';


    std::cout << '\n';
}

class TMP
{
public:
    TMP()
    {
        std::cout << "default ctor" << '\n';
    }
    ~TMP()
    {
        std::cout << "dctor" << '\n';
    }

    TMP([[maybe_unused]] const TMP& tmp)
    {
        std::cout << "copy ctor" << '\n';
    }

    TMP([[maybe_unused]] TMP&& tmp) noexcept
    {
        std::cout << "move copy ctor" << '\n';
    }

    TMP& operator= ([[maybe_unused]] const TMP& tmp)
    {
        std::cout << "assignment op" << '\n';
        return *this;
    }

    TMP& operator= ([[maybe_unused]] TMP&& tmp) noexcept
    {
        std::cout << "move assignment" << '\n';
        return *this;
    }

    std::string get_name() const
    {
        return "TMP";
    }
};

TMP func_returnTMP()
{
    TMP t;
    return t;
}

void consume_TMP(TMP&& t)
{ 
    const TMP tmp{std::move(t)};
    std::cout << tmp.get_name() << '\n';
}

std::string create_string()
{
    std::string s{"a_string"};
    std::cout << s << '\n';
    return s;
}

void consume_string(std::string& s)
{
    s = s.substr(0, 2);
    std::cout << s << '\n';
}

void test_062()
{
    std::cout << "*** test 062 ***" << std::endl;

    //TMP t1{};
    //TMP t2 = t1;
    
    //TMP t3{};
    //t3 = t1;

    /*const*/ auto&& t_ref = func_returnTMP();
    TMP t3{ std::move(t_ref) };

    //consume_TMP(func_returnTMP());

    //consume_string(create_string());
}
