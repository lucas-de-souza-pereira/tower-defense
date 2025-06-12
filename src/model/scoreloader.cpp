#include "model/scoreloader.hpp"
#include "json.hpp" 
#include <fstream>

using json = nlohmann::json;

std::vector<ScoreEntry> loadScores(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<ScoreEntry> scores;

    if (file) {
        json data;
        file >> data;
        for (const auto& entry : data) {
            scores.push_back({entry["name"], entry["score"]});
        }
    }
    return scores;
}
