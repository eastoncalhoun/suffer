#include "./package.hpp"

suffer::core::Package::Package(const std::string& name, const std::string& version, const std::string& author, const std::string& source, const bool headerOnly, const std::map<std::string, std::string>& dependencies) {
    this->name = name;
    this->version = version;
    this->author = author;
    this->source = source;
    this->headerOnly = headerOnly;
    this->dependencies = dependencies;
}

const std::string& suffer::core::Package::getName() {
    return this->name;
}

const std::string& suffer::core::Package::getVersion() {
    return this->version;
}

const std::string& suffer::core::Package::getAuthor() {
    return this->author;
}

const std::string& suffer::core::Package::getSource() {
    return this->source;
}

const std::map<std::string, std::string>& suffer::core::Package::getDependencies() {
    return this->dependencies;
}

const bool suffer::core::Package::isHeaderOnly() {
    return this->headerOnly;
}