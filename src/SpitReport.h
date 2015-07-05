#include <pebble.h>

Window 				*s_window;
GBitmap 			*s_res_up_icon;
GBitmap 			*s_res_next_hr_icon;
GBitmap 			*s_res_down_icon;
GBitmap 			*s_res_prev_hr_icon;
GBitmap 			*s_res_locate_icon;
GBitmap 			*s_res_refresh_icon;
GBitmap 			*s_res_prev_day_icon;
GBitmap 			*s_res_next_day_icon;
GBitmap 			*s_res_wave_icon;
GFont 				s_res_gothic_18;
GFont 				s_res_gothic_18_bold;
GFont 				s_res_gothic_24;
ActionBarLayer 		*spot_actionbar_layer;
InverterLayer		*inverter_bar_layer;
TextLayer 			*spot_name_layer;
TextLayer 			*surf_text_layer;
TextLayer 			*time_text_layer;
TextLayer 			*swel_text_layer;
TextLayer 			*wind_text_layer;
TextLayer 			*tide_text_layer;
AppTimer 			*toggle_timer;
bool 				toggle;
char				*snd_cmds[5];

void 				deinit();
void 				init(void);
void 				send_cmd(char *);
void 				destroy_ui(void);
void 				initialise_ui(void);
void 				handle_window_unload(Window*);
void 				toggle_timer_callback(void *);
void 				click_config_provider(Window *);
void 				tap_handler(AccelAxisType, int32_t);
void 				update_text_layers(DictionaryIterator *);
void 				show_locate_icon(ClickRecognizerRef, void *);
void 				in_dropped_handler(AppMessageResult, void *);
void 				out_sent_handler(DictionaryIterator *, void *);
void 				in_received_handler(DictionaryIterator *, void *);
void 				back_long_click_handler(ClickRecognizerRef, void *);
void 				up_single_click_handler(ClickRecognizerRef, void *);
void 				back_multi_click_handler(ClickRecognizerRef, void *);
void 				back_single_click_handler(ClickRecognizerRef, void *);
void 				select_long_click_handler(ClickRecognizerRef, void *);
void 				down_single_click_handler(ClickRecognizerRef, void *);
void 				select_multi_click_handler(ClickRecognizerRef, void *);
void 				select_single_click_handler(ClickRecognizerRef, void *);
void 				out_failed_handler(DictionaryIterator *, AppMessageResult, void *);
	
enum {
	SPOT_IDEN_KEY	= 0,		// TUPLE_CSTRING
	SPOT_NAME_KEY 	= 1,		// TUPLE_CSTRING
	SURF_HGHT_KEY 	= 2,		// TUPLE_CSTRING
	SURF_TEXT_KEY 	= 3,		// TUPLE_CSTRING
	TIME_TEXT_KEY 	= 4,		// TUPLE_CSTRING
	SWEL_TEXT_KEY 	= 5,		// TUPLE_CSTRING
	WIND_TEXT_KEY 	= 6,		// TUPLE_CSTRING
	TIDE_TEXT_KEY 	= 7,		// TUPLE_CSTRING
	WATR_TEXT_KEY 	= 8,		// TUPLE_CSTRING
	INDEX_KEY 		= 9,		// TUPLE_CSTRING
	REFRESH_KEY		= 10		// TUPLE_CSTRING
};
