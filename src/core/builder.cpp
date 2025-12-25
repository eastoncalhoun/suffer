#include <suffer/core/builder.hpp>

void suffer::core::Builder::checkPermissions(const std::filesystem::path& path) {
    const std::filesystem::perms perms = std::filesystem::status(path).permissions();

    if ((perms & std::filesystem::perms::owner_read) == std::filesystem::perms::none && (perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none) {
        std::cerr << suffer::utils::io::error() << " No read/write permissions in " << suffer::utils::io::dataString(path.string()) << "\n";
        exit(EXIT_FAILURE);
    }
}

bool suffer::core::Builder::fileIsHeader(const std::filesystem::path& path) {
    bool isHeader = false;

    if (std::filesystem::is_directory(path) || !path.has_extension()) {
        return isHeader;
    } 

    std::string extension = path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension == ".h" || extension == ".hh" || extension == ".hpp" || extension == ".hxx" || extension == ".h++") {
        isHeader = true;
    }

    return isHeader;
}

int suffer::core::Builder::determineHeaderPackaging() {
    int type = UNKNOWN;
    const std::filesystem::path pPath = this->package.determinePath();
    const std::string pName = this->package.getName();
    const size_t loc = pName.find("lib");
    std::string possible = pName;

    if (loc != std::string::npos) {
        possible.replace(loc, 3, "");
    }

    if (!std::filesystem::exists(pPath)) {
        std::cerr << suffer::utils::io::error() << " There is nothing at " << suffer::utils::io::dataString(pPath.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    if (std::filesystem::exists(pPath / "single_include")) {
        type = SI_H_STYLE;
    } else if (std::filesystem::exists(pPath / "include")) {
        type = IH_H_STYLE;
    } else if (this->getHeadersFromRoot().size() > 0) {
        type = RT_H_STYLE;
    } else if (std::filesystem::exists(pPath / pName)) {
        type = NM_H_STYLE;
    } else if (std::filesystem::exists(pPath / "src" / pName / "include")) {
        if (!std::filesystem::exists(pPath / "src" / pName / "include" / pName) && !std::filesystem::exists(pPath / "src" / pName / "include" / possible)) {
            type = SN_IH_STYLE;
        } else {
            int count = 0;
            type = SN_IN_STYLE;
            
            for (const auto& element: std::filesystem::directory_iterator(pPath / "src" / this->package.getName() / "include")) {
                if (this->fileIsHeader(element.path())) {
                    if (count == 1) {
                        type = SN_IH_STYLE;
                        break;
                    } else {
                        count++;
                    }
                }
            }
        }
    }

    return type;
}

std::filesystem::path suffer::core::Builder::determineUnknownHeaderRoot(const std::filesystem::path& libPath) {
    bool rootPath = false;
    std::vector<std::filesystem::path> dirs = {};

    if (!std::filesystem::exists(libPath)) {
        std::cerr << suffer::utils::io::error() << " The path " << suffer::utils::io::dataString(libPath.string()) << " does not exist\n";
        exit(EXIT_FAILURE); 
    }

    for (const auto& element : std::filesystem::directory_iterator(libPath)) {          
        if (this->fileIsHeader(element.path())) {
            rootPath = true;
            break;
        }

        if (element.is_directory()) {
            dirs.push_back(element.path());
        }
    }

    if (!rootPath) {
        for (const auto& dir : dirs) {
            return this->determineUnknownHeaderRoot(dir);
        }
    } else {
        return libPath;
    }

    std::cerr << suffer::utils::io::error() << " Failed to find the root\n";
    exit(EXIT_FAILURE);
}

std::vector<std::filesystem::path> suffer::core::Builder::getHeadersFromRoot() {
    std::vector<std::filesystem::path> headers = std::vector<std::filesystem::path>();

    for (auto& e : std::filesystem::directory_iterator(this->package.determinePath())) {
        if (this->fileIsHeader(e.path())) {
            headers.push_back(e.path());
        }
    }

    return headers;
}

void suffer::core::Builder::importUnknownHeaders(const std::filesystem::path& include, const std::filesystem::path& libPath) {
    bool foundHeaders = false; // may not be touched

    if (!std::filesystem::exists(include)) {
        std::filesystem::create_directory(include);
    }

    if (!std::filesystem::exists(libPath) || !std::filesystem::is_directory(libPath)) {
        std::cerr << suffer::utils::io::error() << " No directory found at " << suffer::utils::io::dataString(libPath.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    int headers = 0;

    for (const auto& element : std::filesystem::directory_iterator(libPath)) {
        if (element.is_directory()) {
            this->importUnknownHeaders(include / element.path().filename().string(), element.path());
        }

        if (this->fileIsHeader(element.path())) {
            std::filesystem::copy(element.path(), include, std::filesystem::copy_options::overwrite_existing);
            
            headers++;
            
            if (!foundHeaders) {
                foundHeaders = true;
            }
        }
    }

    if (!foundHeaders) {
        std::cout << suffer::utils::io::info() << " No headers found in " << suffer::utils::io::dataString(libPath.string()) << "\n";
    } else {
        std::cout << suffer::utils::io::info() << " Copied " << headers << " headers from " << suffer::utils::io::dataString(libPath.string()) << "\n";
    }
}

void suffer::core::Builder::importHeaders(const std::filesystem::path& include, const std::filesystem::path& libPath) {
    const std::filesystem::path includeProject = include / this->package.getName(); 
    const std::filesystem::path sInclude = libPath / "single_include"; //only used if SI_H_HEADER_STYLE
    const int packaging = this->determineHeaderPackaging();

    auto headers = std::vector<std::filesystem::path>();
    bool prevAcknowledged = false;

    std::vector<std::filesystem::path> libIncludePaths = std::vector<std::filesystem::path>();
    
    switch (packaging) {
        case SI_H_STYLE:
            for (auto& element : std::filesystem::directory_iterator(sInclude)) {
                std::filesystem::copy(element.path(), include / element.path().filename(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
            }

            break;

        case IH_H_STYLE:
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

            if (!std::filesystem::exists(includeProject / this->package.getName())) {
                std::filesystem::create_directory(includeProject / this->package.getName());
            }

            for (auto& header : headers) {
                std::filesystem::copy(header, includeProject, std::filesystem::copy_options::overwrite_existing);
            }

            break;
        case NM_H_STYLE:
            std::filesystem::copy(libPath / this->package.getName(), include, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
            break;

        case SN_IH_STYLE:
            std::filesystem::copy(libPath / "src" / this->package.getName() / "include", include, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
            break;

        case SN_IN_STYLE:
            std::filesystem::copy(libPath / "src" / this->package.getName() / "include", include, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
            break;

        case UNKNOWN:
            std::cout << suffer::utils::io::warning() << " Unknown header structure for " << suffer::utils::io::dataString(this->package.getName()) << "\n";
            std::cout << suffer::utils::io::info() << " Copying headers in src recursively\n";
            this->importUnknownHeaders(include, libPath / "src");

            break;

        default:
            std::cerr << suffer::utils::io::error() << " Somehow the structure to " << suffer::utils::io::dataString(this->package.getName()) << " is so weird, the type isn't even UNKNOWN\n";
            exit(EXIT_FAILURE);

            break;
    }

    //cleanup
    for (const auto& element : std::filesystem::directory_iterator(include)) {
        std::filesystem::path ePath = element.path();

        if (!this->fileIsHeader(ePath) && !std::filesystem::is_directory(ePath) && ePath.has_extension()) {
            std::filesystem::remove(ePath);
        }
    }

    std::cout << suffer::utils::io::okay() << " Copied over headers from " << suffer::utils::io::dataString(this->package.getName()) << "\n";
}

std::vector<std::filesystem::path> suffer::core::Builder::findStaticLibLocation() {
    const std::filesystem::path libPath = this->package.determinePath();
    std::vector<std::filesystem::path> archives = {};

    for (auto& entry : std::filesystem::recursive_directory_iterator(libPath)) {        
        if (entry.path().extension() == ".a") {
            archives.push_back(entry.path());
        }
    }

    if (archives.size() == 0) {
        std::cerr << suffer::utils::io::error() << " No compiled .a found in " << suffer::utils::io::dataString(libPath.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    return archives;    
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
                if (this->fileIsHeader(e.path())) {
                    headers.push_back(e.path());
                }
            }
        }
    }

    return headers;
}

void suffer::core::Builder::placeBuiltHeaders(const std::vector<std::filesystem::path>& builtHeaders, const std::filesystem::path& include) {
    const std::filesystem::path libPath = this->package.determinePath();
    const std::string pName = this->package.getName();
    const int PACKAGING = this->determineHeaderPackaging();
    std::filesystem::path destiny = libPath / "include";
    std::string buffer; // unused unless SN_IN_STYLE header

    switch (PACKAGING) {
        case UNKNOWN:
            destiny = this->determineUnknownHeaderRoot(libPath / "src");
            break;

        case IH_H_STYLE:
            for (const auto& element : std::filesystem::directory_iterator(destiny)) {
                if (element.is_directory()) {
                    destiny = element.path();
                    break;
                }
            }

            break;

        case SN_IH_STYLE:
            destiny = libPath / "src" / pName / "include";
            break;
        
        case SN_IN_STYLE:
            destiny = libPath / "src" / pName / "include" / pName;

            if (!std::filesystem::exists(destiny)) {
                buffer = pName;
                buffer.replace(buffer.find("lib"), 3, "");

                destiny = libPath / "src" / pName / "include" / buffer;
            }

            break;
    }

    for (const auto& header : builtHeaders) {
        if (std::filesystem::exists(destiny)) {
            std::filesystem::copy(header, destiny, std::filesystem::copy_options::overwrite_existing);
        }
        
        if (this->determineHeaderPackaging() != UNKNOWN) {
            std::filesystem::copy(header, include / pName, std::filesystem::copy_options::overwrite_existing);
        } else {
            std::filesystem::copy(header, include / destiny.filename().string(), std::filesystem::copy_options::overwrite_existing);
        }
    }
}

void suffer::core::Builder::compileLib() {
    const std::filesystem::path libPath = this->package.determinePath();
    const std::filesystem::path include = libPath / "include";
    const std::string& flags = this->package.getFlags();
    const std::string& name = this->package.getName(); 

    if (!std::filesystem::exists(libPath)) {
        std::cerr << suffer::utils::io::error() << " Library not found at " << suffer::utils::io::dataString(libPath) << "\n";
        exit(EXIT_FAILURE);
    }

    if (std::filesystem::exists(libPath / "CMakeLists.txt")) {
        std::cout << suffer::utils::io::info() << " Detected CMake project... Attempting to compile\n";

        const std::string cmakeGen = "cd " + libPath.string()  + " && cmake -S . -B build -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release " + flags;
        const std::string cmakeBuild = "cd " + libPath.string() + " && cmake --build build --config Release";
        
        if (system(cmakeGen.c_str()) != 0) {
            std::cout << suffer::utils::io::error() << " Failed to generate the static CMake build for " << suffer::utils::io::dataString(this->package.getName()) << "\n";
            exit(EXIT_FAILURE);
        }

        if (system(cmakeBuild.c_str()) != 0) {
            std::cout << suffer::utils::io::error() << " CMake failed to compile " << suffer::utils::io::dataString(this->package.getName()) << "\n";
            exit(EXIT_FAILURE);
        }
    } else {
        const std::filesystem::path sufferProj = libPath / "suffer.project.json";

        if (std::filesystem::exists(sufferProj)) {
            std::cout << suffer::utils::io::info() << " Detected Suffer project... Attempting to compile\n";
        } else {
            std::cout << suffer::utils::io::info() << " Unknown project type... Attempting to compile... Results may vary\n";
        }

        //compile
        if (system(this->determineProjectGpp(libPath).c_str()) != 0) {
            std::cerr << suffer::utils::io::error() << " Failed to compile " << suffer::utils::io::dataString(name) << "\n";
            std::cout << suffer::utils::io::info() << " Attempting header include bruteforce\n";
            
            if (system(this->determineProjectGpp(libPath, true).c_str()) !=  0) {
                std::cerr << suffer::utils::io::error() << " Failed to compile " << suffer::utils::io::dataString(name) << "\n";
                exit(EXIT_FAILURE);
            }
        }

        const std::string arCommand = "cd " + libPath.string() + " && ar rcs lib" + name + ".a *.o";
        
        //bundle
        if (system(arCommand.c_str()) != 0) {
            std::cout << suffer::utils::io::error() << " Failed to bundle object files for " << suffer::utils::io::dataString(name) << "\n";
            exit(EXIT_FAILURE);
        }

        //cleanup
        for (auto& entry : std::filesystem::directory_iterator(libPath)) {
            if (entry.path().extension().string() == ".o") {
                std::filesystem::remove(entry.path());
            }
        }
    }

    const std::vector<std::filesystem::path> staticLibs = this->findStaticLibLocation();
    const std::filesystem::path cachePath = this->package.determineCachePath();

    if (!std::filesystem::exists(cachePath)) {
        std::filesystem::create_directory(cachePath);
    } else if (!std::filesystem::is_directory(cachePath)) {
        std::filesystem::remove(cachePath);
        std::filesystem::create_directory(cachePath);
    }

    for (const auto& staticLib : this->findStaticLibLocation()) {
        std::filesystem::copy(staticLib, cachePath, std::filesystem::copy_options::overwrite_existing);
    }

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
    
    if (!std::filesystem::exists(filePath)) {
        return 0;
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

void suffer::core::Builder::createProjectJson(const int index, const std::vector<std::string>& sysLibs) {
    const std::filesystem::path jsonPath = std::filesystem::current_path() / "suffer.project.json";
    suffer::utils::Config config {  };
    nlohmann::json projectObj;

    if (std::filesystem::exists(jsonPath)) {
        std::ifstream currProjectFile { jsonPath };
        const std::string jString = std::string(std::istreambuf_iterator<char>(currProjectFile), std::istreambuf_iterator<char>());

        currProjectFile.close();

        projectObj = nlohmann::json::parse(jString);
    } else {
        projectObj = {  };

        projectObj["projectName"] = "program";
        projectObj["compiler"] = config.getCompilerCpp();
        projectObj["std"] = config.getStdCpp();
        projectObj["flags"] = config.getCompilerFlags();
    }

    if (projectObj["link"].size() <= index) {
        projectObj["link"].push_back(nlohmann::json::array());
    }

    if (std::filesystem::exists(this->package.determineCachePath())) {
        for (const auto& ar : std::filesystem::directory_iterator(this->package.determineCachePath())) {
            std::string arName = ar.path().filename().string();

            arName.replace(0, 3, "");
            arName = arName.substr(0, arName.find(".a"));
            projectObj["link"][index].push_back("-l" + arName);
        }
    }

    for (auto& lib : sysLibs) {
        projectObj["link"][index].push_back("-l" + lib);
    }

    if (projectObj["link"][index].size() == 0) {
        return;
    }

    std::ofstream projectFileOut { jsonPath };
    
    projectFileOut << projectObj.dump(4);
    projectFileOut <<  "\n";
    projectFileOut.close();
}

std::string suffer::core::Builder::determineLinkOrder(const std::filesystem::path& projectPath = std::filesystem::current_path() / "suffer.project.json") {
    std::vector<std::string> links = std::vector<std::string>();
    std::ifstream projectFile(projectPath);
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

std::vector<std::filesystem::path>& suffer::core::Builder::subdirsWithHeaders(const std::filesystem::path& rootToSearch, std::vector<std::filesystem::path>& found) {
    if (!std::filesystem::is_directory(rootToSearch)) {
        std::cerr << suffer::utils::io::error << " The path " << suffer::utils::io::dataString(rootToSearch.string()) << " is not a directory\n";
        exit(EXIT_FAILURE);
    }

    std::vector<std::filesystem::path> subdirs {};
    bool checkNeeded = true;

    for (const auto& element : std::filesystem::directory_iterator(rootToSearch)) {
        if (element.is_directory()) {
            subdirs.push_back(element.path());
        } else {
            if (this->fileIsHeader(element.path()) && checkNeeded) {
                checkNeeded = false;

                found.push_back(rootToSearch);
            }
        }
    }

    for (const auto& subdir : subdirs) {
        this->subdirsWithHeaders(subdir, found);
    }

    return found;
} 

std::string suffer::core::Builder::determineProjectGpp(const std::filesystem::path& projectPath = std::filesystem::current_path(), const bool mashing) {
    const std::filesystem::path pCurr = std::filesystem::current_path();
    std::string gppCommand;

    if (projectPath == pCurr) {
        //building project makefile g++
        gppCommand = "g++ -o ./out/program -I ./include $(shell find ./src -name \"*.cpp\" -o -name \"*.cc\" -o -name \"*.cxx\" -o -name \"*.c++\" -o -name \"*.C\") ";

        if (std::filesystem::exists(projectPath / "lib")) {
            gppCommand = gppCommand + "-L ./lib " + this->determineLinkOrder();
        }
    } else {
        //building a dependency
        const int packaging = this->determineHeaderPackaging();
        gppCommand = "cd " + projectPath.string() + " && g++ -c ";

        if (mashing) {
            std::vector<std::filesystem::path> withHeaders {};

            std::cout << suffer::utils::io::warning() << " Attempting to include every subdir with headers for compilation\n";
            this->subdirsWithHeaders(projectPath, withHeaders);

            for (const auto& subdir : withHeaders) {
                gppCommand = gppCommand + "-I " + subdir.string() + " ";
            }
        } else {
            switch (packaging) {
                case IH_H_STYLE: gppCommand = gppCommand + "-I ./include/ "; break;
                case SI_H_STYLE: gppCommand = gppCommand + "-I ./single_include/ "; break;
                case SN_IH_STYLE: gppCommand = gppCommand + "-I ./src/ " + this->package.getName() + "/include/"; break;
                case SN_IN_STYLE: gppCommand = gppCommand + "-I ./src/ " + this->package.getName() + "/include/" + this->package.getName(); break;
                default: 
                    gppCommand = gppCommand + "-I ./ ";
                    std::cout << suffer::utils::io::warning() << " Unhandled header type.. Include path defaulting to ./\n";
                    break;
            }
        }

        gppCommand = gppCommand + "$(find ";

        if (std::filesystem::exists(projectPath / "src" / this->package.getName())) {
            gppCommand = gppCommand + "./src/" + this->package.getName();
        } else if (std::filesystem::exists(projectPath / "src")) {
            gppCommand = gppCommand + "./src/";
        } else {
            std::cout << suffer::utils::io::warning() << " No currently predictable structure found. Attempting brute force compilation\n";
            gppCommand = gppCommand + "./";
        }

        gppCommand = gppCommand + " -name \"*.cpp\" -o -name \"*.c\" -o -name \"*.cc\" -o -name \"*.cxx\" -o -name \"*.c++\" -o -name \"*.C\")";
    }

    return gppCommand;
}

void suffer::core::Builder::createMakeFile() {
    const std::filesystem::path curr = std::filesystem::current_path();
    const std::filesystem::path pProjConfig = curr / "suffer.project.json"; 

    if (!std::filesystem::exists(pProjConfig)) {
        std::cerr << suffer::utils::io::error() << " No ./suffer.project.json was found\n";
        exit(EXIT_FAILURE);
    }

    std::ifstream fProjConfig { pProjConfig };
    const std::string sProjConfig { std::istreambuf_iterator<char>(fProjConfig), std::istreambuf_iterator<char>() };
    fProjConfig.close();

    nlohmann::json projConfig;

    try {
        projConfig = nlohmann::json::parse(sProjConfig);
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Invalid json in ./suffer.project.json (Builder::createMakeFile)\n";
        exit(EXIT_FAILURE);
    }

    std::string name, compiler, standard, flags;
    nlohmann::json dependencies;

    try {
        name = projConfig["projectName"].get<std::string>();
        compiler = projConfig["compiler"].get<std::string>();
        standard = projConfig["std"].get<std::string>();
        flags = projConfig["flags"].get<std::string>();
        name = projConfig["projectName"].get<std::string>();
    } catch (std::exception& e) {
        std::cerr << suffer::utils::io::error() << " Invalid data in ./suffer.project.json\n";
        exit(EXIT_FAILURE);
    }

    const std::filesystem::path defMakePath = this->registry.getRegistryPath() / "scripts" / "Makefile";
    std::ifstream defMake { defMakePath };

    if (!defMake) {
        std::cerr << suffer::utils::io::error() << " Failed to open " << suffer::utils::io::dataString(defMakePath.string()) << "\n";
        exit(EXIT_FAILURE);
    }

    std::string makeText = std::string(std::istreambuf_iterator<char>(defMake), std::istreambuf_iterator<char>());
    defMake.close();

    makeText.replace(makeText.find("^c"), 2, compiler);
    makeText.replace(makeText.find("^s"), 2, standard);
    makeText.replace(makeText.find("^f"), 2, flags);
    makeText.replace(makeText.find("^l"), 2, this->determineLinkOrder());

    while (makeText.find("^n") != std::string::npos) {
        makeText.replace(makeText.find("^n"), 2, name);
    }

    std::ofstream makeFile(curr / "Makefile", std::ios::trunc);

    if (!makeFile) {
        std::cerr << suffer::utils::io::error() << " Failed to open the new Makefile\n";
        exit(EXIT_FAILURE);
    }

    makeFile << makeText;
    makeFile.close();
}

void suffer::core::Builder::import(const int index, const bool root) {
    if (root) {
        std::cout << suffer::utils::io::info() << " Attempting to import " << suffer::utils::io::dataString(this->package.getName()) << " to " << suffer::utils::io::dataString(std::filesystem::current_path().string()) << "\n";
    } else {
        std::cout << suffer::utils::io::info() << " Attempting to import dependency " << suffer::utils::io::dataString(this->package.getName()) << " to " << suffer::utils::io::dataString(std::filesystem::current_path().string()) << "\n";
    }
    
    const std::filesystem::path libPath = this->package.determinePath();
    const std::filesystem::path curr = std::filesystem::current_path();
    const std::filesystem::path include = curr / "include";

    this->checkPermissions(libPath);
    this->checkPermissions(include);
    this->setupProject();
    this->importHeaders(include, libPath);

    if (!std::filesystem::exists(libPath)) {
        std::cerr << suffer::utils::io::error() << " The path " << suffer::utils::io::dataString(libPath.string()) << " does not exist\n";
        exit(EXIT_FAILURE);
    }

    const std::unordered_map<std::string, std::string>& dependencies = this->package.getDependencies();
    const std::filesystem::path buildPath = std::filesystem::current_path() / "lib";
    std::vector<std::string> sysLibs = {};
    std::vector<suffer::core::Package> packages = {};
    
    if (!std::filesystem::exists(buildPath)) {
        if (!std::filesystem::create_directory(buildPath)) {
            std::cerr << suffer::utils::io::error() << " Failed to create " << suffer::utils::io::dataString(buildPath) << "\n";
            exit(EXIT_FAILURE);
        }

        std::cout << suffer::utils::io::okay() << " Created " << suffer::utils::io::dataString(buildPath.string()) << "\n";
    }
    
    for (const auto& keyValue : dependencies) {
        if (keyValue.second != "sys") {
            suffer::core::Package p = registry.findPackage(keyValue.first);
            
            packages.push_back(p);
        } else {
            std::string depName = keyValue.first;
            std::transform(depName.begin(), depName.end(), depName.begin(), ::tolower);

            const std::string pkgConfigCommand = "pkg-config --exists ";
            const std::string firstTry = pkgConfigCommand + depName;
            const std::string secondTry = pkgConfigCommand + "lib" + depName;
            const std::string thirdTry = pkgConfigCommand + depName + "lib";
            const std::string dataName = suffer::utils::io::dataString(depName);
            const bool glibc = depName == "pthread" || depName == "m" || depName == "dl" || depName == "rt" || depName == "crypt" || depName == "resolv" || depName == "nsl" || depName == "util";
            

            if (glibc) {
                std::cout << suffer::utils::io::okay() << " Detected libC dependency " << dataName << "\n";
            } else if (system(firstTry.c_str()) != 0 && system(secondTry.c_str()) != 0 && system(thirdTry.c_str()) != 0) {
                std::cout << suffer::utils::io::warning() << " Could not detect system dependency " << dataName << "\n";
                std::cout << suffer::utils::io::info() << " You should be able to install " << dataName << " from your system's package manager. If it is not installed you will be unable to link against it.\n";
            }  else {
                std::cout << suffer::utils::io::okay() << " Detected dependency " << dataName << " on the system\n";
            }

            sysLibs.push_back(keyValue.first);
        }
    }

    if (!this->package.isHeaderOnly()) {
        const std::string libName = "lib" + this->package.getName() + ".a";

        if (this->isCached()) {
            std::cout << suffer::utils::io::info() << " Cached version " << suffer::utils::io::dataString(this->package.determineCachePath().string()) << " using...\n"; 

            for (const auto& archive : std::filesystem::directory_iterator(this->package.determineCachePath())) {
                std::filesystem::copy(archive.path(), buildPath, std::filesystem::copy_options::overwrite_existing);
            }
        } else {
            this->compileLib();

            for (const auto& archive : std::filesystem::directory_iterator(this->package.determineCachePath())) {
                std::filesystem::copy(archive.path(), buildPath, std::filesystem::copy_options::overwrite_existing);
            }

            std::vector<std::filesystem::path> builtHeaders = this->detectBuiltHeaders();

            if (builtHeaders.size() > 0) {
                this->placeBuiltHeaders(builtHeaders, include);
            }
        }
    }

    this->createProjectJson(index, sysLibs);

    for (auto& dep : packages) {
        suffer::core::Builder builder(dep, this->registry);
        builder.import(index, false);
    }

    if (root) {
        this->createMakeFile();
        std::cout << suffer::utils::io::okay() << " Successfully imported " << suffer::utils::io::dataString(this->package.getName()) << "\n";
    } else {
        std::cout << suffer::utils::io::okay() << " Successfully imported dependency " << suffer::utils::io::dataString(this->package.getName()) << "\n";
    }   
}

void suffer::core::Builder::setupProject() {
    const std::filesystem::path curr = std::filesystem::current_path();
    const std::filesystem::path include = curr / "include";
    const std::filesystem::path src = curr / "src";
    const std::filesystem::path out = curr / "out";
    const std::filesystem::path lib = curr / "lib";
    const std::filesystem::path pConfig = curr / "suffer.project.json";

    if (!std::filesystem::exists(include)) {
        if (std::filesystem::create_directory(include)) {
            std::cout << suffer::utils::io::okay() << " Created " << suffer::utils::io::dataString(include.string()) << "\n";
        } else {
            std::cout << suffer::utils::io::error() << " Failed to create " << suffer::utils::io::dataString(include.string()) << "\n";
            exit(EXIT_FAILURE);
        }
    }

    if (!std::filesystem::exists(src)) {
        if (std::filesystem::create_directory(src)) {
            std::cout << suffer::utils::io::okay() << " Created " << suffer::utils::io::dataString(src.string()) << "\n";
        } else {
            std::cout << suffer::utils::io::error() << " Failed to create " << suffer::utils::io::dataString(src.string()) << "\n";
            exit(EXIT_FAILURE);
        }

        std::ofstream mainCpp(src / "main.cpp");

        mainCpp << "#include <iostream>\n\nint main(int argc, char** argv) {\n\tstd::cout << \"Hello from suffer\" << std::endl;\n\treturn 0;\n}\n";
        mainCpp.close();
    }

    if (!std::filesystem::exists(out)) {
        std::filesystem::create_directory(out);
    }

    if (!std::filesystem::exists(lib)) {
        std::filesystem::create_directory(lib);
    }

    if (!std::filesystem::exists(pConfig)) {
        this->createProjectJson(0, {});
    }

    this->createMakeFile();
}

suffer::core::Builder::Builder(suffer::core::Package& package, suffer::core::RegistryHandler& registry): package(package), registry(registry) {}
