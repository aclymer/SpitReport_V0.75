#include <pebble.h>
#include "splash_screen.h"

enum {
	SPOT_IDEN_KEY		= 0,		// TUPLE_CSTRING
	SPOT_NAME_KEY 	= 1,		// TUPLE_CSTRING
	SURF_HGHT_KEY 	= 2,		// TUPLE_CSTRING
	SURF_TEXT_KEY 	= 3,		// TUPLE_CSTRING
	TIME_TEXT_KEY 	= 4,		// TUPLE_CSTRING
	SWEL_TEXT_KEY 	= 5,		// TUPLE_CSTRING
	WIND_TEXT_KEY 	= 6,		// TUPLE_CSTRING
	TIDE_TEXT_KEY 	= 7,		// TUPLE_CSTRING
	WATR_TEXT_KEY 	= 8,		// TUPLE_CSTRING
	INDEX_KEY 			= 9,		// TUPLE_CSTRING
	REFRESH_KEY			= 10		// TUPLE_CSTRING
};

static Window *s_window;
static GBitmap *s_res_up_icon;
static GBitmap *s_res_next_hr_icon;
static GBitmap *s_res_down_icon;
static GBitmap *s_res_prev_hr_icon;
static GBitmap *s_res_locate_icon;
static GBitmap *s_res_refresh_icon;
static GBitmap *s_res_prev_day_icon;
static GBitmap *s_res_next_day_icon;
static GFont s_res_gothic_18;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_24;
static ActionBarLayer *spot_actionbar_layer;
static InverterLayer *inverter_bar_layer;
static TextLayer *spot_name_layer;
static TextLayer *surf_text_layer;
static TextLayer *time_text_layer;
static TextLayer *swel_text_layer;
static TextLayer *wind_text_layer;
static TextLayer *tide_text_layer;
static AppTimer *toggle_timer;
static bool toggle;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, false);
  
	// Initialize Icons
  s_res_up_icon = gbitmap_create_with_resource(RESOURCE_ID_UP_ICON);
  s_res_next_hr_icon = gbitmap_create_with_resource(RESOURCE_ID_NEXT_HR_ICON);
  s_res_down_icon = gbitmap_create_with_resource(RESOURCE_ID_DOWN_ICON);
  s_res_prev_hr_icon = gbitmap_create_with_resource(RESOURCE_ID_PREV_HR_ICON);
  s_res_locate_icon = gbitmap_create_with_resource(RESOURCE_ID_LOCATE_ICON);
  s_res_refresh_icon = gbitmap_create_with_resource(RESOURCE_ID_REFRESH_ICON);
  s_res_prev_day_icon = gbitmap_create_with_resource(RESOURCE_ID_PREV_DAY_ICON);
  s_res_next_day_icon = gbitmap_create_with_resource(RESOURCE_ID_NEXT_DAY_ICON);
	
	// Initialize Fonts
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_24 = fonts_get_system_font(FONT_KEY_GOTHIC_24);
	
  // spot_actionbar_layer
  spot_actionbar_layer = action_bar_layer_create();
  action_bar_layer_add_to_window(spot_actionbar_layer, s_window);
  action_bar_layer_set_background_color(spot_actionbar_layer, GColorBlack);
  action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_UP, s_res_up_icon);
  action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_SELECT, s_res_next_hr_icon);
  action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_DOWN, s_res_down_icon);
  layer_add_child(window_get_root_layer(s_window), (Layer *)spot_actionbar_layer);
  
  // inverter_bar_layer
  inverter_bar_layer = inverter_layer_create(GRect(9, 64, 115, 2));
  layer_add_child(window_get_root_layer(s_window), (Layer *)inverter_bar_layer);
  
  // spot_name_layer
  spot_name_layer = text_layer_create(GRect(9, 0, 115, 22));
  text_layer_set_background_color(spot_name_layer, GColorClear);
  text_layer_set_text_color(spot_name_layer, GColorWhite);
  text_layer_set_text(spot_name_layer, "by Aaron Clymer");
  text_layer_set_text_alignment(spot_name_layer, GTextAlignmentCenter);
  text_layer_set_font(spot_name_layer, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)spot_name_layer);
  
  // surf_text_layer
  surf_text_layer = text_layer_create(GRect(0, 20, 124, 20));
  text_layer_set_background_color(surf_text_layer, GColorClear);
  text_layer_set_text_color(surf_text_layer, GColorWhite);
  text_layer_set_text(surf_text_layer, "(C) 2014");
  text_layer_set_text_alignment(surf_text_layer, GTextAlignmentCenter);
  text_layer_set_font(surf_text_layer, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)surf_text_layer);
  
  // time_text_layer
  time_text_layer = text_layer_create(GRect(9, 38, 115, 20));
  text_layer_set_background_color(time_text_layer, GColorClear);
  text_layer_set_text_color(time_text_layer, GColorWhite);
  text_layer_set_text(time_text_layer, "Version 0.70");
  text_layer_set_text_alignment(time_text_layer, GTextAlignmentCenter);
  text_layer_set_font(time_text_layer, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_text_layer);
  
  // swel_text_layer
  swel_text_layer = text_layer_create(GRect(9, 72, 115, 24));
  text_layer_set_background_color(swel_text_layer, GColorClear);
  text_layer_set_text_color(swel_text_layer, GColorWhite);
  text_layer_set_text(swel_text_layer, "Swell: Fair");
  text_layer_set_text_alignment(swel_text_layer, GTextAlignmentCenter);
  text_layer_set_font(swel_text_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)swel_text_layer);
  
  // wind_text_layer
  wind_text_layer = text_layer_create(GRect(9, 96, 115, 24));
  text_layer_set_background_color(wind_text_layer, GColorClear);
  text_layer_set_text_color(wind_text_layer, GColorWhite);
  text_layer_set_text(wind_text_layer, "Wind: Fair");
  text_layer_set_text_alignment(wind_text_layer, GTextAlignmentCenter);
  text_layer_set_font(wind_text_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)wind_text_layer);
  
  // tide_text_layer
  tide_text_layer = text_layer_create(GRect(9, 120, 115, 24));
  text_layer_set_background_color(tide_text_layer, GColorClear);
  text_layer_set_text_color(tide_text_layer, GColorWhite);
  text_layer_set_text(tide_text_layer, "Tide: Fair");
  text_layer_set_text_alignment(tide_text_layer, GTextAlignmentCenter);
  text_layer_set_font(tide_text_layer, s_res_gothic_24);
  layer_add_child(window_get_root_layer(s_window), (Layer *)tide_text_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(spot_actionbar_layer);
  inverter_layer_destroy(inverter_bar_layer);
  text_layer_destroy(spot_name_layer);
  text_layer_destroy(surf_text_layer);
  text_layer_destroy(time_text_layer);
  text_layer_destroy(swel_text_layer);
  text_layer_destroy(wind_text_layer);
  text_layer_destroy(tide_text_layer);
  gbitmap_destroy(s_res_up_icon);
  gbitmap_destroy(s_res_next_hr_icon);
  gbitmap_destroy(s_res_prev_hr_icon);
  gbitmap_destroy(s_res_next_day_icon);
  gbitmap_destroy(s_res_prev_day_icon);
  gbitmap_destroy(s_res_locate_icon);
  gbitmap_destroy(s_res_down_icon);
}

