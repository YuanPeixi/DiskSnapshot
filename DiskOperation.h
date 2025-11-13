#pragma once
#include <ctime>
#include <string>
#include <Windows.h>

#include "BackupTreeXml.h"

using namespace std;

enum VHDXType
{
	fixed,expandable,copy,different
};

//Naked Operations
bool CreateVHDX(string paht,int maxMiB, VHDXType type,string parent)
{
	if (type == different || parent != "") {

	}
}


VHDXNode* Snapshot(string name,string desc,string path,string parent)
{
	//Before snapshot compact
	VHDXNode* node = new VHDXNode;
	node->name = name;
	node->desc = desc;
	node->path = path;
	node->parent = parent;
	node->timeSnap = time(NULL);
	string cmd = "create vdisk file=";
	cmd+='\"',cmd += path, cmd += '\"';
	cmd += ' ';
	cmd += "parent=\"", cmd += parent, cmd += '\"';
	WinExec(cmd.c_str(), SW_HIDE);
	return node;
}

void SwitchTo()
{
	//bcdedit /set {} default
	//reboot
}

void Recover()
{
	//del now.vhdx
	//bcdedit /set {orin.vhdx} default
	//reboot
}

void Delete()
{
	//Merge vhdx
	//sel child
	//merge (to parent)
	//make sure there are no branch on parent or decendant
}

void Fork()
{
	//Create 0 and 1 two children
}

#include <Windows.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Shell32.lib")

// CreateProcess方案 - 功能最完整
bool CreateProcessDiskpart(const std::string& command) {
    SECURITY_ATTRIBUTES sa;
    HANDLE hReadPipe, hWritePipe;
    
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        return false;
    }

    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    
    si.cb = sizeof(STARTUPINFOA);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.wShowWindow = SW_HIDE;

    std::string fullCmd = "diskpart /s \"" + command + "\"";
    
    BOOL success = CreateProcessA(
        NULL, 
        (LPSTR)fullCmd.c_str(), 
        NULL, 
        NULL, 
        TRUE, 
        CREATE_NO_WINDOW, 
        NULL, 
        NULL, 
        &si, 
        &pi
    );

    if (!success) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return false;
    }

    CloseHandle(hWritePipe);

    char buffer[4096];
    DWORD bytesRead;
    std::string output;

    while (ReadFile(hReadPipe, buffer, sizeof(buffer)-1, &bytesRead, NULL)) {
        if (bytesRead == 0) break;
        buffer[bytesRead] = '\0';
        output += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    
    std::cout << "Diskpart Output:\n" << output << std::endl;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hReadPipe);

    return true;
}

// ShellExecute方案 - 中等复杂度
bool ShellExecuteDiskpart(const std::string& command) {
    SHELLEXECUTEINFOA ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = "open";
    ShExecInfo.lpFile = "diskpart.exe";
    ShExecInfo.lpParameters = ("/s \"" + command + "\"").c_str();
    ShExecInfo.nShow = SW_HIDE;
    
    if (ShellExecuteExA(&ShExecInfo)) {
        WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
        CloseHandle(ShExecInfo.hProcess);
        return true;
    }
    return false;
}

// WinExec方案 - 最简单但不推荐用于生产环境
bool WinExecDiskpart(const std::string& command) {
    std::string fullCmd = "diskpart /s \"" + command + "\"";
    UINT result = WinExec(fullCmd.c_str(), SW_HIDE);
    return result > 31;
}

int main() {
    std::string scriptFile = "diskpart_commands.txt";
    
    // 创建diskpart命令文件
    FILE* f = fopen(scriptFile.c_str(), "w");
    if (f) {
        fprintf(f, "list vol\n");
        fprintf(f,"sel vol");
        fprintf(f, "exit\n");
        fclose(f);
    }

    std::cout << "=== WinExec方案 ===" << std::endl;
    if (WinExecDiskpart(scriptFile)) {
        std::cout << "执行成功" << std::endl;
    }

    std::cout << "\n=== ShellExecute方案 ===" << std::endl;
    if (ShellExecuteDiskpart(scriptFile)) {
        std::cout << "执行成功" << std::endl;
    }

    std::cout << "\n=== CreateProcess方案 ===" << std::endl;
    if (CreateProcessDiskpart(scriptFile)) {
        std::cout << "执行成功" << std::endl;
    }

    // 清理临时文件
    remove(scriptFile.c_str());
    
    return 0;
}
