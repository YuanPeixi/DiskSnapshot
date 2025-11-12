#pragma once
#include <ctime>
#include <string>
#include <Windows.h>

#include "BackupTreeXml.h"

using namespace std;

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