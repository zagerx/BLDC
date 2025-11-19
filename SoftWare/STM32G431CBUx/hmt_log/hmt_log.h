#ifndef HMT_LOG_H
#define HMT_LOG_H

// 日志级别配置
#define LOG_LEVEL_DEBUG 0 // 打印所有日志
#define LOG_LEVEL_INFO 1  // 打印 ERROR, WARN, INFO
#define LOG_LEVEL_WARN 2  // 打印 ERROR, WARN
#define LOG_LEVEL_ERROR 3 // 只打印 ERROR
#define LOG_LEVEL_NONE 4  // 不打印任何日志

// 设置当前日志级别（根据需要修改这个值）
#ifndef CURRENT_LOG_LEVEL
#define CURRENT_LOG_LEVEL LOG_LEVEL_DEBUG // 默认打印所有日志
#endif

typedef enum
{
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
} log_level_t;

void log_output(log_level_t level, const char *file, int line, const char *format, ...);

// 根据当前日志级别决定是否输出日志
#if CURRENT_LOG_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_E(...) log_output(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_W(...) log_output(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_I(...) log_output(LOG_INFO, NULL, 0, __VA_ARGS__)
#define LOG_D(...) log_output(LOG_DEBUG, NULL, 0, __VA_ARGS__)
#elif CURRENT_LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_E(...) log_output(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_W(...) log_output(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_I(...) log_output(LOG_INFO, NULL, 0, __VA_ARGS__)
#define LOG_D(...) (void)0 // 禁用 DEBUG 日志
#elif CURRENT_LOG_LEVEL <= LOG_LEVEL_WARN
#define LOG_E(...) log_output(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_W(...) log_output(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_I(...) (void)0 // 禁用 INFO 日志
#define LOG_D(...) (void)0 // 禁用 DEBUG 日志
#elif CURRENT_LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_E(...) log_output(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_W(...) (void)0 // 禁用 WARN 日志
#define LOG_I(...) (void)0 // 禁用 INFO 日志
#define LOG_D(...) (void)0 // 禁用 DEBUG 日志
#else
#define LOG_E(...) (void)0 // 禁用所有日志
#define LOG_W(...) (void)0
#define LOG_I(...) (void)0
#define LOG_D(...) (void)0
#endif
#endif
