#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdint.h>
#include <stdbool.h>
#include <libetc.h>

typedef enum 
{ 
    UP      = PADLup, 
    DOWN    = PADLdown, 
    LEFT    = PADLleft, 
    RIGHT   = PADLright, 
    CONFIRM = PADRright, 
    CANCEL  = PADRdown, 
    START   = PADstart, 
    SELECT  = PADselect
} ACTION;

extern void input_init();
extern void input_update();

// input_id -> 0=left controller or 1=right controller
extern bool input_is_action_just_pressed(uint8_t input_id, ACTION action);
extern bool input_is_action_pressed(uint8_t input_id, ACTION action);

#endif /* _INPUT_H_ */