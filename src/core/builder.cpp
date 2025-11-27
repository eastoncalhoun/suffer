#include "./builder.hpp"

void suffer::core::Builder::checkPermissions(const std::filesystem::path& path) {
    const std::filesystem::perms perms = std::filesystem::status(path).permissions();

    if ((perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none && (perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none) {
        std::cerr << suffer::utils::io::error() << " No read/write permissions in " << suffer::utils::io::dataString(path.string()) << "\n";
        exit(EXIT_FAILURE);
    }
}

int suffer::core::Builder::determineHeaderPackaging() {
    int type = UNKNOWN;
    std::filesystem::path pPath = this->package.determinePath();

    if (!std::filesystem::exists(pPath)) {
        std::cerr << suffer::utils::io::error() << " There is nothing at " << suffer::utils::io::dataString(pPath.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    if (std::filesystem::exists(pPath / "single_include")) {
        type = SI_H_HEADER_STYLE;
    } else if (std::filesystem::exists(pPath / "include")) {
        type = IH_H_HEADER_STYLE;
    } else if (this->getHeadersFromRoot().size() > 0) {
        type = RT_H_STYLE;
    }

    return type;
}

std::vector<std::filesystem::path> suffer::core::Builder::getHeadersFromRoot() {
    std::vector<std::filesystem::path> headers = std::vector<std::filesystem::path>();

    for (auto& e : std::filesystem::directory_iterator(this->package.determinePath())) {
        if (e.is_directory()) {
            continue;
        }

        std::string extension = e.path().extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        
        if (extension == ".hh" || extension == ".hpp" || extension == ".h" || extension == ".hxx") {
            headers.push_back(e.path());
        }
    }

    return headers;
}

void suffer::core::Builder::prevImportDetected() {
    std::string input;
    std::cout << suffer::utils::io::info() << " Previous import of " << suffer::utils::io::dataString(this->package.getName()) << " detected\n";
    std::cout << suffer::utils::io::red("Would you like to overwrite? ") << suffer::utils::io::yesNo();

    std::cin >> input;

    if (input != "yes" && input != "y") {
        std::cout << suffer::utils::io::okay() << " Exiting due to previous import\n";
        exit(EXIT_SUCCESS);
    }
}

void suffer::core::Builder::importHeaders(const std::filesystem::path& include, const std::filesystem::path& libPath) {
    //only used if RTH_H_STYLE
    const std::filesystem::path includeProject = include / this->package.getName();

    auto headers = std::vector<std::filesystem::path>();
    const int packaging = this->determineHeaderPackaging();
    bool prevAcknowledged = false;

    //if the /project/includes dir contains some of the registry's headers already, prompt user for re import.  
    std::vector<std::filesystem::path> libIncludePaths = std::vector<std::filesystem::path>();
    
    switch (packaging) {
        case SI_H_HEADER_STYLE:
            for (auto& element : std::filesystem::directory_iterator(libPath / "single_include")) {
                if (std::filesystem::exists(include / element.path().filename()) && !prevAcknowledged) {
                    this->prevImportDetected();
                    prevAcknowledged = true;
                }

                std::filesystem::copy(element.path(), include / element.path().filename(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
            }

            break;

        case IH_H_HEADER_STYLE:
            for (auto& element : std::filesystem::directory_iterator(libPath / "include")) {
                if (std::filesystem::exists(include / element.path().filename()) && !prevAcknowledged) {
                    this->prevImportDetected();
                    prevAcknowledged = true;
                    break;
                }
            }

            std::filesystem::copy(libPath / "include", include, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);

            break;

        case RT_H_STYLE:
            headers = this->getHeadersFromRoot();
            
            if (!std::filesystem::exists(includeProject)) {
                std::cout << suffer::utils::io::info() << " Attempting to create " << suffer::utils::io::dataString(includeProject.string()) << "\n";

                if (!std::filesystem::create_directory(includeProject)) {
                    std::cerr << suffer::utils::io::error() << " Failed to create " << suffer::utils::io::dataString(includeProject.string()) << "\n";
                    exit(EXIT_FAILURE);
                }
            }

            for (auto& header : headers) {
                if (std::filesystem::exists(includeProject / header.filename()) && !prevAcknowledged) {
                    this->prevImportDetected();
                    prevAcknowledged = true;
                }

                std::filesystem::copy(header, includeProject, std::filesystem::copy_options::overwrite_existing);
            }

            break;

        case UNKNOWN:
            std::cerr << suffer::utils::io::error() << " Unknown header structure for " << suffer::utils::io::dataString(this->package.getName()) << "\n";
            exit(EXIT_FAILURE);

            break;

        default:
            std::cerr << suffer::utils::io::error() << " Somehow the structure to " << suffer::utils::io::dataString(this->package.getName()) << " is so weird, the type isn't even UNKNOWN\n";
            exit(EXIT_FAILURE);

            break;
    }

    std::cout << suffer::utils::io::okay() << " Copied over headers from " << suffer::utils::io::dataString(this->package.getName()) << "\n";
}

void suffer::core::Builder::import() {
    std::cout << suffer::utils::io::okay() << " Attempting to import " << suffer::utils::io::dataString(this->package.getName()) << " to " << suffer::utils::io::dataString(std::filesystem::current_path().string()) << "\n";

    const std::filesystem::path libPath = this->package.determinePath();
    const std::filesystem::path include = std::filesystem::current_path() / "include";

    this->checkPermissions(libPath);
    this->checkPermissions(include);

    if (!std::filesystem::exists(libPath)) {
        std::cerr << suffer::utils::io::error() << " The path " << suffer::utils::io::dataString(libPath.string()) << " does not exist\n";
        exit(EXIT_FAILURE);
    }
    
    if (!std::filesystem::exists(include)) {
        std::cerr << suffer::utils::io::info() << " No project /include directory detected\n";

        if (std::filesystem::create_directory(include)) {
            std::cout << suffer::utils::io::okay() << " Created " << suffer::utils::io::dataString(include.string()) << "\n";
        } else {
            std::cout << suffer::utils::io::error() << " Failed to create " << suffer::utils::io::dataString(include.string()) << "\n";
            exit(EXIT_FAILURE);
        }
    }

    this->importHeaders(include, libPath);

    const std::map<std::string, std::string>& dependencies = this->package.getDependencies();

    for (auto& keyValue : dependencies) {
        std::cout << suffer::utils::io::info() << " Importing dependency " << suffer::utils::io::dataString(keyValue.first) << "\n";
        suffer::core::Package p = registry.findPackage(keyValue.first);
        suffer::core::Builder builder = suffer::core::Builder(p, this->registry);

        builder.import();
    }

    if (!this->package.isHeaderOnly()) {
        std::cout << suffer::utils::io::info() << " Building " << suffer::utils::io::dataString(this->package.getName());
    }

    std::cout << suffer::utils::io::okay() << " Successfully imported " << suffer::utils::io::dataString(this->package.getName()) << "\n";
}

suffer::core::Builder::Builder(suffer::core::Package& package, suffer::core::RegistryHandler& registry): package(package), registry(registry) {}