/*
 * TODO: add custom format and custom header, add custom thread id?
 *      add a default nayukiyq category to logger.
 *      check log category prefix to log.
 */
module;

#include <module_export.h>

export module core.logger;

import core;

// 添加这个前置声明，避免 clangd 警告报错
template <typename T>
class Singleton;

// TODO: Output to console or debugger at the same time.
export class CORE_API Logger : public Singleton<Logger>
{
  public:
    friend class Singleton<Logger>;

  protected:
    Logger()
        : Singleton<Logger>()
    {
    }

  public:
    // Register and create a logger.
    void RegisterCategory( const LoggerCategory& loggerCategory );

    std::shared_ptr<spdlog::logger> Get( const LoggerCategory& loggerCategory );

  private:
    std::shared_ptr<spdlog::logger> CreateSyncLogger( const LoggerCategory& loggerCategory );
    std::shared_ptr<spdlog::logger> CreateAsyncLogger( const LoggerCategory& loggerCategory );
};
