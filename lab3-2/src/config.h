#define ERROR_CODE -1
#define SUCCESS_CODE 0

#include "operators.h"

struct Operator{
    int (*operator) (int, char* []);
    const char* nameOfOperation;
    const char* description1;
    const char* description2;
};

static const struct Operator ALL_OPERATORS[] = {
        {makeDirectory,"makeDirectory", "makeDirectory [PATH]", "makes directory by PATH"},
        {printDirectoryContent, "printDirectoryContent", "printDirectoryContent [FILE_PATH]", "prints content of directory by FILE_PATH"},
        {removeDirectory,"removeDirectory","removeDirectory [PATH]", "removes all directory content by PATH and remove this directory"},
        {createFile, "createFile", "createFile [PATH]", "creates file by PATH"},
        {printFileContent,"printFileContent","printFileContent [FILE_PATH]", "prints all content of file by FILE_PATH"},
        {removeFile,"removeFile","removeFile [FILE_PATH]", "removes file by FILE_PATH"},
        {createSymbolLink, "createSymbolLink", "createSymbolLink [TARGET_PATH] [LINK_NAME]", "makes symbol link between TARGET_PATH and LINK_NAME"},
        {printSymbolLinkContent,"printSymbolLinkContent","printSymbolLinkContent [LINK_NAME]", "prints content of symbol link [LINK_NAME]"},
        {printSymbolLinkSourceContent,"printSymbolLinkSourceContent", "printSymbolLinkSourceContent [LINK_NAME]", "prints content of origin file of symbol link [LINK_NAME]"},
        {removeSymbolLink, "removeSymbolLink", "removeSymbolLink [LINK_NAME]", "unlink symbol link [LINK_NAME]"},
        {createHardLink, "createHardLink", "createHardLink [TARGET_PATH] [LINK_NAME]", "makes hard link between TARGET_PATH and LINK_NAME"},
        {removeHardLink, "removeHardLink", "removeHardLink [LINK_NAME]", "unlink hard link [LINK_NAME]"},
        {printPermissionsAndCountOfHardLinks, "printPermissionsAndCountOfHardLinks", "printPermissionsAndCountOfHardLinks [FILE_PATH]", "prints rights and count of hard links of file by FILE_PATH"},
        {setRights,"setRights","setRights [FILE_PATH] [RIGHTS]", "set rights RIGHTS on file FILE_PATH format of rights is ---------"}
};

static const unsigned int COUNT_OF_OPERATORS = sizeof(ALL_OPERATORS) / sizeof(ALL_OPERATORS[0]);
