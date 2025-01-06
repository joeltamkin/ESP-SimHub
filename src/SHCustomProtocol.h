#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#include <Arduino.h>
/**
 * Bodmer TFT
 */
#include <TFT_eSPI.h>
#include <tft_setup.h>
TFT_eSPI tft = TFT_eSPI();

/**
 * lvgl
 */
#define DRAW_BUF_SIZE (TFT_WIDTH * TFT_HEIGHT / 10 * (LV_COLOR_DEPTH/8))
#include <lvgl.h>

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

LV_IMAGE_DECLARE(aclogo_240px);
LV_IMAGE_DECLARE(lowfuelicon);

class SHCustomProtocol {
private:

public:
	/* Create a display using the inbuilt LVGL support for TFT_eSPI*/
	lv_display_t *disp;
	lv_obj_t * gearcircle;
	lv_obj_t * gearlabel;
	lv_obj_t * frtire;
	lv_obj_t * fltire;
	lv_obj_t * rltire;
	lv_obj_t * rrtire;

	lv_obj_t * fuelgauge;
	lv_obj_t * lowfuelindicator;

	lv_obj_t * gamelogo;

	/*
	lv_obj_t * frtirelife;
	lv_obj_t * fltirelife;
	lv_obj_t * rltirelife;
	lv_obj_t * rrtirelife;
	*/

	lv_style_t frtiretemp;
	lv_style_t fltiretemp;
	lv_style_t rltiretemp;
	lv_style_t rrtiretemp;

	lv_style_t fuelgaugestyle;
	lv_style_t fuelgaugeindicatorstyle;

	//lv_style_t tiretempblue;
	lv_style_t gearcirclestyle;
	lv_style_t gearlabelstyle;
	
	/*
	CUSTOM PROTOCOL CLASS
	SEE https://github.com/zegreatclan/SimHub/wiki/Custom-Arduino-hardware-support

	GENERAL RULES :
		- ALWAYS BACKUP THIS FILE, reinstalling/updating SimHub would overwrite it with the default version.
		- Read data AS FAST AS POSSIBLE in the read function
		- NEVER block the arduino (using delay for instance)
		- Make sure the data read in "read()" function READS ALL THE DATA from the serial port matching the custom protocol definition
		- Idle function is called hundreds of times per second, never use it for slow code, arduino performances would fall
		- If you use library suspending interrupts make sure to use it only in the "read" function when ALL data has been read from the serial port.
			It is the only interrupt safe place

	COMMON FUNCTIONS :
		- FlowSerialReadStringUntil('\n')
			Read the incoming data up to the end (\n) won't be included
		- FlowSerialReadStringUntil(';')
			Read the incoming data up to the separator (;) separator won't be included
		- FlowSerialDebugPrintLn(string)
			Send a debug message to simhub which will display in the log panel and log file (only use it when debugging, it would slow down arduino in run conditions)

	*/

