#ifndef SCOPE_TIMER_H
#define SCOPE_TIMER_H

#pragma once

#include <chrono>
#include <iostream>
#include <map>

// Scope Timer is not thread safe, so use it only on a single thread
class ScopeTimer {
public:
    explicit ScopeTimer(std::string name, bool store = false)  noexcept 
        : mName(std::move(name))
        , mStart(std::chrono::steady_clock::now())
        , mStore(store) 
    { }

    ~ScopeTimer() {
        const auto end = std::chrono::steady_clock::now();
        const auto res = std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart).count();
        std::cout << "duration in ms: " << res << " ms\n";
        if (mStore)
            sResults[mName] += res;
        else
            std::cout << mName << ": " << res << " ms\n";
    }
    ScopeTimer(const ScopeTimer&) = delete;
    ScopeTimer(ScopeTimer&&) = delete;
    ScopeTimer& operator=(const ScopeTimer&) = delete;
    ScopeTimer& operator=(ScopeTimer&&) = delete;

    static void ShowStoredResults() {
        for (const auto&[name, res] : sResults)
            std::cout << name << ": " << res << " ms\n";
    }

    static void ClearStoredResults() {
        sResults.clear();
    }


private:
    const std::string mName;
    const std::chrono::time_point<std::chrono::steady_clock> mStart;
    const bool mStore{ false };

    static inline std::map<std::string, long long> sResults;
};

#endif // SCOPE_TIMER_H