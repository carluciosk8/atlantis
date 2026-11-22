#include "score.h"

#include "hud.h"


// The score is 6 ASCII digits stored in the HUD text buffer, most significant
// digit first (score[0] = hundred-thousands ... score[5] = ones).
u8* score = &hud[9];
u8 score_credits = 0;


void score_init()
{
    score[0] = '0';
    score[1] = '0';
    score[2] = '0';
    score[3] = '0';
    score[4] = '0';
    score[5] = '0';
}


// Add 'points' (at most 99) to the score. Saturates at 999999. Every time the
// score crosses a multiple of 100 a credit (bonus installation) is earned.
void score_add(u8 points)
{
    u8 hundreds_digit = score[3];

    u8 tens = 0;
    while (points >= 10)
    {
        points -= 10;
        tens++;
    }

    score[5] += points;
    score[4] += tens;

    for (u8 i = 5; i > 0; i--)
    {
        if (score[i] > '9')
        {
            score[i] -= 10;
            score[i - 1]++;
        }
    }

    if (score[0] > '9')
    {
        for (u8 i = 0; i < 6; i++) score[i] = '9';
        return;
    }

    // Points are always below 100, so at most one multiple of 100 is crossed
    // and the hundreds digit changes exactly when that happens.
    if (score[3] != hundreds_digit) score_credits++;
}


u8 score_check_hi()
{
    const u8* hi_score = &hud[23];
    for (u8 i = 0; i < 6; i++)
    {
        if (score[i] > hi_score[i]) return 1;
        else if (score[i] < hi_score[i]) return 0;
    }
    return 0;
}


void score_promote_hi()
{
    u8* hi_score = &hud[23];
    for (u8 i = 0; i < 6; i++) hi_score[i] = score[i];
    TILES_PUT_BUFFER(23, 23, hi_score, 6);
}
