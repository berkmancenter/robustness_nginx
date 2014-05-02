#include <stdio.h>
#include <string.h>
#include <time.h>
#include "cayl_utils.h"


#define CAYL_MAX_BEHAVIOR_STRING 20
#define CAYL_MAX_DATE_STRING 30

/* Create a string containing attributes to be added to the HREF

    cayl_options_t *options : configuration settings
    char *out               : buffer to where the attribute is written
    chatr *locatino         : location of the cached copy
    int status              : whether the site is up or down
    time_t date             : when the cache was generated (unix epoch)

    returns 0 on success
*/
int cayl_build_attribute(cayl_options_t *options, unsigned char *out, char *location, int status, time_t date)
{
    char behavior[CAYL_MAX_BEHAVIOR_STRING];
    char date_string[CAYL_MAX_DATE_STRING];

    int rc = cayl_get_behavior(options, behavior, status);
    if (!rc) {
        struct tm *timeinfo = localtime(&date);
        strftime(date_string,CAYL_MAX_DATE_STRING,"%FT%T%z",timeinfo);
        snprintf(out,
                 CAYL_MAX_ATTRIBUTE_STRING,
                 "data-cache='%s %s' data-cayl-behavior='%s' ",
                 location,
                 date_string,
                 behavior
                 );
         }

    return rc;
}

/* Get the contents of behavior attribute based on the status of the link
   and the configuration settings.

   cayl_options_t *options : configuration settings
   char *out               : buffer to where the attribute is written
   int status              : whether the site is up or down

   returns 0 on success

   TODO: Country-specific behavior
   */
int cayl_get_behavior(cayl_options_t *options, unsigned char *out, int status) {
    if (!options || !out) {
        return 1;
    }
    if (status == CAYL_STATUS_UP) {
        switch (options->behavior_up) {
            case CAYL_ACTION_HOVER:
                snprintf(out,
                         CAYL_MAX_ATTRIBUTE_STRING,
                         "up hover:%d",
                         options->hover_delay_up);
                break;
            case CAYL_ACTION_POPUP:
                snprintf(out, CAYL_MAX_ATTRIBUTE_STRING, "up popup");
                break;
            case CAYL_ACTION_CACHE:
                snprintf(out, CAYL_MAX_ATTRIBUTE_STRING, "up cache");
                break;
            case CAYL_ACTION_NONE:
                break;
        }
    } else if (status == CAYL_STATUS_DOWN) {
        switch (options->behavior_down) {
            case CAYL_ACTION_HOVER:
                snprintf(out,
                         CAYL_MAX_ATTRIBUTE_STRING,
                         "down hover:%d",
                         options->hover_delay_down);
                break;
            case CAYL_ACTION_POPUP:
                snprintf(out, CAYL_MAX_ATTRIBUTE_STRING, "down popup");
                break;
            case CAYL_ACTION_CACHE:
                snprintf(out, CAYL_MAX_ATTRIBUTE_STRING, "down cache");
                break;
            case CAYL_ACTION_NONE:
                break;

        }
    }
    return 0;
}