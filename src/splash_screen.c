#include "splash_screen.h"
#include <pebble.h>

#undef ACTION_BAR_WIDTH
#define ACTION_BAR_WIDTH   15

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_14;
static GBitmap *s_res_spitcast_logo;
static GBitmap *s_res_up_icon;
static GBitmap *s_res_next_hr_icon;
static GBitmap *s_res_down_icon;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;
static TextLayer *s_textlayer_4;
static TextLayer *s_textlayer_5;
static InverterLayer *s_inverterlayer_1;
static InverterLayer *s_inverterlayer_2;
static BitmapLayer *s_bitmaplayer_1;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, 0);
  
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_spitcast_logo = gbitmap_create_with_resource(RESOURCE_ID_SPITCAST_LOGO);
  s_res_up_icon = gbitmap_create_with_resource(RESOURCE_ID_UP_ICON);
  s_res_next_hr_icon = gbitmap_create_with_resource(RESOURCE_ID_NEXT_HR_ICON);
  s_res_down_icon = gbitmap_create_with_resource(RESOURCE_ID_DOWN_ICON);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(126, 5, 15, 17));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text_color(s_textlayer_1, GColorWhite);
  text_layer_set_text(s_textlayer_1, "Up");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(2, 6, 84, 42));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text_color(s_textlayer_2, GColorWhite);
  text_layer_set_text(s_textlayer_2, "-1 Hr    (Single) -1 Day (Double)      Exit (Hold)");
  text_layer_set_font(s_textlayer_2, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(126, 132, 15, 16));
  text_layer_set_background_color(s_textlayer_3, GColorClear);
  text_layer_set_text_color(s_textlayer_3, GColorWhite);
  text_layer_set_text(s_textlayer_3, "Dn");
  text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_3, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
  
  // s_textlayer_4
  s_textlayer_4 = text_layer_create(GRect(41, 54, 87, 42));
  text_layer_set_background_color(s_textlayer_4, GColorClear);
  text_layer_set_text_color(s_textlayer_4, GColorWhite);
  text_layer_set_text(s_textlayer_4, "+1 Hr    (Single)  +1 Day (Double) Locate (Hold)     ");
  text_layer_set_text_alignment(s_textlayer_4, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_4, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_4);
  
  // s_textlayer_5
  s_textlayer_5 = text_layer_create(GRect(57, 114, 47, 28));
  text_layer_set_background_color(s_textlayer_5, GColorClear);
  text_layer_set_text_color(s_textlayer_5, GColorWhite);
  text_layer_set_text(s_textlayer_5, "Shake to Continue");
  text_layer_set_text_alignment(s_textlayer_5, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_5, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_5);
  
  // s_inverterlayer_1
  s_inverterlayer_1 = inverter_layer_create(GRect(54, 112, 51, 32));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_1);
  
  // s_inverterlayer_2
  s_inverterlayer_2 = inverter_layer_create(GRect(55, 113, 49, 30));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_2);
  
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(12, 82, 29, 77));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_spitcast_logo);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_up_icon);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_next_hr_icon);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_down_icon);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
  text_layer_destroy(s_textlayer_4);
  text_layer_destroy(s_textlayer_5);
  inverter_layer_destroy(s_inverterlayer_1);
  inverter_layer_destroy(s_inverterlayer_2);
  bitmap_layer_destroy(s_bitmaplayer_1);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_spitcast_logo);
  gbitmap_destroy(s_res_up_icon);
  gbitmap_destroy(s_res_next_hr_icon);
  gbitmap_destroy(s_res_down_icon);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_splash_screen(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_splash_screen(void) {
  window_stack_remove(s_window, true);
}