	// Called when starting the arduino (setup method in main sketch)
	void setup() {
		// Bodmer test
		//tft.init();
		//tft.begin();
		//tft.fillScreen(TFT_BLUE);
		// end Bodmer test
		lv_init();
		disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));
		gearlabel = lv_label_create( lv_screen_active() );
		gamelogo = lv_image_create(lv_screen_active());
		gearcircle = lv_label_create( lv_screen_active() );
		frtire = lv_arc_create(lv_screen_active());
		fltire = lv_arc_create(lv_screen_active());
		rltire = lv_arc_create(lv_screen_active());
		rrtire = lv_arc_create(lv_screen_active());

		fuelgauge = lv_arc_create(lv_screen_active());
		lowfuelindicator = lv_image_create(lv_screen_active());
		
		
		
		lv_image_set_src(gamelogo, &aclogo_240px);
		lv_image_set_src(lowfuelindicator, &lowfuelicon);
		
		/*frtirelife = lv_arc_create(lv_screen_active());
		fltirelife = lv_arc_create(lv_screen_active());
		rltirelife = lv_arc_create(lv_screen_active());
		rrtirelife = lv_arc_create(lv_screen_active());
		*/


  		/* Set a tick source so that LVGL will know how much time elapsed. */
  		lv_tick_set_cb((lv_tick_get_cb_t)millis);

		
    	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
    	//lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

		/** 
		static lv_style_t ledstyle;
		lv_style_set_width(&ledstyle, 150);
		lv_style_set_height(&ledstyle, 150);
		lv_style_set_border_width(&ledstyle, 4);
		lv_style_set_border_color(&ledstyle, lv_color_white());

 		lv_obj_t * gearled  = lv_led_create(lv_screen_active());
		lv_obj_add_style( gearled, &ledstyle,0);
   		lv_obj_align(gearled, LV_ALIGN_CENTER, 0, 0);
    	//lv_led_set_brightness(gearled, 255);
		lv_led_off(gearled);
    	lv_led_set_color(gearled, lv_color_black());
		*/

	
		
    	lv_obj_set_size(frtire, 220, 220);
    	lv_arc_set_rotation(frtire, 280);
    	lv_arc_set_bg_angles(frtire, 0, 70);
		
    	lv_obj_set_size(rrtire, 220, 220);
    	lv_arc_set_rotation(rrtire, 10);
    	lv_arc_set_bg_angles(rrtire, 0, 70);
		
    	lv_obj_set_size(rltire, 220, 220);
    	lv_arc_set_rotation(rltire, 100);
    	lv_arc_set_bg_angles(rltire, 0, 70);
		
    	lv_obj_set_size(fltire, 220, 220);
    	lv_arc_set_rotation(fltire, 190);
    	lv_arc_set_bg_angles(fltire, 0, 70);

		lv_obj_set_size(fuelgauge, 155, 155);
		lv_arc_set_rotation(fuelgauge, 135);
		lv_arc_set_bg_angles(fuelgauge, 0, 90);
		/*
		lv_obj_set_size(frtirelife, 220, 220);
    	lv_arc_set_rotation(frtirelife, 290);
    	lv_arc_set_bg_angles(frtirelife, 0, 50);
		
    	lv_obj_set_size(fltirelife, 220, 220);
    	lv_arc_set_rotation(fltirelife, 20);
    	lv_arc_set_bg_angles(fltirelife, 0, 50);
		
    	lv_obj_set_size(rltirelife, 220, 220);
    	lv_arc_set_rotation(rltirelife, 110);
    	lv_arc_set_bg_angles(rltirelife, 0, 50);
		
    	lv_obj_set_size(rrtirelife, 220, 220);
    	lv_arc_set_rotation(rrtirelife, 200);
    	lv_arc_set_bg_angles(rrtirelife, 0, 50);
		*/

		lv_style_set_arc_color(&frtiretemp, lv_color_hex(0x111111));
		//lv_style_set_arc_rounded(&frtiretemp, false);
		lv_style_set_arc_color(&fltiretemp, lv_color_hex(0x111111));
		//lv_style_set_arc_rounded(&fltiretemp, false);
		lv_style_set_arc_color(&rltiretemp, lv_color_hex(0x111111));
		//lv_style_set_arc_rounded(&rltiretemp, false);
		lv_style_set_arc_color(&rrtiretemp, lv_color_hex(0x111111));
		//lv_style_set_arc_rounded(&rrtiretemp, false);

		lv_style_set_arc_color(&fuelgaugestyle, lv_color_hex(0xAAAAAA));
		lv_style_set_arc_color(&fuelgaugeindicatorstyle, lv_color_hex(0xFFFF00));
		lv_arc_set_value(fuelgauge, 50);
	
		//lv_style_set_arc_rounded(&fuelgaugestyle, false);


		lv_obj_add_style(frtire, &frtiretemp,LV_PART_MAIN);
		lv_obj_add_style(fltire, &fltiretemp,LV_PART_MAIN);
		lv_obj_add_style(rltire, &rltiretemp,LV_PART_MAIN);
		lv_obj_add_style(rrtire, &rrtiretemp,LV_PART_MAIN);

		lv_obj_add_style(fuelgauge, &fuelgaugestyle,LV_PART_MAIN);
		lv_obj_add_style(fuelgauge, &fuelgaugeindicatorstyle, LV_PART_INDICATOR);

		lv_obj_remove_style(frtire, NULL, LV_PART_KNOB);
		lv_obj_remove_style(frtire, NULL, LV_PART_INDICATOR);
		lv_obj_remove_style(fltire, NULL, LV_PART_KNOB);
		lv_obj_remove_style(fltire, NULL, LV_PART_INDICATOR);
		lv_obj_remove_style(rrtire, NULL, LV_PART_KNOB);
		lv_obj_remove_style(rrtire, NULL, LV_PART_INDICATOR);
		lv_obj_remove_style(rltire, NULL, LV_PART_KNOB);
		lv_obj_remove_style(rltire, NULL, LV_PART_INDICATOR);
		lv_obj_remove_style(fuelgauge, NULL, LV_PART_KNOB);
		//lv_obj_remove_style(fuelgauge, NULL, LV_PART_INDICATOR);

    	lv_obj_center(frtire);
		lv_obj_center(fltire);
		lv_obj_center(rrtire);
		lv_obj_center(rltire);

		lv_obj_center(fuelgauge);

		lv_style_init(&gearcirclestyle);
		lv_style_init(&gearlabelstyle);
		lv_style_set_text_font(&gearlabelstyle, &lv_font_dsdigital_120);
		//lv_style_set_text_font(&gearlabelstyle, &lv_font_montserrat_48);
		lv_style_set_text_color(&gearlabelstyle, lv_color_white());

		lv_style_set_radius(&gearcirclestyle, 90);
		lv_style_set_border_width(&gearcirclestyle, 4);
		lv_style_set_border_color(&gearcirclestyle, lv_color_white());
		
		lv_obj_set_size(gearcircle, 180, 180);
    	lv_obj_add_style( gearcircle, &gearcirclestyle,0);
		lv_obj_add_style( gearlabel, &gearlabelstyle,0);
		lv_label_set_text( gearcircle, "");
		lv_label_set_text( gearlabel, "-" );
		lv_obj_align( gearcircle, LV_ALIGN_CENTER, 0, 0 );
    	lv_obj_align( gearlabel, LV_ALIGN_CENTER, 0, 0 );
		//lv_obj_set_style_text_align(gearlabel , LV_TEXT_ALIGN_CENTER, 0);

		lv_obj_set_size(gamelogo, 40, 40);
		lv_image_set_scale(gamelogo, 256*40/240);
		lv_obj_align( gamelogo, LV_ALIGN_CENTER,55,0);

		lv_obj_set_size(lowfuelindicator, 40, 40);
		lv_image_set_scale(lowfuelindicator, 256*40/338);
		lv_obj_align( lowfuelindicator, LV_ALIGN_CENTER,0,61);
	}

	// Called when new data is coming from computer
	void read() {
		String full = "";
		// EXAMPLE 1 - read the whole message and sent it back to simhub as debug message
		// Protocol formula can be set in simhub to anything, it will just echo it
		// -------------------------------------------------------
		//String message = FlowSerialReadStringUntil('\n'); 
		//full = "Message received : " + message;
		//FlowSerialDebugPrintLn(full);

		/*
		// -------------------------------------------------------
		// EXAMPLE 2 - reads speed and gear from the message
		// Protocol formula must be set in simhub to
		// format([DataCorePlugin.GameData.NewData.SpeedKmh],'0') + ';' + isnull([DataCorePlugin.GameData.NewData.Gear],'N')
		// -------------------------------------------------------
		*/

		int speed = FlowSerialReadStringUntil(';').toInt();
		String gear = FlowSerialReadStringUntil(';');
		String fltemp = FlowSerialReadStringUntil(';');
		String frtemp = FlowSerialReadStringUntil(';');
		String rltemp = FlowSerialReadStringUntil(';');
		String rrtemp = FlowSerialReadStringUntil(';');
		int fuelpct = FlowSerialReadStringUntil(';').toInt();
		int fuellow = FlowSerialReadStringUntil('\n').toInt();

		lv_label_set_text( gearlabel, gear.c_str() );

		char* end;
		unsigned long fltempcolor = strtol(fltemp.c_str(), &end, 16);
		unsigned long frtempcolor = strtol(frtemp.c_str(), &end, 16);
		unsigned long rltempcolor = strtol(rltemp.c_str(), &end, 16);
		unsigned long rrtempcolor = strtol(rrtemp.c_str(), &end, 16);

		lv_style_set_arc_color(&fltiretemp, lv_color_hex(fltempcolor));
		lv_obj_invalidate(fltire);
		lv_style_set_arc_color(&frtiretemp, lv_color_hex(frtempcolor));
		lv_obj_invalidate(frtire);
		lv_style_set_arc_color(&rltiretemp, lv_color_hex(rltempcolor));
		lv_obj_invalidate(rltire);
		lv_style_set_arc_color(&rrtiretemp, lv_color_hex(rrtempcolor));
		lv_obj_invalidate(rrtire);

		lv_arc_set_value(fuelgauge, fuelpct);
		lv_obj_invalidate(fuelgauge);

		if (fuellow == 1) {
			lv_obj_remove_flag(lowfuelindicator,LV_OBJ_FLAG_HIDDEN); }
		else {
			lv_obj_add_flag(lowfuelindicator, LV_OBJ_FLAG_HIDDEN); }


		//full = "Speed : " + String(speed);
		//FlowSerialDebugPrintLn(full);

		//full = "Gear : " + gear;
		//FlowSerialDebugPrintLn(full);
		
		//String fltempcolorstring = String(fltempcolor);

		//full = "fltempint : " + fltemp;
		//FlowSerialDebugPrintLn(full);
	}

	// Called once per arduino loop, timing can't be predicted, 
	// but it's called between each command sent to the arduino
	void loop() {
		lv_timer_periodic_handler();
	}

	// Called once between each byte read on arduino,
	// THIS IS A CRITICAL PATH :
	// AVOID ANY TIME CONSUMING ROUTINES !!!
	// PREFER READ OR LOOP METHOS AS MUCH AS POSSIBLE
	// AVOID ANY INTERRUPTS DISABLE (serial data would be lost!!!)
	void idle() {
	}
};

#endif