void toggle_timer_callback(void *data) {
	toggle_shake(toggle);
	toggle_timer = app_timer_register(60 + (160 * (int) toggle), toggle_timer_callback, NULL);
	toggle = !toggle;
}

void deinit() {
	accel_tap_service_unsubscribe();
	app_timer_cancel(toggle_timer);
  window_stack_remove(s_window, true);
}

void handle_window_unload(Window* window) {
  destroy_ui();
}

void update_text_layers(DictionaryIterator *iterator)
{
	Tuple *text_tuple = dict_read_first(iterator);
	
	while (text_tuple)
	{
		switch (text_tuple->key)
		{
			case SPOT_NAME_KEY: text_layer_set_text(spot_name_layer, text_tuple->value->cstring);	break;
			case SURF_TEXT_KEY: text_layer_set_text(surf_text_layer, text_tuple->value->cstring);	break;
			case TIME_TEXT_KEY: text_layer_set_text(time_text_layer, text_tuple->value->cstring);	break;
			case SWEL_TEXT_KEY: text_layer_set_text(swel_text_layer, text_tuple->value->cstring);	break;
			case WIND_TEXT_KEY: text_layer_set_text(wind_text_layer, text_tuple->value->cstring);	break;
			case TIDE_TEXT_KEY: text_layer_set_text(tide_text_layer, text_tuple->value->cstring);	break;
		}
		text_tuple = dict_read_next(iterator);
	}
}

