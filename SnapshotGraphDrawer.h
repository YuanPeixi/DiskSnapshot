#pragma once
#include "framework.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include "BackupTreeXml.h"

// 构建树结构，并按层展开

//int HitTestSnapshotNode(const std::vector<RECT>& nodeRects, int x, int y);
int HitTestSnapshotNode(int x, int y);
void DrawSnapshotGraphTree(HDC dc, const std::vector<VHDXNode>& nodes, int startX, int startY, int selectedIdx = -1);

void DrawNodeInfo(HDC dc, const VHDXNode& node, int x, int y);