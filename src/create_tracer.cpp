#include <cstdio>

#include <memory>

#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

#include "create_tracer.hpp"

namespace {
class Logger : public jaegertracing::logging::Logger {
public:
  Logger() = default;

  ~Logger() override = default;

  void error(const std::string& message) override {
    fprintf(stderr, "JAEGER ERROR: \"%s\"\n", message.c_str());
  }

  void info(const std::string&) override {
  }
};
} // namespace

std::shared_ptr<opentracing::Tracer>
create_tracer(const std::string& config_filepath, const std::string& service) {
  try {
    auto config_yaml = YAML::LoadFile(config_filepath);
    auto config = jaegertracing::Config::parse(config_yaml);
    return jaegertracing::Tracer::make(service, config,
                                       std::make_unique<Logger>());
  } catch (const std::runtime_error& ex) {
    if (strstr(ex.what(), "Cannot connect socket to remote address") != nullptr)
      fprintf(stderr, "Exception caught: \"%s\"\nIs Jaeger running?\n",
              ex.what());

    throw;
  }
}
