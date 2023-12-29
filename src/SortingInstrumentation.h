#pragma once

#include <vector>
#include <map>

namespace Sorting {

struct RecursiveDiffInstrumentation {
    struct RecursionSamples {
        std::size_t offset;
        std::vector<std::uint8_t> data;
    };

    std::map<std::size_t, std::vector<RecursionSamples>> samples;

    void operator()(
        std::vector<uint8_t>& arr,
        std::size_t lo,
        std::size_t hi,
        std::size_t depth
    ) {
        if (lo < hi) {
            RecursionSamples sample{};

            assert(lo < arr.size());
            assert(hi < arr.size());

            auto size = hi - lo;

            assert(size > 0);

            sample.offset = lo;
            sample.data = std::vector<uint8_t>(arr.begin() + lo, arr.begin() + lo + size + 1);

            samples[depth].push_back(sample);
        }
    }

    [[nodiscard]] std::vector<std::vector<std::uint8_t>> collapse_into_frames(
        const std::vector<std::uint8_t>& original_state
    ) const {
        std::vector<std::vector<std::uint8_t>> frames(samples.size());

        auto get_frame_state = [&] (std::size_t frame_i) {
            // Since samples are stored as a series of changes, we'll need to apply them to the previous state.
            auto state = frame_i == 0
                         ? original_state
                         : frames[frame_i - 1];

            for (const auto& edit : samples.at(frame_i)) {
                for (auto i = 0; i < edit.data.size(); ++i) {
                    state[edit.offset + i] = edit.data[i];
                }
            }

            return state;
        };

        for (std::size_t i = 0; i < samples.size(); ++i) {
            frames[i] = get_frame_state(i);
        }

        return frames;
    }
};


} // namespace Sorting
