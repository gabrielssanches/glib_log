#include "test_file.h"

#include <glib/gprintf.h>

void test_func(void) {
    gint k = 0;
    g_printf("This is a %s call %d\n", "g_printf", k++);
    g_message("This is a %s log call %d\n", "g_message", k++);
    g_info("This is a %s log call %d\n", "g_info", k++);
    g_debug("This is a %s log call %d\n", "g_debug", k++);
    g_warning("This is a %s log call %d\n", "g_warning", k++);
}

