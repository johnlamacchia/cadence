// Cadence installer: puts the loader, the example mods and the address data into a
// Geometry Dash folder, and takes them out again.
//
//   CadenceInstaller.exe                          the window (finds Steam's copy of GD itself)
//   CadenceInstaller.exe --silent                 install without a window
//   CadenceInstaller.exe --silent --uninstall     remove it again
//   options: --gd-dir <folder>  --no-mods  --allow-unknown-version  --gui-smoke
//
// Everything it installs is carried inside this exe (see pack.cpp). What it wrote is
// listed in <GD>/cadence_install.txt, so uninstall removes exactly that, and a
// winmm.dll that was already there (another mod loader) is renamed, never overwritten.
// Messages also go to %TEMP%\cadence_installer.log.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <tlhelp32.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' " \
                        "processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef CADENCE_VERSION
#define CADENCE_VERSION "dev"
#endif

namespace fs = std::filesystem;

namespace {

constexpr wchar_t kManifestName[] = L"cadence_install.txt";
constexpr wchar_t kBackupName[] = L"winmm.dll.pre-cadence";

// PE timestamps of the GeometryDash.exe builds this Cadence was generated for. 2.2091 is 0x6AD2D987.
constexpr uint32_t kKnownTimestamps[] = {0x6AD2D987u};

// ---- Small helpers ---------------------------------------------------------------------

std::wstring widen(const std::string& s) {
    if (s.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), nullptr, 0);
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), w.data(), n);
    return w;
}

std::string narrow(const std::wstring& w) {
    if (w.empty()) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), nullptr, 0, nullptr, nullptr);
    std::string s(static_cast<size_t>(n), '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), s.data(), n, nullptr, nullptr);
    return s;
}

std::wstring system_message(DWORD error) {
    wchar_t* buffer = nullptr;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
                   error, 0, reinterpret_cast<wchar_t*>(&buffer), 0, nullptr);
    std::wstring text = buffer ? buffer : L"unknown error";
    if (buffer) LocalFree(buffer);
    while (!text.empty() && (text.back() == L'\n' || text.back() == L'\r' || text.back() == L' ')) text.pop_back();
    return text;
}

bool g_console = false;

void log_line(const std::wstring& text) {
    static const fs::path path = [] {
        wchar_t temp[MAX_PATH];
        GetTempPathW(MAX_PATH, temp);
        return fs::path(temp) / L"cadence_installer.log";
    }();
    std::ofstream file(path, std::ios::app);
    file << narrow(text) << '\n';
    if (g_console) std::fwprintf(stdout, L"%ls\n", text.c_str());
}

// ---- The payload carried inside this exe ---------------------------------------------------

struct PayloadFile {
    std::string path;                      // relative to the GD folder, forward slashes
    const unsigned char* data = nullptr;
    uint64_t size = 0;
};

std::vector<PayloadFile> load_payload() {
    std::vector<PayloadFile> files;
    HRSRC res = FindResourceW(nullptr, MAKEINTRESOURCEW(1), RT_RCDATA);
    if (!res) return files;
    HGLOBAL loaded = LoadResource(nullptr, res);
    const auto* base = static_cast<const unsigned char*>(LockResource(loaded));
    const size_t total = SizeofResource(nullptr, res);
    if (!base || total < 12 || std::memcmp(base, "CDNPAK01", 8) != 0) return files;

    size_t at = 8;
    uint32_t count;
    std::memcpy(&count, base + at, 4);
    at += 4;
    for (uint32_t i = 0; i < count; i++) {
        uint32_t path_len;
        uint64_t size;
        if (at + 4 > total) return {};
        std::memcpy(&path_len, base + at, 4);
        at += 4;
        if (at + path_len + 8 > total) return {};
        PayloadFile f;
        f.path.assign(reinterpret_cast<const char*>(base + at), path_len);
        at += path_len;
        std::memcpy(&size, base + at, 8);
        at += 8;
        if (at + size > total) return {};
        f.data = base + at;
        f.size = size;
        at += static_cast<size_t>(size);
        files.push_back(std::move(f));
    }
    return files;
}

bool is_mod(const PayloadFile& f) { return f.path.rfind("mods/", 0) == 0; }

// ---- Finding Geometry Dash -------------------------------------------------------------------

