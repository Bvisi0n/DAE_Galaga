#ifndef THRASHCACHE_H
#define THRASHCACHE_H

#include <algorithm>
#include <chrono>
#include <map>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace dae::ThrashCache
{
	struct Transform
	{
		float Matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	struct TestObject
	{
		Transform Local;
		int ID{ 1 };
	};

	struct TestObjectAlt
	{
		Transform* pLocal{ new Transform };
		int ID{ 1 };
	};

    template <typename T, typename WorkFunc>
    std::vector<float> MeasureCache(std::vector<T>& data, int sampleCount, WorkFunc doWork)
    {
        if (sampleCount < 1 || sampleCount > 1000)
        {
            throw std::invalid_argument("sampleCount < 1 || sampleCount > 1000");
        }

        constexpr short max_step{ 1024 };
        std::map<short, std::vector<long long>> results_map{};

        for (int sample = 0; sample < sampleCount + 2; ++sample)
        {
            for (short step = 1; step <= max_step; step *= 2)
            {
                const auto start = std::chrono::high_resolution_clock::now();

                for (size_t index = 0; index < data.size(); index += step)
                {
                    doWork(data[index]);
                }

                const auto end = std::chrono::high_resolution_clock::now();
                results_map[step].push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
            }
        }

        std::vector<float> averaged_results{};

        for (auto& [step, timings] : results_map)
        {
            std::sort(timings.begin(), timings.end());

            timings.erase(timings.begin());
            timings.pop_back();

            long long sum{ std::accumulate(timings.begin(), timings.end(), 0LL) };
            float average = static_cast<float>(sum / timings.size());

            averaged_results.push_back(average);
        }

        return averaged_results;
    }
}
#endif