void out_sent_handler(DictionaryIterator *sent, void *context) {
   // outgoing message was delivered
 }


 void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   // outgoing message failed
 }


 void in_received_handler(DictionaryIterator *received, void *context) {
   // incoming message received
	action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_SELECT, s_res_next_hr_icon);
	update_text_layers(received);
 }

 void in_dropped_handler(AppMessageResult reason, void *context) {
   // incoming message dropped
	 
 }

static void send_cmd(char *index) {
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if (iter == NULL) {
		return;
	}
	Tuplet direction = TupletCString(9, index);	
	dict_write_tuplet(iter, &direction);
	dict_write_end(iter);
	
	app_message_outbox_send();
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	send_cmd("above");
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	send_cmd("below");
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	send_cmd("next_hr");
}

void select_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
	action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_SELECT, s_res_next_day_icon);
	vibes_double_pulse();
	send_cmd("next_day");
}

void show_locate_icon(ClickRecognizerRef recognizer, void *context) {
	action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_SELECT, s_res_locate_icon);
	vibes_short_pulse();
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
	action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_SELECT, s_res_locate_icon);
	send_cmd("locate");
}

void back_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_SELECT, s_res_prev_hr_icon);
	send_cmd("prev_hr");
}

void back_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
	action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_SELECT, s_res_prev_day_icon);
	vibes_double_pulse();
	send_cmd("prev_day");
}

void back_long_click_handler(ClickRecognizerRef recognizer, void *context) {
	window_stack_pop(true);
	deinit();
}

void click_config_provider(Window *window) {
	
	//	Single Click Up Action
	window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
	
	//	Single Click Down
	window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
	
	//	Single Click Select
	window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
	
	//	Double Click Select
	window_multi_click_subscribe(BUTTON_ID_SELECT, 2, 2, 200 , true, select_multi_click_handler);
	
	// Long Click Select
	window_long_click_subscribe(BUTTON_ID_SELECT, 400, show_locate_icon, select_long_click_handler);
	
	//	Single Click Back
	window_single_click_subscribe(BUTTON_ID_BACK, back_single_click_handler);
	
	//	Double Click Back
	window_multi_click_subscribe(BUTTON_ID_BACK, 2, 2, 200 , true, back_multi_click_handler);
	
	//	Long Click Back
	window_long_click_subscribe(BUTTON_ID_BACK, 500, NULL, back_long_click_handler);
}

void tap_handler(AccelAxisType axis, int32_t dir) {
	hide_splash_screen();
	action_bar_layer_set_icon(spot_actionbar_layer, BUTTON_ID_SELECT, s_res_refresh_icon);
	send_cmd("refresh");
}

static void init(void) {

  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
		
	// Register callback functions
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);
	
	const int inbound_size = 1024;
	const int outbound_size = 256;
	app_message_open(inbound_size, outbound_size);
	
	// Subscribe to tap events
	accel_tap_service_subscribe(tap_handler);
	
	//Click Config Provider Init
	window_set_click_config_provider(s_window, (ClickConfigProvider) click_config_provider);
	
  window_stack_push(s_window, true);	
	show_splash_screen();
	app_timer_register(80, toggle_timer_callback, NULL);
	send_cmd("locate");
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}