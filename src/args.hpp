#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

namespace hexview {
class ArgsParser {
  public:
    ArgsParser(const int argc, const char *const *argv) {
        for (int i = 1; i < argc; i++) {
            m_tokens.push_back(std::string{argv[i]});
        }
    }

    std::optional<std::string> get_option(const std::string &option) {
        auto it = std::find(m_tokens.begin(), m_tokens.end(), option);
        if (it != m_tokens.end() && (++it != m_tokens.end())) {
            return *it;
        }

        return {};
    }
    bool option_exists(const std::string &option) {
        return std::find(m_tokens.begin(), m_tokens.end(), option) !=
               m_tokens.end();
    }

  private:
    std::vector<std::string> m_tokens;
};
} // namespace hexview
