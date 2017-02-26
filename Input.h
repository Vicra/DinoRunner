unsigned char cval[4],lval[4];

#define BUTTON_1 FPGA_BTN_0
#define BUTTON_2 FPGA_BTN_1
#define BUTTON_3 FPGA_BTN_2
#define BUTTON_4 FPGA_BTN_3

int time_button_released = 0;
int time_button_pressed = 0;
int time_hold = 0;
int time_released = 0;

enum event_t {
	event_none,
	event_1,
	event_2,
	event_3,
	event_4
};

enum event_t hasEvent()
{
	enum event_t ret = event_none;

	cval[0] = digitalRead( BUTTON_1 );
	cval[1] = digitalRead( BUTTON_2 );
	cval[2] = digitalRead( BUTTON_3 );
	cval[3] = digitalRead( BUTTON_4 );

	if (lval[0] != cval[0] ) {
          if(cval[0] == HIGH)
          {
            time_button_pressed = millis();
            time_released = time_button_pressed - time_button_released;
            ret = event_1;
          }
          else
          {
            time_button_released = millis();
            time_hold = time_button_released - time_button_pressed;
            ret = event_none;
          }
	} else if (/*!lval[1] && */cval[1]) {
		ret = event_2;
	} else if (/*!lval[2] && */cval[2]) {
		ret = event_3;
	} else if (/*!lval[3] && */cval[3]) {
		ret=  event_4;
	}
	*((unsigned*)lval)=*((unsigned*)cval);

	return ret;
}
