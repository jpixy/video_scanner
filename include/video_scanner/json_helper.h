#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace video_scanner {

void SaveToJson(const std::vector<std::string> &files,
                const std::string &outputPath);

} // namespace video_scanner

#endif // JSON_HELPER_H