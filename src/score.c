#include "score.h"

#include "hud.h"


// The score is 6 ASCII digits stored in the HUD text buffer, most significant
// digit first (score[0] = hundred-thousands ... score[5] = ones), so it can be
// drawn straight from the buffer and compared digit by digit.
u8* score = &hud[HUD_SCORE_COL];
u8 score_credits = 0;


void score_init(void)
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

    for (u8 i = HUD_DIGITS - 1; i > 0; i--)
    {
        if (score[i] > '9')
        {
            score[i] -= 10;
            score[i - 1]++;
        }
    }

    if (score[0] > '9')
    {
        for (u8 i = 0; i < HUD_DIGITS; i++) score[i] = '9';
        return;
    }

    // Points are always below 100, so at most one multiple of 100 is crossed
    // and the hundreds digit changes exactly when that happens.
    if (score[3] != hundreds_digit) score_credits++;
}


u8 score_check_hi(void)
{
    const u8* hi_score = &hud[HUD_HISCORE_COL];
    for (u8 i = 0; i < HUD_DIGITS; i++)
    {
        if (score[i] > hi_score[i]) return 1;
        else if (score[i] < hi_score[i]) return 0;
    }
    return 0;
}


void score_promote_hi(void)
{
    u8* hi_score = &hud[HUD_HISCORE_COL];
    for (u8 i = 0; i < HUD_DIGITS; i++) hi_score[i] = score[i];
    TILES_PUT_BUFFER(HUD_HISCORE_COL, HUD_ROW, hi_score, HUD_DIGITS);
}
