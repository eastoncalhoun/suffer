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
            for (auto& element : std::filesystem::directory_iterator(include)) {
                std::filesystem::copy(element.path(), include / element.path().filename(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
            }

            break;

        case IH_H_HEADER_STYLE:
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

std::filesystem::path suffer::core::Builder::findStaticLibLocation() {
    const std::string libName = "lib" + this->package.getName() + ".a";
    const std::filesystem::path libPath = this->package.determinePath();

    for (auto& entry : std::filesystem::recursive_directory_iterator(libPath)) {
        std::string entryName = entry.path().filename().string();
        
        if (entryName.find(libName) != std::string::npos && entryName.size() == libName.size()) {
            return entry.path();
        }
    }

    std::cerr << suffer::utils::io::error() << " No " << suffer::utils::io::dataString(libName) << " found in " << suffer::utils::io::dataString(libPath.string());
    exit(EXIT_FAILURE);
}

std::vector<std::filesystem::path> suffer::core::Builder::detectBuiltHeaders() {
    std::filesystem::path libPath = this->package.determinePath();
    std::vector<std::filesystem::path> headers = {};

    for (auto& element : std::filesystem::directory_iterator(libPath)) {
        const std::filesystem::path elementPath = element.path();
        const std::string eName = element.path().filename().string();
        const bool buildish = eName.find("build") != std::string::npos || eName.find("out") != std::string::npos || eName.find("generated")!= std::string::npos;

        if (buildish && element.is_directory())  {
            for (auto& e : std::filesystem::recursive_directory_iterator(elementPath)) {
                const std::filesystem::path ePath = e.path();
                const std::string extension = ePath.filename().extension().string();
                
                if (extension == ".h" || extension == ".hpp" || extension == ".hh" || extension == ".hxx") {
                    headers.push_back(ePath);
                }
            }
        }
    }

    return headers;
}

void suffer::core::Builder::compileLib() {
    const std::filesystem::path libPath = this->package.determinePath();
    const std::filesystem::path include = libPath / "include";

    if (!std::filesystem::exists(libPath)) {
        std::cerr << suffer::utils::io::error() << " Library not found at " << suffer::utils::io::dataString(libPath) << "\n";
        exit(EXIT_FAILURE);
    }

    if (std::filesystem::exists(libPath / "CMakeLists.txt")) {
        std::cout << suffer::utils::io::info() << " Detected CMake\n";

        std::string flags = this->package.getFlags();
        std::string cmakeGen = "cd " + libPath.string()  + " && cmake -S . -B build -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release " + flags;
        std::string cmakeBuild = "cd " + libPath.string() + " && cmake --build build --config Release";
        
        if (system(cmakeGen.c_str()) != 0) {
            std::cout << suffer::utils::io::error() << " Failed to generate the static CMake build for " << suffer::utils::io::dataString(this->package.getName()) << "\n";
            exit(EXIT_FAILURE);
        }

        if (system(cmakeBuild.c_str()) != 0) {
            std::cout << suffer::utils::io::error() << " CMake failed to compile " << suffer::utils::io::dataString(this->package.getName()) << "\n";
            exit(EXIT_FAILURE);
        }
    }

    const std::filesystem::path staticLib = this->findStaticLibLocation();

    if (!std::filesystem::exists(staticLib)) {
        std::cout << suffer::utils::io::error() << " The supposed library at " << suffer::utils::io::dataString(staticLib.string()) << " does not exist\n";
        exit(EXIT_FAILURE);
    }

    std::filesystem::copy(staticLib, registry.getCachePath(), std::filesystem::copy_options::overwrite_existing);

    std::cout << suffer::utils::io::okay() << " Successfully compiled and cached " << suffer::utils::io::dataString(this->package.getName()) << "\n";
}

bool suffer::core::Builder::isCached() {
    bool cached = false;

    if (std::filesystem::exists(this->package.determineCachePath())) {
        cached = true;
    }

    return cached;
}

int suffer::core::Builder::determineLinkingIndex() {
    std::filesystem::path filePath = std::filesystem::current_path() / "suffer.project.json";
    bool newJson = !std::filesystem::exists(filePath);
    
    if (newJson) {
        std::ofstream projectFile(filePath);

        if (!projectFile) {
            std::cerr << suffer::utils::io::error() << " Failed to open " << suffer::utils::io::dataString(filePath.string()) << "\n";
            exit(EXIT_FAILURE);
        }


        projectFile << "{\"link\" : []} ";
        projectFile.close();
    }

    std::ifstream jsonFile { filePath };
    std::string buffer;

    if (!jsonFile) {
        std::cerr << suffer::utils::io::error() << " Failed to open " << suffer::utils::io::dataString(filePath.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    buffer = std::string(std::istreambuf_iterator<char>(jsonFile), std::istreambuf_iterator<char>());
    jsonFile.close();

    nlohmann::json jsonObj;

    try {
        jsonObj = nlohmann::json::parse(buffer);
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Invalid json in " << suffer::utils::io::dataString(filePath.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    return jsonObj["link"].size();
}

void suffer::core::Builder::createProjectJson(int index, const std::vector<std::string>& sysLibs) {
    const std::filesystem::path jsonPath = std::filesystem::current_path() / "suffer.project.json";

    //upon running it will already have been proven valid file and json via determineLinkingIndex
    std::ifstream currProjectFile(jsonPath);
    std::string jString = std::string(std::istreambuf_iterator<char>(currProjectFile), std::istreambuf_iterator<char>());
    currProjectFile.close();

    nlohmann::json projectObj = nlohmann::json::parse(jString);

    if (projectObj["link"].size() <= index) {
        projectObj["link"].push_back(nlohmann::json::array());
    }

    projectObj["link"][index].push_back("-l" + this->package.getName());

    for (auto& lib : sysLibs) {
        projectObj["link"][index].push_back("-l" + lib);
    }

    std::ofstream projectFileOut(jsonPath, std::ios::out | std::ios::trunc);
    
    projectFileOut << projectObj.dump(4);
    projectFileOut <<  "\n";
    projectFileOut.close();
}

std::string suffer::core::Builder::determineLinkOrder() {
    std::vector<std::string> links = std::vector<std::string>();
    std::ifstream projectFile(std::filesystem::current_path() / "suffer.project.json");
    std::string jString = std::string(std::istreambuf_iterator<char>(projectFile), std::istreambuf_iterator<char>());

    projectFile.close();

    nlohmann::json jObj = nlohmann::json::parse(jString);

    try {
        for (int i = 0; i < jObj["link"].size(); i++) {
            for (int j = 0; j < jObj["link"][i].size(); j++) {
                std::string flag = jObj["link"][i][j].get<std::string>();
                
                if (std::find(links.begin(), links.end(), flag) != links.end()) {
                    continue;
                } else {
                    links.push_back(flag);
                }
            }
        }
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Invalid suffer.project.json\n";
        std::cerr << suffer::utils::io::red(e.what()) << "\n";
        exit(EXIT_FAILURE);
    }

    std::string linkOrder = "";

    for (auto& element : links) {
        linkOrder = linkOrder + " " + element;
    }

    linkOrder.replace(0, 1, "");

    return linkOrder;
}

void suffer::core::Builder::createMakeFile() {
    std::string makeFileText = "";
    makeFileText = makeFileText + "build:\n";
    makeFileText = makeFileText + "\tg++ -o ./out/program -I ./include $(shell find ./src -name \"*.cpp\" -o -name \"*.cc\" -o -name \"*.cxx\" -o -name \"*.c++\" -o -name \"*.C\") ";

    if (std::filesystem::exists(std::filesystem::current_path() / "lib")) {
        makeFileText = makeFileText + "-L ./lib " + this->determineLinkOrder();
    }

    makeFileText = makeFileText + "\nrun:\n";
    makeFileText = makeFileText + "\t./out/program\n";

    std::ofstream makeFile(std::filesystem::current_path() / "Makefile", std::ios::trunc);
    makeFile << makeFileText;
    makeFile.close();
}

void suffer::core::Builder::import(int index, bool root) {
    std::cout << suffer::utils::io::info() << " Attempting to import " << suffer::utils::io::dataString(this->package.getName()) << " to " << suffer::utils::io::dataString(std::filesystem::current_path().string()) << "\n";

    const std::filesystem::path libPath = this->package.determinePath();
    const std::filesystem::path curr = std::filesystem::current_path();
    const std::filesystem::path include = curr / "include";
    const std::filesystem::path src = curr / "src";
    const std::filesystem::path out = curr / "out";

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
    std::vector<std::string> sysLibs = {};

    for (auto& keyValue : dependencies) {
        if (keyValue.second != "sys") {
            suffer::core::Package p = registry.findPackage(keyValue.first);
            suffer::core::Builder builder = suffer::core::Builder(p, this->registry);

            builder.import(index, false);
        } else {
            std::cout << suffer::utils::io::info() << " System Dependency " << suffer::utils::io::dataString(keyValue.first) << "\n";
            sysLibs.push_back(keyValue.first);
        }
    }

    if (!this->package.isHeaderOnly()) {
        const std::filesystem::path buildPath = std::filesystem::current_path() / "lib";
        const std::string libName = "lib" + this->package.getName() + ".a";

        if (!std::filesystem::exists(buildPath)) {
            std::cout << suffer::utils::io::info() << " No project /lib directory detected\n";

            if (!std::filesystem::create_directory(buildPath)) {
                std::cerr << suffer::utils::io::error() << " Failed to create " << suffer::utils::io::dataString(buildPath) << "\n";
                exit(EXIT_FAILURE);
            }

            std::cout << suffer::utils::io::okay() << " Created " << suffer::utils::io::dataString(buildPath.string()) << "\n";
        }

        std::cout << suffer::utils::io::info() << " Checking cache for " << suffer::utils::io::dataString(this->package.getName()) << "\n";

        if (this->isCached()) {
            std::filesystem::copy(this->package.determineCachePath(), buildPath, std::filesystem::copy_options::overwrite_existing);
        } else {
            this->compileLib();
            
            std::vector<std::filesystem::path> builtHeaders = this->detectBuiltHeaders();

            if (builtHeaders.size() > 0) {
                std::filesystem::path destiny = libPath / "include";
                
                for (auto& element : std::filesystem::directory_iterator(destiny)) {
                    if (element.is_directory()) {
                        destiny = element.path();
                        break;
                    }
                }

                for (auto& header : builtHeaders) {
                    std::filesystem::copy(header, destiny, std::filesystem::copy_options::overwrite_existing);
                    std::filesystem::copy(header, include / this->package.getName());
                }

                std::filesystem::copy(this->package.determineCachePath(), buildPath, std::filesystem::copy_options::overwrite_existing);
            }
            
        }
    }

    this->createProjectJson(index, sysLibs);

    if (root) {
        this->createMakeFile();
    }

    if (!std::filesystem::exists(src)) {
        std::filesystem::create_directory(src);
        std::ofstream mainCpp(src / "main.cpp");

        mainCpp << "";
        mainCpp.close();
    }

    if (!std::filesystem::exists(out)) {
        std::filesystem::create_directories(out);
    }

    std::cout << suffer::utils::io::okay() << " Successfully imported " << suffer::utils::io::dataString(this->package.getName()) << "\n";
}

suffer::core::Builder::Builder(suffer::core::Package& package, suffer::core::RegistryHandler& registry): package(package), registry(registry) {}