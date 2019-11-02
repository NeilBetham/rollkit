#include <stdio.h>
#include <string.h>
#include "unity.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static void print_banner(const char* text);

void run_tests() {
  print_banner("Running all the registered tests");
  UNITY_BEGIN();
  unity_run_all_tests();
  UNITY_END();
  while(1) { vTaskDelay(10); }
}

void app_main()
{
  TaskHandle_t test_task_handle = NULL;
  xTaskCreate(
    run_tests,
    "Tests",
    40000,
    NULL,
    tskIDLE_PRIORITY,
    &test_task_handle
  );
  while(1) { vTaskDelay(10); }
}

static void print_banner(const char* text)
{
    printf("\n#### %s #####\n\n", text);
}