std::optional<std::wstring> registry_string(HKEY root, const wchar_t* subkey, const wchar_t* name, REGSAM view = 0) {
    HKEY key;
    if (RegOpenKeyExW(root, subkey, 0, KEY_READ | view, &key) != ERROR_SUCCESS) return std::nullopt;
    wchar_t buffer[1024];
    DWORD size = sizeof(buffer), type = 0;
    LONG rc = RegQueryValueExW(key, name, nullptr, &type, reinterpret_cast<BYTE*>(buffer), &size);
    RegCloseKey(key);
    if (rc != ERROR_SUCCESS || type != REG_SZ) return std::nullopt;
    return std::wstring(buffer);
}

// "path"  "C:\\Program Files (x86)\\Steam" lines of steamapps/libraryfolders.vdf.
std::vector<fs::path> steam_library_folders(const fs::path& steam_root) {
    std::vector<fs::path> libraries{steam_root};
    std::ifstream file(steam_root / L"steamapps" / L"libraryfolders.vdf");
    for (std::string line; std::getline(file, line);) {
        auto key = line.find("\"path\"");
        if (key == std::string::npos) continue;
        auto open = line.find('"', key + 6);
        auto close = open == std::string::npos ? std::string::npos : line.find('"', open + 1);
        if (open == std::string::npos || close == std::string::npos) continue;
        std::string value = line.substr(open + 1, close - open - 1);
        std::string unescaped;
        for (size_t i = 0; i < value.size(); i++) {
            if (value[i] == '\\' && i + 1 < value.size() && value[i + 1] == '\\') i++;
            unescaped += value[i];
        }
        libraries.emplace_back(widen(unescaped));
    }
    return libraries;
}

bool looks_like_gd(const fs::path& dir) {
    std::error_code ec;
    return fs::exists(dir / L"GeometryDash.exe", ec);
}

std::vector<fs::path> find_geometry_dash() {
    std::vector<fs::path> roots;
    if (auto p = registry_string(HKEY_CURRENT_USER, L"Software\\Valve\\Steam", L"SteamPath")) roots.emplace_back(*p);
    if (auto p = registry_string(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Valve\\Steam", L"InstallPath", KEY_WOW64_32KEY)) roots.emplace_back(*p);
    roots.emplace_back(L"C:\\Program Files (x86)\\Steam");
    roots.emplace_back(L"C:\\Program Files\\Steam");

    std::vector<fs::path> found;
    for (const fs::path& root : roots) {
        std::error_code ec;
        if (!fs::exists(root, ec)) continue;
        for (const fs::path& library : steam_library_folders(root)) {
            fs::path dir = library / L"steamapps" / L"common" / L"Geometry Dash";
            if (!looks_like_gd(dir)) continue;
            bool seen = false;
            for (const fs::path& f : found) {
                std::error_code same_ec;
                if (fs::equivalent(f, dir, same_ec)) seen = true;
            }
            if (!seen) found.push_back(dir);
        }
    }
    return found;
}

// ---- Checks before touching anything ---------------------------------------------------------

bool geometry_dash_running() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return false;
    PROCESSENTRY32W entry{sizeof(entry)};
    bool running = false;
    for (BOOL ok = Process32FirstW(snapshot, &entry); ok; ok = Process32NextW(snapshot, &entry))
        if (_wcsicmp(entry.szExeFile, L"GeometryDash.exe") == 0) running = true;
    CloseHandle(snapshot);
    return running;
}

std::optional<uint32_t> pe_timestamp(const fs::path& exe) {
    std::ifstream file(exe, std::ios::binary);
    IMAGE_DOS_HEADER dos;
    if (!file.read(reinterpret_cast<char*>(&dos), sizeof(dos)) || dos.e_magic != IMAGE_DOS_SIGNATURE) return std::nullopt;
    file.seekg(dos.e_lfanew);
    DWORD signature;
    IMAGE_FILE_HEADER header;
    if (!file.read(reinterpret_cast<char*>(&signature), 4) || signature != IMAGE_NT_SIGNATURE) return std::nullopt;
    if (!file.read(reinterpret_cast<char*>(&header), sizeof(header))) return std::nullopt;
    return header.TimeDateStamp;
}

bool known_version(const fs::path& exe) {
    auto stamp = pe_timestamp(exe);
    if (!stamp) return false;
    for (uint32_t known : kKnownTimestamps)
        if (*stamp == known) return true;
    return false;
}

// ---- What this installer wrote ---------------------------------------------------------------

struct Manifest {
    bool present = false;
    std::vector<std::string> files;        // relative paths, forward slashes
    bool backed_up = false;                // an existing winmm.dll was renamed to kBackupName
};

Manifest read_manifest(const fs::path& gd) {
    Manifest m;
    std::ifstream file(gd / kManifestName);
    if (!file) return m;
    m.present = true;
    for (std::string line; std::getline(file, line);) {
        while (!line.empty() && (line.back() == '\r')) line.pop_back();
        if (line.rfind("file ", 0) == 0) m.files.push_back(line.substr(5));
        else if (line == "backup winmm.dll") m.backed_up = true;
    }
    return m;
}

bool write_manifest(const fs::path& gd, const Manifest& m) {
    std::ofstream file(gd / kManifestName, std::ios::trunc);
    if (!file) return false;
    file << "cadence " << CADENCE_VERSION << "\n";
    if (m.backed_up) file << "backup winmm.dll\n";
    for (const std::string& f : m.files) file << "file " << f << "\n";
    return static_cast<bool>(file);
}

// ---- Install / uninstall ------------------------------------------------------------------------

struct Outcome {
    bool ok = false;
    std::wstring message;
};

using Progress = std::function<void(int done, int total, const std::wstring& text)>;

bool write_file(const fs::path& path, const unsigned char* data, uint64_t size, std::wstring& error) {
    HANDLE h = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) {
        DWORD e = GetLastError();
        error = L"could not write " + path.filename().wstring() + L": " + system_message(e);
        if (e == ERROR_ACCESS_DENIED) error += L" (try running the installer as administrator)";
        if (e == ERROR_SHARING_VIOLATION) error += L" (is Geometry Dash running?)";
        return false;
    }
    bool ok = true;
    for (uint64_t at = 0; ok && at < size;) {
        DWORD chunk = static_cast<DWORD>(std::min<uint64_t>(size - at, 1u << 20)), written = 0;
        ok = WriteFile(h, data + at, chunk, &written, nullptr) && written == chunk;
        at += chunk;
    }
    if (!ok) error = L"could not write " + path.filename().wstring() + L": " + system_message(GetLastError());
    CloseHandle(h);
    return ok;
}

