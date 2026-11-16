#include "test_framework.hpp"
#include "core/Logger.hpp"
#include <memory>

using namespace kow::core;

TEST(logger_level_filtering_and_capture) {
    Logger logger(LogLevel::Warn);
    auto sink = std::make_shared<VectorSink>();
    logger.clear_sinks();
    logger.add_sink(sink);

    logger.debug("debug-msg");
    logger.info("info-msg");
    logger.warn("warn-msg");
    logger.error("error-msg");

    auto entries = sink->entries();
    ASSERT_EQ(entries.size(), static_cast<std::size_t>(2));
    ASSERT_EQ(entries[0].message, std::string("warn-msg"));
    ASSERT_EQ(static_cast<int>(entries[0].level), static_cast<int>(LogLevel::Warn));
    ASSERT_EQ(entries[1].message, std::string("error-msg"));
    return true;
}

TEST(logger_set_level_and_clear) {
    Logger logger(LogLevel::Info);
    auto sink = std::make_shared<VectorSink>();
    logger.clear_sinks();
    logger.add_sink(sink);
    logger.info("a");
    logger.set_level(LogLevel::Error);
    logger.info("b");
    logger.error("c");
    auto entries = sink->entries();
    ASSERT_EQ(entries.size(), static_cast<std::size_t>(2)); // "a" and "c"
    sink->clear();
    ASSERT_EQ(sink->entries().size(), static_cast<std::size_t>(0));
    return true;
}
