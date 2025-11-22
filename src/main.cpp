#include "./core/registryHandler.hpp"

int main() {
    auto r = suffer::core::RegistryHandler();
    auto p = r.findPackage("foo");

    std::cout << p.getSource() << "\n";

    return 777;
}