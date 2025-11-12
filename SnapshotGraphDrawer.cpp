#include "SnapshotGraphDrawer.h"

#ifndef LEGACYCODE
#include <algorithm>
#include <sstream>

static std::vector<CRect> outNodeRects;

struct TreeNode {
    int idx; // 复用nodes索引
    std::vector<TreeNode*> children;
    TreeNode* parent;
    int depth;  // 层号
};

static void BuildTree(const std::vector<VHDXNode>& nodes, std::vector<TreeNode>& treeNodes, std::vector<TreeNode*>& roots)
{
    int N = nodes.size();
    treeNodes.resize(N);
    std::map<std::string, int> name2idx;
    for (int i = 0; i < N; ++i) {
        treeNodes[i].idx = i; treeNodes[i].parent = nullptr;
        name2idx[nodes[i].name] = i;
    }
    for (int i = 0; i < N; ++i) {
        if (nodes[i].parent.empty()) roots.push_back(&treeNodes[i]);
        else {
            auto it = name2idx.find(nodes[i].parent);
            if (it != name2idx.end()) {
                treeNodes[i].parent = &treeNodes[it->second];
                treeNodes[it->second].children.push_back(&treeNodes[i]);
            }
        }
    }
}

static void AssignRowCol(TreeNode* root, int depth, int& nextCol, std::vector<std::vector<int>>& layerIdxs, std::vector<int>& nodeCols)
{
    root->depth = depth;
    if (layerIdxs.size() <= depth) layerIdxs.resize(depth + 1);
    layerIdxs[depth].push_back(root->idx);
    nodeCols[root->idx] = nextCol++;
    for (auto* c : root->children) AssignRowCol(c, depth + 1, nextCol, layerIdxs, nodeCols);
}

void DrawSnapshotGraphTree(HDC dc, const std::vector<VHDXNode>& nodes, int startX, int startY, int selectedIdx)
{
    if (nodes.empty()) return;
    // --- 1 构建树结构
    std::vector<TreeNode> treeNodes; std::vector<TreeNode*> roots;
    BuildTree(nodes, treeNodes, roots);
    std::vector<std::vector<int>> layerIdxs; // 每层的节点idx列表
    std::vector<int> nodeCols(nodes.size(), -1); int nextCol = 0;
    for (auto* r : roots) AssignRowCol(r, 0, nextCol, layerIdxs, nodeCols); // 分层

    const int nodeW = 200, nodeH = 60, gapX = 60, gapY = 80;
    // --- 2. 计算每个节点坐标
    struct { int x, y; } pos[100]; // 假设最多100节点
    for (size_t depth = 0; depth < layerIdxs.size(); ++depth) {
        const auto& idxs = layerIdxs[depth];
        for (size_t i = 0; i < idxs.size(); ++i) {
            int idx = idxs[i];
            pos[idx].x = startX + nodeCols[idx] * (nodeW + gapX);
            pos[idx].y = startY + depth * (nodeH + gapY);
        }
    }
    /*
    // --- 3. 绘制节点与箭头
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        RECT rc{ pos[i].x, pos[i].y, pos[i].x + nodeW, pos[i].y + nodeH };
        HBRUSH hBrush = CreateSolidBrush((int)i == selectedIdx ? RGB(180, 220, 255) : RGB(230, 230, 230));
        FillRect(dc, &rc, hBrush); DeleteObject(hBrush);
        Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
        // 上排名字
        RECT nameRc = rc; nameRc.bottom = nameRc.top + nodeH / 2;
        DrawTextA(dc, nodes[i].name.c_str(), -1, &nameRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
        // 下排日期
        RECT dateRc = rc; dateRc.top += nodeH / 2;
        std::string dt = FormatTime(nodes[i].timeSnap);
        DrawTextA(dc, dt.c_str(), -1, &dateRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    }
    */
    //提供节点位置 (V3)
    outNodeRects.resize(nodes.size());
    // --- 3. 绘制节点与箭头
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        RECT rc{ pos[i].x, pos[i].y, pos[i].x + nodeW, pos[i].y + nodeH };
        outNodeRects[i] = rc; // 记录当前位置
        HBRUSH hBrush = CreateSolidBrush((int)i == selectedIdx ? RGB(180, 220, 255) : RGB(230, 230, 230));
        FillRect(dc, &rc, hBrush); DeleteObject(hBrush);
        Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);

        RECT nameRc = rc; nameRc.bottom = nameRc.top + nodeH / 2;
        DrawTextA(dc, nodes[i].name.c_str(), -1, &nameRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
        RECT dateRc = rc; dateRc.top += nodeH / 2;
        std::string dt = FormatTime(nodes[i].timeSnap);
        DrawTextA(dc, dt.c_str(), -1, &dateRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    }

    // --- 箭头
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i].parent.empty()) continue;
        int pIdx = treeNodes[i].parent->idx;
        int x1 = pos[pIdx].x + nodeW / 2;
        int y1 = pos[pIdx].y + nodeH;
        int x2 = pos[i].x + nodeW / 2;
        int y2 = pos[i].y;
        MoveToEx(dc, x1, y1, NULL);
        LineTo(dc, x2, y2);
        // 箭头V型
        const int arrowSize = 7;
        LineTo(dc, x2 - arrowSize, y2 + arrowSize);
        MoveToEx(dc, x2, y2, NULL);
        LineTo(dc, x2 + arrowSize, y2 + arrowSize);
    }
}

