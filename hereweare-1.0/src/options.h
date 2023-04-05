#define DEFAULT_CONFIG_FILE "/etc/hereweare.config"
#define DEFAULT_MOUSE_DEVICE ""
#define DEFAULT_KEYBOARD_DEVICE ""
#define DEFAULT_HOME_ASSISTANT_URL "http://homeassistant.local"
#define DEFAULT_HOME_ASSISTANT_TOKEN ""
#define DEFAULT_INACTIVE_WAIT_TIME 10
#define DEFAULT_ACTIVE_WAIT_TIME 600
#define DEFAULT_SCAN_TIME 60
#define DEFAULT_JSON_DATA "{}"

struct options
{
    const char *mouse_device;
    const char *keyboard_device;
    const char *home_assistant_url;
    char *home_assistant_active_url;
    char *home_assistant_inactive_url;
    const char *home_assistant_token;
    char *json_data;
    int active_wait_time;
    int inactive_wait_time;
    int scan_time;
};

void print_options(FILE *stream, struct options *opts);

struct options load_config(char *config_filename);

void free_opts(struct options *opts);