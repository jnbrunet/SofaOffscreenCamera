

extern "C" {
void        initExternalModule();
const char* getModuleName();
const char* getModuleVersion();
const char* getModuleLicense();
const char* getModuleDescription();
const char* getModuleComponentList();
}

void initExternalModule() {}

const char* getModuleName() {
    return "SofaOffscreenCamera";
}

const char* getModuleVersion() {
    return "20.12";
}

const char* getModuleLicense() {
    return "LGPL";
}

const char* getModuleDescription() {
    return "Offscreen rendering camera for SOFA Framework.";
}

const char* getModuleComponentList() {
    /// string containing the names of the classes provided by the plugin
    return "OffscreenCamera";
}