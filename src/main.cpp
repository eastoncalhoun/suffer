#include <iostream>
#include <unistd.h>

#include <suffer/commands/router.hpp>
#include <suffer/utils/io.hpp>

int main(int argc, char** argv) {
    if (geteuid() == 0) {
        std::cerr << suffer::utils::io::error() << " NO! ABSOLUTELY NOT!!\nDo " << suffer::utils::io::red("NOT") << " run suffer as root!!!\n";
        exit(EXIT_FAILURE);
    }

    suffer::commands::Router router = suffer::commands::Router(argc, argv);
    router.route();

    return EXIT_SUCCESS;
}