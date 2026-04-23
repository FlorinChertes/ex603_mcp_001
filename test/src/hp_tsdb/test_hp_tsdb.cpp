#include <cmath>
#include <ranges>
#include <chrono>

#include <deque>
#include <stack>

#include <list>
#include <vector>
#include <queue>

#include <functional>
#include <algorithm>
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct DataPoint 
{
    int64_t     timestamp;
    double      value;
    std::string tag;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::vector<DataPoint> storage_;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void insert(
      int64_t timestamp
    , double value
    , std::string tag)
{
    if (tag.empty())
        throw std::invalid_argument("Tag must not be empty");

    if (!std::isfinite(value))
        throw std::invalid_argument("Value must be finite");

    storage_.push_back(DataPoint{
        .timestamp  = timestamp,
        .value      = value,
        .tag        = std::move(tag)
        });
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
[[nodiscard]] auto query_range(
      int64_t start_ts
    , int64_t end_ts)
    -> std::vector<DataPoint>
{
    if (start_ts > end_ts)
        throw std::invalid_argument("start_ts must be <= end_ts");

    std::vector<DataPoint> result;
    
    for (const auto& point : storage_) {
        if (point.timestamp >= start_ts && point.timestamp <= end_ts)
            result.push_back(point);  // Deep copy!
    }

    std::sort(result.begin(), result.end(),
        [](const DataPoint& a, const DataPoint& b) {
            return a.timestamp < b.timestamp;
        });

    return result;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct AggregatedResult
{
    std::string tag;
    std::size_t count = 0;
    double      min = std::numeric_limits<double>::max();
    double      max = std::numeric_limits<double>::lowest();
    double      sum = 0.0;

    [[nodiscard]] auto avg() const -> double
    {
        if (count == 0) return 0.0;
        return sum / static_cast<double>(count);
    }
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//std::unordered_map<std::string, AggregatedResult> accumulators_;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
[[nodiscard]] auto query_aggregate(
      int64_t start_ts
    , int64_t end_ts ) -> std::unordered_map<std::string, AggregatedResult>
{
    if (start_ts > end_ts)
        throw std::invalid_argument("start_ts must be <= end_ts");

    auto in_range = [=](const DataPoint& point) {
        return point.timestamp >= start_ts && point.timestamp <= end_ts;
        };
    
    std::unordered_map<std::string, AggregatedResult> accumulators;
    auto accumulate_result = [&accumulators](const DataPoint& point) {
        auto& agg = accumulators[point.tag];
        if (agg.count == 0) agg.tag = point.tag;
        agg.count++;
        agg.sum += point.value;
        if (point.value < agg.min) agg.min = point.value;
        if (point.value > agg.max) agg.max = point.value;
    };

    std::ranges::for_each (
          storage_ | std::views::filter(in_range)
        , accumulate_result );

    return accumulators;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
double bench_vector_lower_bound(
      const std::vector<int>& initial
    , const std::vector<int>& inserts)
{
    std::vector<int> data;
    data.reserve(initial.size() + inserts.size());
    data = initial;

    const auto start = std::chrono::high_resolution_clock::now();

    for (int val : inserts)
        data.insert(std::ranges::lower_bound(data, val), val);

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    return static_cast<double>(duration) / static_cast<double>(inserts.size());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_manning_HP_cpp_001()
{
    std::cout << "\n*** test manning HP C++ 001s ***" << std::endl;

    std::cout << "start\n";
    const std::string file_path{ "data/inputfile_manning_hp_cpp_001.txt" };
 
    std::ifstream file{ file_path };
    std::string line{};
    int64_t current_time{};

    while(std::getline(file, line))
    {
        const size_t delim = line.find(';');
        std::string station = line.substr(0, delim);

        const std::string& val_str = line.substr(delim + 1);
        // locale-aware + throws on bad input
        const double temp = std::stod(val_str);

        insert(current_time++, temp, std::move(station));
    }

    std::unordered_map<std::string, AggregatedResult> accumulators = query_aggregate(0,100);
    for (const auto& [tag, agg] : accumulators)
    {
        std::cout << "Tag: " << tag <<
            " count: " << agg.count <<
            " min: " << agg.min <<
            " max: " << agg.max <<
            " avg: " << agg.avg() <<
            std::endl;
	}

    std::cout << "\nend\n";
}

