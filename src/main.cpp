
#ifndef ALLEGROINITS_H_
#define ALLEGROINITS_H_

// Version 1.0

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_native_dialog.h>

class AllegroInits {
public:
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event_queue;

	AllegroInits();
	virtual ~AllegroInits();
};

#endif 