#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

#include "appconfig.h"

namespace HighScore {
    inline unsigned score = AppConfig::Score::minimal_high_score;

    inline bool checkAndUpdate(unsigned new_score) {
        if (new_score > 0 && new_score > score) {
            score = new_score;
            return true;
        }
        return false;
    }
}

#endif // HIGH_SCORE_H