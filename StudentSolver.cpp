#ifndef STUDENTSOLVER_CPP
#define STUDENTSOLVER_CPP

#include "Maze.cpp"

int pathSteps(PathNode*path) {
    int len = 0;
    PathNode* cur = path;

    while (cur != nullptr) {
        len++;
        cur = cur ->next;
    }
    if( len == 0)
    return 0;
    
    return len - 1;
}

void push(PathNode*& stack, int r, int c) {
    PathNode* newNode = new PathNode(r, c, stack);
    stack = newNode;
}
bool pop(PathNode*& stack, int& r, int& c) {
    if(stack == nullptr)
    return false;

    PathNode* temp = stack;
    r = temp->r;
    c = temp->c;
    stack = stack-> next;
    delete temp;

    return true;
}

bool solveByDFS(const Maze& maze, PathNode*& path) {
    int n = maze.getN();
    path = nullptr;

    int sr = maze.getStartR();
    int sc = maze.getStartC();
    int er = maze.getEndR();
    int ec = maze.getEndC();
    
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    
    // 1. 動態配置矩陣並確保初始化為 false
    bool** visited = new bool*[n];
    int** parentR = new int*[n];
    int** parentC = new int*[n];
    for(int i = 0; i < n; i++){
        visited[i] = new bool[n];
        parentR[i] = new int[n];
        parentC[i] = new int[n];
        for(int j = 0; j < n; j++) { 
            visited[i][j] = false; // 確保清空，否則會亂跑
            parentR[i][j] = -1; 
            parentC[i][j] = -1; 
        }
    }

    PathNode* stack = nullptr;
    push(stack, sr, sc);
    visited[sr][sc] = true; // 起點標記為已訪問
    
    bool found = false;
    int r, c;

    // 2. DFS 主迴圈
    while(pop(stack, r, c)) {
        if(r == er && c == ec) {
            found = true;
            break;
        }

        for(int i = 0; i < 4; i++){
            int nr = r + dr[i], nc = c + dc[i];
            // 檢查邊界與牆壁
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && !maze.isWall(nr, nc) && !visited[nr][nc]) {
                visited[nr][nc] = true; 
                parentR[nr][nc] = r;
                parentC[nr][nc] = c; 
                push(stack, nr, nc);
            }
        }
    }

    // 3. 回溯建立路徑
    if (found) {
        int currR = er, currC = ec;
        while(currR != -1) {
            path = new PathNode(currR, currC, path); // 建立起點到終點的路徑
            int prevR = parentR[currR][currC];
            int prevC = parentC[currR][currC];
            currR = prevR; currC = prevC;
        }
    }

    // 4. 重要：釋放所有剩餘資源 
    int tr, tc;
    while(pop(stack, tr, tc)); // 清空 Stack 剩餘節點

    for (int i = 0; i < n; i++) {
        delete[] visited[i];
        delete[] parentR[i];
        delete[] parentC[i];
    }
    delete[] visited;
    delete[] parentR;
    delete[] parentC;
    
    return found;
}
    
void enqueue(PathNode*& head, PathNode*& tail, int r, int c) {
    PathNode* node = new PathNode(r, c, nullptr);

    if (tail == nullptr) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
}

bool dequeue(PathNode*& head, PathNode*& tail, int& r, int& c) {
    if (head == nullptr)
        return false;

    PathNode* temp = head;

    r = temp->r;
    c = temp->c;

    head = head->next;

    if (head == nullptr)
        tail = nullptr;

    delete temp;

    return true;
}

bool solveByBFS(const Maze& maze, PathNode*& path) {
    int n = maze.getN();
    int sr = maze.getStartR(), sc = maze.getStartC();
    int endR = maze.getEndR(), endC = maze.getEndC();
    
    //  配置輔助陣列
    bool** visited = new bool*[n];
    int** parentR = new int*[n];
    int** parentC = new int*[n];
    for (int i = 0; i < n; i++) {
        visited[i] = new bool[n](); // 初始化為 false
        parentR[i] = new int[n];
        parentC[i] = new int[n];
        for (int j = 0; j < n; j++) {
            parentR[i][j] = -1;
            parentC[i][j] = -1;
        }
    }

    PathNode* head = nullptr;
    PathNode* tail = nullptr;
    
    enqueue(head, tail, sr, sc);
    visited[sr][sc] = true;
    
    bool found = false;
    int r, c;

    //  BFS 主迴圈
    while (dequeue(head, tail, r, c)) {
        if (r == endR && c == endC) {
            found = true;
            break;
        }

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < n && nc >= 0 && nc < n && 
                !maze.isWall(nr, nc) && !visited[nr][nc]) {
                
                visited[nr][nc] = true;
                parentR[nr][nc] = r; 
                parentC[nr][nc] = c;
                enqueue(head, tail, nr, nc);
            }
        }
    }

    if (found) {
        int br = endR, bc = endC; 
        while (br != -1) {
            path = new PathNode(br, bc, path); 
            int nextR = parentR[br][bc];
            bc = parentC[br][bc];
            br = nextR;
        }
    }

    int tempR, tempC;
    while (dequeue(head, tail, tempR, tempC)); 

    for (int i = 0; i < n; i++) {
        delete[] visited[i];
        delete[] parentR[i];
        delete[] parentC[i];
    }
    delete[] visited;
    delete[] parentR;
    delete[] parentC;

    return found;
}

#endif
