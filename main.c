#include "lvgl/lvgl.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "lv_port_disp.h"
#include "main/qm_ui_entry.h"
extern void logo_init(void);
uint32_t custom_tick_get(void);

int main(void)
{
	/*LittlevGL init*/
	lv_init();
	lv_port_disp_init();

#ifdef LV_DEMO_TEST
	lv_demo_widgets();
#else

#endif
	
	lv_qm_ui_entry();
	/*Handle LitlevGL tasks (tickless mode)*/
	while(1) {
#ifndef LV_DEMO_TEST
		lv_qm_ui_loop();
#endif
		lv_task_handler();
		usleep(5000);
	}

	lv_port_disp_deinit();
	lv_deinit();

	return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
	static uint64_t start_ms = 0;
	if(start_ms == 0) {
		struct timeval tv_start;
		gettimeofday(&tv_start, NULL);
		start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
	}

	struct timeval tv_now;
	gettimeofday(&tv_now, NULL);
	uint64_t now_ms;
	now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

	uint32_t time_ms = now_ms - start_ms;
	return time_ms;
}
