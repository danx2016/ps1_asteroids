#include <libetc.h>

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

static bool check_button(uint32_t pad_info, uint8_t input_id, ACTION action)
{
    switch (action)
    {
        case UP:      return pad_info & _PAD(input_id, PADLup);
        case DOWN:    return pad_info & _PAD(input_id, PADLdown);
        case LEFT:    return pad_info & _PAD(input_id, PADLleft);
        case RIGHT:   return pad_info & _PAD(input_id, PADLright);
        case CONFIRM: return pad_info & _PAD(input_id, PADRright); // circle
        case CANCEL:  return pad_info & _PAD(input_id, PADRdown); // cross
        case START:   return pad_info & _PAD(input_id, PADstart);
        case SELECT:  return pad_info & _PAD(input_id, PADselect);
        case ANY:     return pad_info;
    }
    return false;    
}

bool input_is_action_just_pressed(uint8_t input_id, ACTION action)
{
    return check_button(pad_info_b, input_id, action);
}

bool input_is_action_pressed(uint8_t input_id, ACTION action)
{
    return check_button(pad_info_a, input_id, action);
}
