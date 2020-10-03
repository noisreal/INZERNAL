#include <stdio.h>
#include "pe_lib/pe_bliss.h"
#include <fstream>
#include <Windows.h>

//all patcher code made by ness


void PatchImports(pe_bliss::pe_base& image) {
    pe_bliss::section imp_section;
    imp_section.set_name(".imports");
    imp_section.readable(true).writeable(true);

    // Copy original imports
    char* orig_imp = image.section_data_from_rva(image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_IMPORT));
    imp_section.get_raw_data().assign(orig_imp, (size_t)image.get_directory_size(IMAGE_DIRECTORY_ENTRY_IMPORT));

    PIMAGE_IMPORT_DESCRIPTOR imports = (PIMAGE_IMPORT_DESCRIPTOR)imp_section.get_raw_data().data();
    size_t num_imports = imp_section.get_raw_data().size() / sizeof(IMAGE_IMPORT_DESCRIPTOR);
    imp_section.get_raw_data().resize((num_imports + 1) * sizeof(IMAGE_IMPORT_DESCRIPTOR)); // add null terminating descriptor

    size_t imports_size = imp_section.get_raw_data().size();
    imp_section.get_raw_data() += "INZERNAL.dll";
    imp_section.get_raw_data() += '\0'; // null terminate string
    size_t first_thunk = imp_section.get_raw_data().size();
    imp_section.get_raw_data().resize(first_thunk + 8); // allocate 2 iat/thunkref/whatever entries (second one is null terminator)

    auto& section = image.add_section(imp_section);

    imports = (PIMAGE_IMPORT_DESCRIPTOR)section.get_raw_data().data();
    imports[num_imports - 1].Name = section.get_virtual_address() + imports_size;
    imports[num_imports - 1].OriginalFirstThunk = section.get_virtual_address() + first_thunk;
    imports[num_imports - 1].FirstThunk = section.get_virtual_address() + first_thunk;

    *(uint32_t*)(section.get_raw_data().data() + first_thunk) = section.get_virtual_address() + first_thunk - 6;

    image.set_directory_rva(IMAGE_DIRECTORY_ENTRY_IMPORT, section.get_virtual_address());
    image.set_directory_size(IMAGE_DIRECTORY_ENTRY_IMPORT, imp_section.get_raw_data().size());
}

int main(int argc, char** argv) {
    if (argc < 1) {
        printf("Usage: patcher.exe file");
        return 0;
    }
    std::string fname = argv[1];
    std::ifstream pe_file(fname, std::ios::in | std::ios::binary);

    pe_bliss::pe_base image(pe_bliss::pe_factory::create_pe(pe_file));

    if (image.get_pe_type() != pe_bliss::pe_type::pe_type_64) {
        printf("Only PE64 is supported\n");
        return 0;
    }

    PatchImports(image);

    std::ofstream new_pe_file(fname, std::ios::out | std::ios::binary);
    rebuild_pe(image, new_pe_file);
}