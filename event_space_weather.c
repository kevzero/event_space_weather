#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define API_KEY "DEMO_KEY"
#define START_DATE "2025-07-01"
#define END_DATE "2025-07-05"

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void fetch_and_display(const char *url, const char *eventType) {
    CURL *curl = curl_easy_init();
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    if (!curl) return;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        fprintf(stderr, "❌ Error fetching %s: %s\n", eventType, curl_easy_strerror(res));
    } else {
        cJSON *root = cJSON_Parse(chunk.memory);
        if (!root || !cJSON_IsArray(root)) {
            printf("⚠️  No %s events or JSON error.\n\n", eventType);
        } else {
            int count = cJSON_GetArraySize(root);
            if (count == 0) {
                printf("🔎 No %s events found.\n\n", eventType);
            } else {
                printf("🛰️  %s EVENTS:\n\n", eventType);
                for (int i = 0; i < count; i++) {
                    cJSON *item = cJSON_GetArrayItem(root, i);
                    cJSON *time = cJSON_GetObjectItem(item, "startTime");
                    if (!time) time = cJSON_GetObjectItem(item, "flrID");
                    if (time) printf("📅 Time: %s\n", time->valuestring);

                    cJSON *source = cJSON_GetObjectItem(item, "sourceLocation");
                    if (source) printf("🌍 Source: %s\n", source->valuestring);

                    cJSON *link = cJSON_GetObjectItem(item, "link");
                    if (link) printf("🔗 More info: %s\n", link->valuestring);

                    cJSON *videoLinks = cJSON_GetObjectItem(item, "videoLinks");
                    if (videoLinks && cJSON_IsArray(videoLinks)) {
                        for (int j = 0; j < cJSON_GetArraySize(videoLinks); j++) {
                            cJSON *v = cJSON_GetArrayItem(videoLinks, j);
                            printf("🎞️  Video: %s\n", v->valuestring);
                        }
                    }

                    printf("----------------------------------------------------\n");
                }
            }
        }
        cJSON_Delete(root);
    }

    free(chunk.memory);
    curl_easy_cleanup(curl);
}

int main(void) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    char url[512];

    // CME Events
    snprintf(url, sizeof(url),
             "https://api.nasa.gov/DONKI/CME?startDate=%s&endDate=%s&api_key=%s",
             START_DATE, END_DATE, API_KEY);
    fetch_and_display(url, "CME");

    // FLR Events (Solar Flares)
    snprintf(url, sizeof(url),
             "https://api.nasa.gov/DONKI/FLR?startDate=%s&endDate=%s&api_key=%s",
             START_DATE, END_DATE, API_KEY);
    fetch_and_display(url, "FLR");

    // SEP Events (Solar Energetic Particles)
    snprintf(url, sizeof(url),
             "https://api.nasa.gov/DONKI/SEP?startDate=%s&endDate=%s&api_key=%s",
             START_DATE, END_DATE, API_KEY);
    fetch_and_display(url, "SEP");

    // HSS Events (High Speed Streams)
    snprintf(url, sizeof(url),
             "https://api.nasa.gov/DONKI/HSS?startDate=%s&endDate=%s&api_key=%s",
             START_DATE, END_DATE, API_KEY);
    fetch_and_display(url, "HSS");

    // WSA Enlil Simulation
    snprintf(url, sizeof(url),
             "https://api.nasa.gov/DONKI/WSAEnlilSimulations?startDate=%s&endDate=%s&api_key=%s",
             START_DATE, END_DATE, API_KEY);
    fetch_and_display(url, "WSA Enlil Simulation");

    // Notifications
    snprintf(url, sizeof(url),
             "https://api.nasa.gov/DONKI/notifications?startDate=%s&endDate=%s&type=all&api_key=%s",
             START_DATE, END_DATE, API_KEY);
    fetch_and_display(url, "Notifications");

    curl_global_cleanup();
    return 0;
}
