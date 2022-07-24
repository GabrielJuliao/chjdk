#include <Windows.h>
#include <cstdio>
#include <tlhelp32.h>
#include <Psapi.h>
#include <string>

DWORD get_ppid() {
    DWORD pid = GetCurrentProcessId();
    HANDLE h;
    PROCESSENTRY32 pe = {0};
    DWORD ppid = 0;
    pe.dwSize = sizeof(PROCESSENTRY32);
    h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(h, &pe)) {
        do {
            if (pe.th32ProcessID == pid) {
                ppid = pe.th32ParentProcessID;
                break;
            }
        } while (Process32Next(h, &pe));
    }
    CloseHandle(h);
    return (ppid);
}

std::string get_proc_name(DWORD pid) {
    HANDLE h;
    char proc_name[MAX_PATH] = {0};
    h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (h) {
        if (GetModuleFileNameEx(h, nullptr, reinterpret_cast<LPSTR>(proc_name), MAX_PATH) == 0)
            GetLastError();
        CloseHandle(h);
    }

    std::string full_path_str;

    for (int i = 0; i < strlen(proc_name); i++) {
        full_path_str.push_back(proc_name[i]);
    }
    std::string base_filename = full_path_str.substr(full_path_str.find_last_of('\\') + 1);

    return base_filename;
}

int main(int argc, char *argv[]) {
    std::string pp_name = get_proc_name(get_ppid());
    printf("%s", pp_name.c_str());
}