Outcome do_install(const fs::path& gd, bool with_mods, const Progress& progress) {
    auto files = load_payload();
    if (files.empty()) return {false, L"This installer has no files inside it (it was built without the payload)."};

    std::error_code ec;
    Manifest previous = read_manifest(gd);
    Manifest manifest;
    manifest.backed_up = previous.backed_up;

    // Another loader's winmm.dll is moved aside, not overwritten.
    fs::path loader = gd / L"winmm.dll";
    if (fs::exists(loader, ec) && !previous.present) {
        fs::path backup = gd / kBackupName;
        if (fs::exists(backup, ec))
            return {false, L"winmm.dll already exists here and so does winmm.dll.pre-cadence. "
                           L"Move one of them away first, so nothing gets overwritten."};
        fs::rename(loader, backup, ec);
        if (ec) return {false, L"could not move the existing winmm.dll aside: " + widen(ec.message())};
        manifest.backed_up = true;
        log_line(L"existing winmm.dll renamed to winmm.dll.pre-cadence");
    }

    std::vector<const PayloadFile*> todo;
    for (const PayloadFile& f : files)
        if (with_mods || !is_mod(f)) todo.push_back(&f);

    int done = 0;
    for (const PayloadFile* f : todo) {
        fs::path dest = gd / widen(f->path);
        progress(done, static_cast<int>(todo.size()), L"Copying " + dest.filename().wstring());
        fs::create_directories(dest.parent_path(), ec);
        std::wstring error;
        if (!write_file(dest, f->data, f->size, error)) return {false, error};
        manifest.files.push_back(f->path);
        log_line(L"wrote " + dest.wstring());
        done++;
    }
    if (!write_manifest(gd, manifest)) return {false, L"installed, but could not write " + std::wstring(kManifestName)};
    progress(done, static_cast<int>(todo.size()), L"Done");

    std::wostringstream msg;
    msg << L"Installed the loader" << (with_mods ? L" and " + std::to_wstring(todo.size() - 2) + L" files" : L"") << L".";
    return {true, msg.str()};
}

