#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "num2words.h"

	#define MY_UUID { 0x97, 0xce, 0x8e, 0x68, 0x05, 0x18, 0x40, 0xfe, 0xaa, 0x6d, 0x49, 0x26, 0x31, 0x85, 0x8c, 0x16 }
	PBL_APP_INFO(MY_UUID,
             "Fuzzy Time HR Purgerski",
             "Goran Krajnovic",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define BUFFER_SIZE 86

static struct CommonWordsData {
  TextLayer label;
  Window window;
  char buffer[BUFFER_SIZE];
} s_data;

static void update_time(PblTm* t) {
  fuzzy_time_to_words(t->tm_hour, t->tm_min, s_data.buffer, BUFFER_SIZE);
  text_layer_set_text(&s_data.label, s_data.buffer);
}

static void handle_minute_tick(AppContextRef app_ctx, PebbleTickEvent* e) {
  update_time(e->tick_time);
}

static void handle_init(AppContextRef ctx) {

  
  resource_init_current_app(&APP_RESOURCES);

  window_init(&s_data.window, "Fuzzy Time Purgerski");
  const bool animated = true;
  window_stack_push(&s_data.window, animated);

  window_set_background_color(&s_data.window, GColorBlack);
  GFont font = fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD);

  text_layer_init(&s_data.label, GRect(0, 20, s_data.window.layer.frame.size.w, s_data.window.layer.frame.size.h - 20));
  text_layer_set_background_color(&s_data.label, GColorBlack);
  text_layer_set_text_color(&s_data.label, GColorWhite);
  text_layer_set_font(&s_data.label, font);
  layer_add_child(&s_data.window.layer, &s_data.label.layer);

  PblTm t;
  get_time(&t);
  update_time(&t);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
