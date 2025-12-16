#ifndef DEVICE_H
#define DEVICE_H
struct device {
    const char *name;
    void *config;
    void *api;
    void *data;
    // struct device_state *state;
};
#endif