Outcome do_uninstall(const fs::path& gd, const Progress& progress) {
    Manifest manifest = read_manifest(gd);
    std::vector<std::string> files = manifest.files;
    if (!manifest.present) {
        // An install made by install.ps1 or by hand has no manifest: remove what this installer would have written.
        for (const PayloadFile& f : load_payload()) files.push_back(f.path);
    }

    std::error_code ec;
    int done = 0;
    for (const std::string& rel : files) {
        progress(done++, static_cast<int>(files.size()), L"Removing " + widen(rel));
        fs::path path = gd / widen(rel);
        if (!fs::exists(path, ec)) continue;
        fs::remove(path, ec);
        if (ec) return {false, L"could not remove " + path.filename().wstring() + L": " + widen(ec.message()) +
                               L" (is Geometry Dash running?)"};
        log_line(L"removed " + path.wstring());
    }

    fs::remove_all(gd / L"mods" / L".cadence-cache", ec);          // shadow copies made by hot reload
    for (const wchar_t* dir : {L"generated", L"mods"}) {           // only if nothing else lives there
        std::error_code empty_ec;
        if (fs::is_directory(gd / dir, empty_ec) && fs::is_empty(gd / dir, empty_ec)) fs::remove(gd / dir, ec);
    }

    if (manifest.backed_up) {
        fs::path backup = gd / kBackupName, loader = gd / L"winmm.dll";
        if (fs::exists(backup, ec) && !fs::exists(loader, ec)) {
            fs::rename(backup, loader, ec);
            if (!ec) log_line(L"restored the winmm.dll that was here before");
        }
    }
    fs::remove(gd / kManifestName, ec);
    progress(done, static_cast<int>(files.size()), L"Done");
    return {true, L"Cadence has been removed. Your own mods, settings and logs were left alone."};
}

// ---- The window -------------------------------------------------------------------------------------

enum Ids { IdPath = 101, IdBrowse, IdMods, IdInstall, IdUninstall, IdStatus, IdProgress };

struct Ui {
    HWND window = nullptr, path = nullptr, mods = nullptr, install = nullptr, uninstall = nullptr, status = nullptr, progress = nullptr;
    HFONT font = nullptr, title_font = nullptr;
} ui;

fs::path chosen_folder() {
    wchar_t buffer[MAX_PATH * 2] = {};
    GetWindowTextW(ui.path, buffer, static_cast<int>(std::size(buffer)));
    std::wstring text = buffer;
    while (!text.empty() && (text.back() == L'\\' || text.back() == L'/' || text.back() == L'"' || text.back() == L' ')) text.pop_back();
    while (!text.empty() && (text.front() == L'"' || text.front() == L' ')) text.erase(text.begin());
    return text;
}

void set_status(const std::wstring& text) {
    SetWindowTextW(ui.status, text.c_str());
    log_line(text);
    UpdateWindow(ui.window);
}

void set_busy(bool busy) {
    for (HWND h : {ui.install, ui.uninstall, ui.path, ui.mods}) EnableWindow(h, !busy);
    UpdateWindow(ui.window);
}

void refresh_status() {
    fs::path dir = chosen_folder();
    if (dir.empty()) return;
    if (!looks_like_gd(dir)) { set_status(L"GeometryDash.exe is not in that folder."); return; }
    Manifest m = read_manifest(dir);
    std::error_code ec;
    bool loader_here = fs::exists(dir / L"winmm.dll", ec);
    if (m.present) set_status(L"Cadence is installed here. Install again to update it, or uninstall it.");
    else if (loader_here) set_status(L"There is already a winmm.dll here (another mod loader?). Installing will move it aside, not overwrite it.");
    else if (!known_version(dir / L"GeometryDash.exe")) set_status(L"This does not look like Geometry Dash " L"2.2091" L". Cadence only supports that version.");
    else set_status(L"Geometry Dash 2.2091 found. Ready to install.");
}

