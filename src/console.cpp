#include <Arduino.h>
#include "KNOO.h"
#include "console.h"
#include "simple_log.h"
#define LOGGER_MODULE_NAME "console"

#ifndef EOF
#define EOF (-1)
#endif

#define PRINTF(...) Serial.printf(__VA_ARGS__)
#define GETCHAR() (Serial.available()?Serial.read():EOF)

#define CONSOLE_STATUSES \
    X(CONSOLE_OK), \
    X(CONSOLE_UNKNOWN_COMMAND), \
    X(CONSOLE_INPUT_TOO_LONG), \
    X(CONSOLE_INVALID_ARGUMENT), \
    X(CONSOLE_INVALID_ARG_VALUE), \
    X(CONSOLE_TOO_MANY_ARGUMENTS), \
    X(CONSOLE_MISSING_ARGUMENT), \
     \
    X(CONSOLE_MAX), 

#define X(n) n
typedef enum {
    CONSOLE_STATUSES
} CONSOLE_STATUS;
#undef X

#define X(n) #n
const char *console_status_str[] = {
    CONSOLE_STATUSES
};
#undef X

typedef struct {
    char line[200];
    char *cursor;
    char *line_end;
    bool discard_line;
} CONSOLE;

CONSOLE console = {
    .cursor = console.line,
    .line_end = console.line + sizeof(console.line)
};

static const char * console_status_to_str(CONSOLE_STATUS status)
{
    if (status > CONSOLE_MAX || status < 0)
        return "UNKNOWN STATUS";
    return console_status_str[status];
}

static CONSOLE_STATUS echo_app(int argc, char *argv[])
{
    for (int i=0; i<argc; i++)
        PRINTF(">>%s<<\n", argv[i]);
    return CONSOLE_OK;
}

static CONSOLE_STATUS mode_app(int argc, char *argv[])
{
    if (argc==0)
        PRINTF("%d\n", KNOO.mode);
    else if (argc==1)
    {
        int mode;
        if(0==sscanf(argv[0], "%d", &mode) || mode < 0 || mode >= KNOO_MODE_MAX)
            return CONSOLE_INVALID_ARG_VALUE;
        KNOO.mode = (KNOO_MODE)mode;
    }
    else
        return CONSOLE_TOO_MANY_ARGUMENTS;
    return CONSOLE_OK;
}


static CONSOLE_STATUS log_app(int argc, char *argv[])
{
    if (argc < 2)
        return CONSOLE_MISSING_ARGUMENT;
    int level;
    if(0 >= sscanf(argv[0], "%d", &level))
    {
        return CONSOLE_INVALID_ARG_VALUE;
    }
    simple_log((LOG_LEVEL)level, LOGGER_MODULE_NAME, argv[1]);
    return CONSOLE_OK;
}

static CONSOLE_STATUS log_level_app(int argc, char *argv[])
{
    if (argc < 2)
        return CONSOLE_MISSING_ARGUMENT;
    LOG_LEVEL level;
    if(0 >= sscanf(argv[1], "%d", &level))
    {
        return CONSOLE_INVALID_ARG_VALUE;
    }
    if (0 == strcmp("console", argv[0]))
        simple_log_set_console_level(level);
    else if (0 == strcmp("storage", argv[0]))
        simple_log_set_storage_level(level);
    else
        return CONSOLE_INVALID_ARG_VALUE;
    return CONSOLE_OK;
}

typedef CONSOLE_STATUS (*console_app)(int argc, char *argv[]);
static const struct {
    char *name;
    console_app app;
} console_apps[] = {
#define APP(n) {.name = (char *) #n, .app = n##_app}
    APP(echo),
    APP(mode),
    APP(log_level),
    APP(log),
#undef APP
};

static bool is_whitespace(char c) { return c == ' ' || c == '\t'; }

static char * skip_token(char *line)
{
    while('\0' != *line && !is_whitespace(*line))
        line++;
    return line;
}

static char * remove_whitespaces(char *line)
{
    while(is_whitespace(*line))
        *(line++) = '\0';
    return line;
}

static CONSOLE_STATUS console_execute(char *line)
{
    char *argv[CONSOLE_MAX_ARGS];
    char argc = 0;
    char *c = remove_whitespaces(line);
    while('\0' != *c)
    {
        argv[argc++] = c;
        if (argc >= ARR_SIZE(argv))
            return CONSOLE_TOO_MANY_ARGUMENTS;
        c = remove_whitespaces(skip_token(c));
    }

    for (int i=0; i < sizeof(console_apps)/sizeof(console_apps[0]); i++)
        if ( 0 == strcmp(console_apps[i].name, argv[0]))
            return console_apps[i].app(argc-1, argv+1);

    return CONSOLE_UNKNOWN_COMMAND;
}

static void console_reset(void)
{
    console.cursor = console.line;
    console.discard_line = false;
}

void console_process(void)
{
    for (int c; (c = GETCHAR()) != EOF;)
    {
        if (console.discard_line)
        {
            if ('\n' == c)
                console_reset();
            continue;
        }

        switch(c){
            case KEY_BACKSPACE:
                if(console.cursor > console.line)
                {
                    console.cursor--;
                    PRINTF("%c %c", KEY_BACKSPACE,KEY_BACKSPACE);
                }
                break;
            case '\r':
                break;
            case '\n':
                *console.cursor = '\0';
                PRINTF("\n");
                if(console.cursor != console.line)
                    PRINTF("%s\n", console_status_to_str(console_execute(console.line)));
                console_reset();
                break;
            default:
                PRINTF("%c", c);
                *(console.cursor++) = c;
        }

        if (console.cursor == console.line_end)
        {
            console.discard_line = true;
            PRINTF("%s\n", console_status_to_str(CONSOLE_INPUT_TOO_LONG));
        }
    }
}