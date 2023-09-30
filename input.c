#include "input.h"

static uint32_t pad_info_a;
static uint32_t pad_info_b; // keep track 'just_pressed' states

void input_init()
{
    PadInit(0);
    pad_info_a = 0;
    pad_info_b = 0;
}

void input_update()
{
    uint32_t pad_info = PadRead(0);
    pad_info_b = pad_info & (pad_info ^ pad_info_a);
    pad_info_a = pad_info;
}

bool input_is_action_just_pressed(uint8_t input_id, ACTION action)
{
    return pad_info_b & _PAD(input_id, action);
}

bool input_is_action_pressed(uint8_t input_id, ACTION action)
{
    return pad_info_a & _PAD(input_id, action);
}