bool precheck(const fs::path& gd, bool allow_unknown_version, bool ask_user, std::wstring& problem) {
    if (!looks_like_gd(gd)) { problem = L"GeometryDash.exe is not in " + gd.wstring(); return false; }
    if (geometry_dash_running()) { problem = L"Geometry Dash is running. Close it first."; return false; }
    if (!known_version(gd / L"GeometryDash.exe") && !allow_unknown_version) {
        if (!ask_user) { problem = L"this is not Geometry Dash 2.2091 (use --allow-unknown-version to install anyway)"; return false; }
        int answer = MessageBoxW(ui.window,
            L"This does not look like Geometry Dash 2.2091. Cadence only works with that exact version, and with another version "
            L"the game can crash.\n\nInstall anyway?", L"Cadence", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
        if (answer != IDYES) { problem = L"Cancelled."; return false; }
    }
    return true;
}

void run_action(bool installing) {
    fs::path gd = chosen_folder();
    std::wstring problem;
    if (!precheck(gd, !installing, true, problem)) {       // uninstalling does not care about the version
        set_status(problem);
        if (problem != L"Cancelled.") MessageBoxW(ui.window, problem.c_str(), L"Cadence", MB_OK | MB_ICONWARNING);
        return;
    }
    set_busy(true);
    auto progress = [](int done, int total, const std::wstring& text) {
        SendMessageW(ui.progress, PBM_SETRANGE32, 0, std::max(total, 1));
        SendMessageW(ui.progress, PBM_SETPOS, done, 0);
        SetWindowTextW(ui.status, text.c_str());
        UpdateWindow(ui.window);
    };
    bool with_mods = SendMessageW(ui.mods, BM_GETCHECK, 0, 0) == BST_CHECKED;
    Outcome result = installing ? do_install(gd, with_mods, progress) : do_uninstall(gd, progress);
    set_busy(false);
    if (result.ok) {
        std::wstring text = result.message;
        if (installing) text += L"\n\nStart Geometry Dash from Steam. Press F6 in game to reload mods.";
        set_status(text);
        MessageBoxW(ui.window, text.c_str(), L"Cadence", MB_OK | MB_ICONINFORMATION);
    } else {
        set_status(L"Failed: " + result.message);
        MessageBoxW(ui.window, result.message.c_str(), L"Cadence: something went wrong", MB_OK | MB_ICONERROR);
    }
    refresh_status();
}

void browse() {
    BROWSEINFOW info{};
    info.hwndOwner = ui.window;
    info.lpszTitle = L"Select your Geometry Dash folder (the one with GeometryDash.exe)";
    info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    if (PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW(&info)) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) SetWindowTextW(ui.path, path);
        CoTaskMemFree(pidl);
        refresh_status();
    }
}

HWND make(const wchar_t* cls, const wchar_t* text, DWORD style, int x, int y, int w, int h, int id, DWORD ex = 0) {
    HWND control = CreateWindowExW(ex, cls, text, WS_CHILD | WS_VISIBLE | style, x, y, w, h, ui.window,
                                  reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), GetModuleHandleW(nullptr), nullptr);
    SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(ui.font), TRUE);
    return control;
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE: {
            ui.window = hwnd;
            NONCLIENTMETRICSW metrics{sizeof(metrics)};
            SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0);
            ui.font = CreateFontIndirectW(&metrics.lfMessageFont);
            LOGFONTW big = metrics.lfMessageFont;
            big.lfHeight = big.lfHeight * 2;
            big.lfWeight = FW_BOLD;
            ui.title_font = CreateFontIndirectW(&big);

            HWND title = make(L"STATIC", L"Cadence", SS_LEFT, 20, 14, 300, 34, 0);
            SendMessageW(title, WM_SETFONT, reinterpret_cast<WPARAM>(ui.title_font), TRUE);
            make(L"STATIC", L"A mod loader for Geometry Dash 2.2091 (version " CADENCE_VERSION L")", SS_LEFT, 22, 52, 480, 18, 0);
            make(L"STATIC", L"Geometry Dash folder:", SS_LEFT, 22, 88, 300, 18, 0);
            ui.path = make(L"EDIT", L"", ES_AUTOHSCROLL | WS_TABSTOP, 22, 110, 396, 24, IdPath, WS_EX_CLIENTEDGE);
            make(L"BUTTON", L"Browse...", BS_PUSHBUTTON | WS_TABSTOP, 428, 109, 84, 26, IdBrowse);
            ui.mods = make(L"BUTTON", L"Also install the example mods (hitboxes, noclip, speedhack, ...)", BS_AUTOCHECKBOX | WS_TABSTOP,
                           22, 146, 480, 20, IdMods);
            SendMessageW(ui.mods, BM_SETCHECK, BST_CHECKED, 0);
            ui.status = make(L"STATIC", L"", SS_LEFT, 22, 180, 490, 64, IdStatus);
            ui.progress = make(PROGRESS_CLASSW, L"", PBS_SMOOTH, 22, 252, 490, 16, IdProgress);
            ui.uninstall = make(L"BUTTON", L"Uninstall", BS_PUSHBUTTON | WS_TABSTOP, 290, 286, 108, 30, IdUninstall);
            ui.install = make(L"BUTTON", L"Install", BS_DEFPUSHBUTTON | WS_TABSTOP, 406, 286, 106, 30, IdInstall);

            auto found = find_geometry_dash();
            if (!found.empty()) SetWindowTextW(ui.path, found.front().c_str());
            else set_status(L"Could not find Geometry Dash automatically. Click Browse and pick the folder that contains GeometryDash.exe.");
            if (!found.empty()) refresh_status();
            return 0;
        }
        case WM_COMMAND:
            switch (LOWORD(wparam)) {
                case IdBrowse: browse(); break;
                case IdInstall: run_action(true); break;
                case IdUninstall: run_action(false); break;
                case IdPath: if (HIWORD(wparam) == EN_KILLFOCUS) refresh_status(); break;
            }
            return 0;
        case WM_DESTROY:
            DeleteObject(ui.font);
            DeleteObject(ui.title_font);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

