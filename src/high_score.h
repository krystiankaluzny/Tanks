#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

namespace HighScore {
    inline unsigned score = 0;

    inline bool checkAndUpdate(unsigned new_score) {
        if (new_score > 0 && new_score > score) {
            score = new_score;
            return true;
        }
        return false;
    }
}

#endif // HIGH_SCORE_H