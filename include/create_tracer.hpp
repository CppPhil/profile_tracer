#pragma once
#include <string>

#include <opentracing/tracer.h>

std::shared_ptr<opentracing::Tracer>
create_tracer(const std::string& config_filepath, const std::string& service);