int run_window(bool smoke_test) {
    INITCOMMONCONTROLSEX controls{sizeof(controls), ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES};
    InitCommonControlsEx(&controls);
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    WNDCLASSW cls{};
    cls.lpfnWndProc = window_proc;
    cls.hInstance = GetModuleHandleW(nullptr);
    cls.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    cls.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    cls.lpszClassName = L"CadenceInstaller";
    cls.hIcon = LoadIconW(cls.hInstance, MAKEINTRESOURCEW(2));
    RegisterClassW(&cls);

    RECT rect{0, 0, 534, 330};
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    AdjustWindowRect(&rect, style, FALSE);
    HWND hwnd = CreateWindowExW(0, cls.lpszClassName, L"Cadence installer", style, CW_USEDEFAULT, CW_USEDEFAULT,
                                rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, cls.hInstance, nullptr);
    if (!hwnd) return 1;
    ShowWindow(hwnd, SW_SHOW);
    if (smoke_test) {
        log_line(L"gui smoke test: window created with the controls");
        SetTimer(hwnd, 1, 300, [](HWND w, UINT, UINT_PTR, DWORD) { PostMessageW(w, WM_CLOSE, 0, 0); });
    }
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (!IsDialogMessageW(hwnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    return 0;
}

// ---- Command line ----------------------------------------------------------------------------------

struct Options {
    bool silent = false, uninstall = false, with_mods = true, allow_unknown_version = false, gui_smoke = false;
    fs::path gd_dir;
};

int run_silent(const Options& options) {
    fs::path gd = options.gd_dir;
    if (gd.empty()) {
        auto found = find_geometry_dash();
        if (found.empty()) { log_line(L"error: Geometry Dash not found; pass --gd-dir <folder>"); return 1; }
        gd = found.front();
    }
    std::wstring problem;
    ui.window = nullptr;
    if (!precheck(gd, options.allow_unknown_version || options.uninstall, false, problem)) {
        log_line(L"error: " + problem);
        return 1;
    }
    auto progress = [](int done, int total, const std::wstring& text) {
        log_line(L"[" + std::to_wstring(done) + L"/" + std::to_wstring(total) + L"] " + text);
    };
    Outcome result = options.uninstall ? do_uninstall(gd, progress) : do_install(gd, options.with_mods, progress);
    log_line((result.ok ? L"ok: " : L"error: ") + result.message);
    return result.ok ? 0 : 1;
}

} // namespace

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    Options options;
    bool usage_error = false;
    for (int i = 1; i < argc; i++) {
        std::wstring a = argv[i];
        if (a == L"--silent") options.silent = true;
        else if (a == L"--uninstall") options.uninstall = true;
        else if (a == L"--install") options.uninstall = false;
        else if (a == L"--no-mods") options.with_mods = false;
        else if (a == L"--allow-unknown-version") options.allow_unknown_version = true;
        else if (a == L"--gui-smoke") options.gui_smoke = true;
        else if (a == L"--gd-dir" && i + 1 < argc) options.gd_dir = argv[++i];
        else usage_error = true;
    }
    LocalFree(argv);

    if (options.silent || usage_error) {
        if (AttachConsole(ATTACH_PARENT_PROCESS)) {
            g_console = true;
            FILE* out;
            freopen_s(&out, "CONOUT$", "w", stdout);
        }
    }
    if (usage_error) {
        log_line(L"usage: CadenceInstaller [--silent] [--install|--uninstall] [--gd-dir <folder>] [--no-mods] "
                 L"[--allow-unknown-version] [--gui-smoke]");
        return 2;
    }
    if (options.silent) return run_silent(options);
    return run_window(options.gui_smoke);
}
