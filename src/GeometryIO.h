#pragma once

#include <fmt/format.h>

#include <fstream>
#include <iostream>

namespace Geometry::IO {

/// Writes triangles to an .stl file located at `path`.
template <typename Tris>
bool write_stl(
    const std::string& path,
    const Tris& tris
) {
    std::ofstream outfile;
    outfile.open(path);

    if (!outfile.is_open()) {
        return false;
    }

    outfile << "solid bubble\n";
    for (const auto& tri : tris) {
        const auto& normal = tri.normal();

        outfile << fmt::format("facet normal {} {} {}\n", normal.x, normal.y, normal.z);
        outfile << fmt::format("    outer loop\n");
        outfile << fmt::format("        vertex {} {} {}\n", tri.pts[0].x, tri.pts[0].y, tri.pts[0].z);
        outfile << fmt::format("        vertex {} {} {}\n", tri.pts[1].x, tri.pts[1].y, tri.pts[1].z);
        outfile << fmt::format("        vertex {} {} {}\n", tri.pts[2].x, tri.pts[2].y, tri.pts[2].z);
        outfile << fmt::format("    endloop\n");
        outfile << fmt::format("endfacet\n");
    }
    outfile << "endsolid bubble\n";

    outfile.close();

    return true;
}

} // namespace Geometry::IO
