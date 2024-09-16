#include "score.h"

#include "hud.h"


uint8_t* score = &hud[9];
uint8_t score_credits = 0;


void score_init()
{
    score[0] = '0';
    score[1] = '0';
    score[2] = '0';
    score[3] = '0';
    score[4] = '0';
    score[5] = '0';
}


void score_increase_hundred(uint8_t digit)
{
    uint8_t ten_thousand_digit = score[1];

    score[3] += digit;
    if (score[3] > '9')
    {
        score[3] -= 10;
        score[2]++;
    }
    if (score[2] > '9')
    {
        score[2] -= 10;
        score[1]++;
    }
    if (score[1] > '9')
    {
        score[1] -= 10;
        score[0]++;
    }
    if (score[0] > '9')
    {
        score[0] -= 10;
        score[5]++;
    }
    if (score[5] > '9')
    {
        score[5] -= 10;
        score[4]++;
    }
    if (score[4] > '9')
    {
        score[4] -= 10;
        score[3]++;
    }

    if ( score[1] > ten_thousand_digit ) score_credits++;
}


void score_increase_thousand(uint8_t digit)
{
    uint8_t ten_thousand_digit = score[1];

    score[2] += digit;
    if (score[2] > '9')
    {
        score[2] -= 10;
        score[1]++;
    }
    if (score[1] > '9')
    {
        score[1] -= 10;
        score[0]++;
    }
    if (score[0] > '9')
    {
        score[0] -= 10;
        score[5]++;
    }
    if (score[5] > '9')
    {
        score[5] -= 10;
        score[4]++;
    }
    if (score[4] > '9')
    {
        score[4] -= 10;
        score[3]++;
    }
    if (score[3] > '9')
    {
        score[3] -= 10;
        score[2]++;
    }

    if ( score[1] > ten_thousand_digit ) score_credits++;
}




uint8_t score_check_hi()
{
    const uint8_t* hi_score = &hud[23];
    for (uint8_t i=0 ; i<6 ; i++)
    {
        if (score[i] > hi_score[i]) return 1;
        else if (score[i] < hi_score[i]) return 0;
    }
    return 0;
}


void score_promote_hi()
{
    uint8_t* hi_score = &hud[23];
    for (uint8_t i=0; i<6; i++) hi_score[i] = score[i];
    TILES_PUT_BUFFER(23, 23, hi_score, 6);
}
