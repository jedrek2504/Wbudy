#ifndef	_SAMPLE_H_
#define	_SAMPLE_H_

//  OLED SCREEN
//   display_space is in pixels:
//    x: OLED_DISPLAY_WIDTH: 0 - 93
//    y: OLED_DISPLAY_HEIGHT: 0 - 63
//   colors: 2:
//	  OLED_COLOR_WHITE
//	  OLED_COLOR_BLACK
//   other:
//    (uint8_t*) - cstring pointer is 8bit
//	 font is:
//    x: 5px +1px spacing
//    y: 8px +1px spacing

// OLED screen also requires
//  previous call to
//  init_ssp(), init_adc(), oled_init()

// 8 nóg ? 8 funkcji?

const uint8_t SCREEN_SPACING = 1;
const uint8_t SCREEN_FONT_X = 5;
const uint8_t SCREEN_FONT_Y = 8;


static int sample() {
	return 5;
}

#endif