//int HitTestSnapshotNode(const std::vector<RECT>& nodeRects, int x, int y)
int HitTestSnapshotNode(int x, int y)
{
    for (size_t i = 0; i < outNodeRects.size(); ++i)
    {
        const RECT& r = outNodeRects[i];
        if (x >= r.left && x <= r.right && y >= r.top && y <= r.bottom)
            return static_cast<int>(i);
    }
    return -1;
}

#else
//Version 2
#include <sstream>

static int FindNodeIdx(const std::vector<VHDXNode>& nodes, const std::string& name)
{
    for (size_t i = 0; i < nodes.size(); ++i) if (nodes[i].name == name) return (int)i;
    return -1;
}

void DrawSnapshotGraph(HDC dc, const std::vector<VHDXNode>& nodes, int offsetY, int selectedIdx)
{
    const int nodeW = 200, nodeH = 60, gap = 80; // 加宽加高
    std::vector<POINT> nodePos(nodes.size());
    // 横排分配位置
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        nodePos[i].x = 40 + i * (nodeW + gap);
        nodePos[i].y = offsetY;
    }

    // 画框和文字
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        RECT rc{ nodePos[i].x, nodePos[i].y, nodePos[i].x + nodeW, nodePos[i].y + nodeH };
        HBRUSH hBrush = CreateSolidBrush(i == selectedIdx ? RGB(180, 220, 255) : RGB(230, 230, 230));
        FillRect(dc, &rc, hBrush); DeleteObject(hBrush);
        Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);

        // 名字第一行
        RECT nameRc = rc;
        nameRc.bottom = nameRc.top + nodeH / 2;
        DrawTextA(dc, nodes[i].name.c_str(), -1, &nameRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

        // 日期第二行
        RECT dateRc = rc;
        dateRc.top += nodeH / 2;
        std::string dt = FormatTime(nodes[i].timeSnap);
        DrawTextA(dc, dt.c_str(), -1, &dateRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
    }

    // 画箭头（父指向子）
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i].parent.empty()) continue;
        int pIdx = FindNodeIdx(nodes, nodes[i].parent);
        if (pIdx == -1) continue;
        int x1 = nodePos[pIdx].x + nodeW;
        int y1 = nodePos[pIdx].y + nodeH / 2;
        int x2 = nodePos[i].x;
        int y2 = nodePos[i].y + nodeH / 2;
        MoveToEx(dc, x1, y1, NULL);
        LineTo(dc, x2, y2);
        // 箭头
        int arrowSize = 7;
        LineTo(dc, x2 - arrowSize, y2 - arrowSize);
        MoveToEx(dc, x2, y2, NULL);
        LineTo(dc, x2 - arrowSize, y2 + arrowSize);
    }
}

#endif // LEGACYCODE

void DrawNodeInfo(HDC dc, const VHDXNode& node, int x, int y)
{
    std::ostringstream oss;
    oss << "Name: " << node.name << "\n";
    oss << "Date Time: " << FormatTime(node.timeSnap) << "\n";
    oss << "Description: " << node.desc << "\n";
    oss << "Parent: " << (node.parent.empty() ? "None" : node.parent) << "\n";
    RECT rc{ x, y, x + 300, y + 100 };
    DrawTextA(dc, oss.str().c_str(), -1, &rc, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOCLIP);
}