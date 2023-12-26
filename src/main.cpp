#include <vector>
#include <random>
#include <array>
#include <map>

#include "Geometry.h"
#include "GeometryIO.h"
#include "Sorting.h"

using namespace Geometry;

struct PrismParams {
    double x_mm = 1.0;
    double y_mm = 1.0;
    double z_mm_per_count = 1.0;
};

/// Construct prisms arranged in a line starting from 'origin', with heights based on the values of elements in 'items'.
[[nodiscard]]
std::vector<RectangularPrism> make_prisms(
    const std::vector<uint8_t>& items,
    PrismParams params,
    Pt origin
) {
    std::vector<RectangularPrism> out;

    for (auto i = 0; i < items.size(); ++i) {
        const auto xmin = (i+0) * params.x_mm;
        const auto xmax = (i+1) * params.x_mm;
        const auto ymin = 0;
        const auto ymax = params.y_mm;

        const auto lower_face = Rectangle {
            {
                Pt{xmin, ymin, 0 },
                Pt{xmax, ymin, 0 },
                Pt{xmax, ymax, 0 },
                Pt{xmin, ymax, 0 }
            }
        }.translated(origin.toVec());

        out.push_back(
            RectangularPrism {
                lower_face.flipped(),
                lower_face.translated(Vec{0,0,params.z_mm_per_count * items[i]})
            }
        );
    }

    return out;
}

/// Construct prisms arranged in a line starting from 'origin', with heights based on the values of elements in 'items'.
std::vector<Tri> make_triangles(
    const std::vector<std::vector<std::uint8_t>>& epochs,
    PrismParams params
) {
    assert(!epochs.empty());

    std::vector<Tri> triangles;
    triangles.reserve(epochs.size() * epochs[0].size());

    for (auto i = 0; i < epochs.size(); ++i) {
        const auto origin = Pt {
            0,
            i * params.y_mm,
            0
        };

        auto prisms = make_prisms(epochs[i], params, origin);
        for (const auto& prism : prisms) {
            auto tris = prism.triangles();
            triangles.insert(triangles.end(), tris.begin(), tris.end());
        }
    }

    return triangles;
}

void visualize_bubblesort(
    const std::string& destination,
    const std::vector<uint8_t>& original_state
) {
    auto items = original_state;

    std::vector<std::vector<std::uint8_t>> epochs;

    Sorting::bubblesort(
        items,
        [&](const auto& is) {
            epochs.push_back(is);
        }
    );

    fmt::print("bubble: {}\n", epochs.size());

    Geometry::IO::write_stl(destination, make_triangles(epochs, PrismParams{}));
}

void visualize_quicksort(
    const std::string& destination,
    const std::vector<uint8_t>& original_state
) {
    auto items = original_state;

    struct RecursionSamples {
        std::size_t offset;
        std::vector<std::uint8_t> data;
    };

    std::map<std::size_t, std::vector<RecursionSamples>> samples;

    auto qsort_instrumenter = [&](
        std::vector<uint8_t>& arr,
        std::size_t lo,
        std::size_t hi,
        std::size_t depth
    ) {
        RecursionSamples sample{};

        if (lo < hi) {
            assert(lo < arr.size());
            assert(hi < arr.size());

            auto size = hi - lo;

            assert(size > 0);

            sample.offset = lo;
            sample.data = std::vector<uint8_t>(arr.begin() + lo, arr.begin() + lo + size + 1);

            samples[depth].push_back(sample);
        }
    };

    Sorting::quicksort(
        items,
        qsort_instrumenter
    );

    // Now let's get the value from the frames!
    std::vector<std::vector<std::uint8_t>> frames(samples.size());

    auto get_frame_state = [&] (std::size_t frame_i) {
        // Since samples are stored as a series of changes, we'll need to apply them to the previous state.
        auto state = frame_i == 0
                ? original_state
                : frames[frame_i - 1];

        for (const auto& edit : samples[frame_i]) {
            for (auto i = 0; i < edit.data.size(); ++i) {
                state[edit.offset + i] = edit.data[i];
            }
        }

        return state;
    };

    for (std::size_t i = 0; i < samples.size(); ++i) {
        frames[i] = get_frame_state(i);
    }

    fmt::print("quicksort: {}\n", samples.size());

    Geometry::IO::write_stl(destination, make_triangles(frames, PrismParams{}));
}

int main() {
    const auto num_items = 50;

    const auto collection = std::invoke(
        [&] {
            // TODO: see rng if we want...
            std::mt19937 rng;

            std::uniform_int_distribution<std::uint8_t> dist{
                std::numeric_limits<std::uint8_t>::min(),
                std::numeric_limits<std::uint8_t>::max()
            };

            std::vector<std::uint8_t> items = {};
            items.reserve(num_items);
            for (auto i = 0; i < num_items; ++i) {
                items.push_back(dist(rng));
            }

            return items;
        }
    );

    visualize_bubblesort("/Users/matthew.nielsen/Documents/bubble.stl", collection);
    visualize_quicksort("/Users/matthew.nielsen/Documents/quicksort.stl", collection);

    return 0;
}
