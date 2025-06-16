#pragma once
#include <vector>
#include <string>

struct ScoreEntry {
    std::string name;
    int score;
};

std::vector<ScoreEntry> loadScores(const std::string& filename);
