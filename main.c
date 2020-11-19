#include "lvgl/lvgl.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

extern void lv_8ms_init(void);
extern void lv_8ms_deinit(void);
extern void lv_qm_ui_entry(void);
extern void lv_qm_ui_loop(void);

uint32_t custom_tick_get(void);

int main(void)
{
    /*8ms init*/
    lv_8ms_init();

    lv_qm_ui_entry();

    while(1) {
        lv_qm_ui_loop();
        lv_task_handler();
        usleep(5000);
    }

    lv_8ms_deinit();

    return 0;
}

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
