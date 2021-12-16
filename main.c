#define G_LOG_USE_STRUCTURED

#include <glib.h>
#include <glib/gprintf.h>

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

GLogWriterOutput log_writer (
    GLogLevelFlags log_level,
    const GLogField *fields,
    gsize n_fields,
    gpointer data
) {
    if (log_level < G_LOG_LEVEL_MESSAGE) return G_LOG_WRITER_HANDLED;

    GLogLevelFlags prio = G_LOG_LEVEL_MASK;
    const gchar *c_file = NULL;
    const gchar *c_line = NULL;
    const gchar *c_func = NULL;
    const GLogField *field = fields;
    for (gsize k = 0; k < n_fields; k++) {
        printf("%s\n", field->key);
        if (g_strcmp0("CODE_FILE", field->key) == 0) c_file = field->value;
        if (g_strcmp0("CODE_FILE", field->key) == 0) c_file = field->value;
        if (g_strcmp0("CODE_LINE", field->key) == 0) c_line = field->value;
        if (g_strcmp0("CODE_FUNC", field->key) == 0) c_func = field->value;
        field++;
    }
    gchar *msg = g_log_writer_format_fields(log_level, fields, n_fields, FALSE);

    // writes to stdout
    printf("%s:%s [%s] %s\n", c_file, c_line, c_func, msg);
    // write to syslog
    syslog(LOG_NOTICE, "%s:%s [%s] %s\n", c_file, c_line, c_func, msg);

    g_free(msg);
    return G_LOG_WRITER_HANDLED;
}

static void test_func(void) {
    gint k = 0;
    g_printf("This is a %s call %d\n", "g_printf", k++);
    g_message("This is a %s log call %d\n", "g_message", k++);
    g_info("This is a %s log call %d\n", "g_info", k++);
    g_debug("This is a %s log call %d\n", "g_debug", k++);
    g_warning("This is a %s log call %d\n", "g_warning", k++);
}

int main(int argc, char *argv[]) {
    g_log_set_fatal_mask(NULL, G_LOG_LEVEL_CRITICAL);
    g_log_set_writer_func(log_writer, NULL, NULL);

    openlog("glib_log", LOG_NDELAY, 0);
    
    gint k = 0;
    g_printf("This is a %s call %d\n", "g_printf", k++);
    g_message("This is a %s log call %d\n", "g_message", k++);
    g_info("This is a %s log call %d\n", "g_info", k++);
    g_debug("This is a %s log call %d\n", "g_debug", k++);
    g_warning("This is a %s log call %d\n", "g_warning", k++);
    test_func();

    gint *state_object = NULL;
    const GLogField fields[] = {
      { "MESSAGE", "This is a debug message.", -1 },
      { "MESSAGE_ID", "fcfb2e1e65c3494386b74878f1abf893", -1 },
      { "MY_APPLICATION_CUSTOM_FIELD", "some debug string", -1 },
      { "MY_APPLICATION_STATE", state_object, 0 },
    };
    g_log_structured_array(G_LOG_LEVEL_DEBUG, fields, G_N_ELEMENTS (fields));
    return 0;
}
