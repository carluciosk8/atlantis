#include "collision.h"

#include "gorgons.h"
#include "lane.h"
#include "score.h"
#include "tiles.h"
#include "torpedo.h"


// lo <= d <= hi for an 8-bit difference d, as a single unsigned compare
// (exact with 8-bit wraparound: values below lo wrap above hi - lo).
#define IN_RANGE(d, lo, hi)  ((u8)((u8)(d) - (u8)(lo)) <= (u8)((hi) - (lo)))

// Test one active torpedo against the lanes, from the bottom one up (3 to 0).
// The first hit wins.
static void collision_handle_torpedo(u8 torpedo_index)
{
    torpedo_t* torpedo = &torpedoes[torpedo_index];
    u8 tx = torpedo->sprite->X;
    u8 ty = torpedo->sprite->Y;
    lane_t* lane = &g_lanes[LANE_LAST];

    for (u8 lane_index = LANE_LAST; lane_index != 0xFF; lane_index--, lane--)
    {
        if (lane->state != LANE_STATE_MOVING) continue;

        u8 dx = tx - lane->sprite1->X;
        u8 dy = ty - lane->sprite1->Y;
        u8 id = lane->gorgon_id;

        if (GORGON_IS_BANDIT(id))
        {
            // A bandit hit blows up every gorgon on screen.
            if (IN_RANGE(dx, 2, 12) && IN_RANGE(dy, 6, 14))
            {
                torpedo_hide(torpedo_index);
                if (g_lanes[0].state == LANE_STATE_MOVING) lane_explode(0, 1);
                if (g_lanes[1].state == LANE_STATE_MOVING) lane_explode(1, 1);
                if (g_lanes[2].state == LANE_STATE_MOVING) lane_explode(2, 1);
                if (g_lanes[3].state == LANE_STATE_MOVING) lane_explode(3, 1);

                score_add((torpedo->dx == 0) ? 10 : 20);
                SCORE_DRAW();
                return;   // the torpedo is now hidden: no further hit possible
            }
        }
        else if (IN_RANGE(dx, -2, 15) && IN_RANGE(dy, 4, 15))
        {
            torpedo_hide(torpedo_index);
            lane_explode(lane_index, 0);

            score_add((torpedo->dx == 0) ? 1 : 2);
            SCORE_DRAW();
            return;       // the torpedo is now hidden: no further hit possible
        }
    }
}


// An inactive torpedo is parked at Y=192 and can never overlap a lane (lane
// Y <= 72), so skipping it gives the same result as testing it. The test is
// done here, with constant indices, so it costs a single absolute load.
void collision_handle_torpedoes(void)
{
    if (torpedoes[0].is_active) collision_handle_torpedo(0);
    if (torpedoes[1].is_active) collision_handle_torpedo(1);
}
