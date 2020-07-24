#include "fpsControl.h"
#include "../typing.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#ifndef MICRO
    #define MICRO 1000000
#endif

typedef struct {
    struct timeval start_time;
    struct timeval current_time;
    int *elapsed_time;
    int *isUpdated;
} FPS_INFO;
FPS_INFO *fps_info;

void fps_init(int *elapsed_time, int *isUpdated) {
    fps_info = (FPS_INFO *)malloc(sizeof(FPS_INFO));
    fps_info->elapsed_time = elapsed_time;
    fps_info->isUpdated = isUpdated;
    gettimeofday(&fps_info->start_time, NULL);
}

int fps_elapsed(void) {
    gettimeofday(&fps_info->current_time, NULL);
    time_t elapsed_micro = (fps_info->current_time.tv_sec - fps_info->start_time.tv_sec) * MICRO + (fps_info->current_time.tv_usec - fps_info->start_time.tv_usec);
    double elapsed_frames = elapsed_micro * FPS / MICRO;
    if (*fps_info->isUpdated = *fps_info->elapsed_time != elapsed_frames) {
        *fps_info->elapsed_time = elapsed_frames;
    }
    return *fps_info->isUpdated;
}
