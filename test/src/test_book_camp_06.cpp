#include "../inc/test_book_camp_06.h"

#include <string>

#include <chrono>
#include <thread>
#include <random>

#include <type_traits>

#include <iomanip>
#include <sstream>
#include <iostream>

#include <cassert>

void check_properties_06()
{
    static_assert(std::is_destructible_v<Race::Blob>);
    static_assert(std::is_nothrow_destructible<Race::Blob>{});
    static_assert(!std::is_trivially_destructible<Race::Blob>{});

    static_assert(!std::is_default_constructible_v<Race::Blob>);
    static_assert(!std::is_nothrow_default_constructible<Race::Blob>{});
    static_assert(!std::is_trivially_default_constructible<Race::Blob>{});

    static_assert(!std::is_copy_constructible_v<Race::Blob>);
    static_assert(!std::is_copy_assignable_v<Race::Blob>);

    static_assert(!std::is_move_constructible_v<Race::Blob>);
    static_assert(!std::is_move_assignable_v<Race::Blob>);

    static_assert(std::has_virtual_destructor_v<Race::Blob>);

    Race::StepperBlob blob;
    blob.step();
    assert(blob.total_steps() == 2);

    std::random_device rd;
    Race::RandomBlob rnd_blob{
        std::default_random_engine{ rd() },
        std::uniform_int_distribution{ 0, 4 }
    };
    rnd_blob.step();
    assert(rnd_blob.total_steps() >= 0);

    Race::RandomBlob another_rnd_blob{
        std::default_random_engine{ rd() },
        std::poisson_distribution{ 2 }
    };
    another_rnd_blob.step();
    assert(another_rnd_blob.total_steps() >= 0);

    // Listing 6.12 Testing with random generators and distributions
    Race::RandomBlob random_blob(
          []() { return 0; }
        , [](auto gen) { return gen(); });
    random_blob.step();
    assert(random_blob.total_steps() == 0);

}

void Race::draw_blobs(const std::vector<Race::StepperBlob>& blobs)
{
    const int bag_height = 3;
    for (int y = 8; y >= 0; --y)
    {
        std::string output = y > 2 ? "  " : "| ";

        for (const auto& blob : blobs)
        {
            if (blob.total_steps() >= y)
                output += "* ";
            else
                output += "  ";
        }

        output += y >= bag_height ? ' ' : '|';
        std::cout << output << '\n';
    }

    std::cout << std::string(blobs.size() * 2 + 3, '-') << '\n';
}

// Listing 6.17 Draw each blob's current position
void Race::draw_blobs(const std::vector<std::unique_ptr<Race::Blob>>& blobs)
{
    const int bag_height = 3;
    for (int y = 12; y >= 0; --y)
    {
        std::string output = y > 2 ? "  " : "| ";

        for (const auto& blob : blobs)
        {
            if (blob->total_steps() >= y)
                output += "* ";
            else
                output += "  ";
        }

        output += y >= bag_height ? ' ' : '|';
        std::cout << output << '\n';
    }

    std::cout << std::string(blobs.size() * 2 + 3, '-') << '\n';
}


void Race::move_blobs(std::vector<Race::StepperBlob>& blobs)
{
    for (auto& blob : blobs)
    {
        blob.step();
    }
}

// Listing 6.16 Move all the blobs
void Race::move_blobs(const std::vector<std::unique_ptr<Race::Blob>>& blobs)
{
    for (const auto& blob : blobs)
    {
        blob->step();
    }
}

void Race::race(std::vector<Race::StepperBlob>& blobs)
{
    using namespace std::chrono;
    const int max = 3;

    std::cout << "+++++++++++";
        for (int i = 0; i < max; ++i)
        {
            draw_blobs(blobs);
            move_blobs(blobs);
            std::this_thread::sleep_for(1000ms);
            std::cout << "+++++++++++";
        }
    draw_blobs(blobs);
}

// Listing 6.15 A less predictable race
void Race::race(std::vector<std::unique_ptr<Blob>>& blobs)
{
    using namespace std::chrono;
    const int max = 3;

    std::cout << "\n+++++++++++\n";
    for (int i = 0; i < max; ++i)
    {
        draw_blobs(blobs);
        move_blobs(blobs);
        std::this_thread::sleep_for(1000ms);
        std::cout << "\n+++++++++++\n";
    }
    draw_blobs(blobs);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_boot_camp_polymorphism_069()
{
    std::cout << "\n*** test boot camp polymorphism 069 ***" << std::endl;

    check_properties_06();
    std::vector<Race::StepperBlob> blobs(4);
    Race::race(blobs);
}


// Listing 6.18 Create blobs for a proper race
std::vector<std::unique_ptr<Race::Blob>> create_blobs(int number)
{
    std::vector<std::unique_ptr<Race::Blob>> blobs;
    std::random_device rd;

    for (int i = 0; i < number / 2; ++i)
    {
        blobs.emplace_back(std::make_unique<Race::StepperBlob>());
        blobs.emplace_back(std::make_unique<Race::RandomBlob<
            std::default_random_engine,
            std::uniform_int_distribution<int>>>
            (
                std::default_random_engine{ rd() },
                std::uniform_int_distribution{ 0, 4 }
            )
        );
    }
    return blobs;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_boot_camp_polymorphism_069_01()
{
    std::cout << "\n*** test boot camp polymorphism 069 ***" << std::endl;

    check_properties_06();

    // Running both races together might be confusing, so
    // choose a type of race; either 6.9 (just steppers)
    //                        or 6.18 (various types)

     // Listing 6.19 A proper race
    auto blobs = create_blobs(8);
    Race::race(blobs);
}


