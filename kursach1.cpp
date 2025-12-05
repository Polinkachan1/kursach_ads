#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Edge {
    string source;
    string destination;
    int weight;
};

struct Graph {
    string vertices[50];
    Edge edges[100];
    int V;
    int E;
};


struct AdjacencyMatrix {
    int matrix[50][50];
    string vertices[50];
    int V;
};

struct AdjacencyList {
    struct ListNode {
        string dest;
        int weight;
        ListNode* next;
    };

    ListNode* list[50];
    string vertices[50];
    int V;
};

struct IncidenceMatrix {
    int matrix[50][100];  
    string vertices[50];
    Edge edges[100];
    int V;
    int E;
};

struct DSU {
    string parent[50];
    int rank[50];
    int size;
};

struct Stack {
    string data[50];
    int top;

    Stack() {
        top = -1;
    }

    void push(string value) {
        if (top < 49) {
            data[++top] = value;
        }
    }

    string pop() {
        if (top >= 0) {
            return data[top--];
        }
        return "";
    }

    bool isEmpty() {
        return top == -1;
    }
};

struct Queue {
    string data[26];
    int front;
    int rear;

    Queue() {
        front = 0;
        rear = -1;
    }

    void enqueue(string value) {
        if (rear < 25) {
            data[++rear] = value;
        }
    }

    string dequeue() {
        if (front <= rear) {
            return data[front++];
        }
        return "";
    }

    bool isEmpty() {
        return front > rear;
    }
};

int find_index(Graph* g, string vertex) {
    for (int i = 0; i < g->V; i++) {
        if (g->vertices[i] == vertex) {
            return i;
        }
    }
    return -1;
}

void display_menu() {
    cout << "\nКурсовая" << endl;
    cout << "1) Загрузить граф из файла" << endl;
    cout << "2) Вывести граф" << endl;
    cout << "3) Обход графа в ширину" << endl;
    cout << "4) Обход графа в глубину " << endl;
    cout << "5) Сортировка рёбер" << endl;
    cout << "6) Хранение графов" << endl;
    cout << "7) Система непересекающихся множеств" << endl;
    cout << "8) Алгоритм Краскала" << endl;
    cout << "9) Сохранить граф в файл" << endl;
    cout << "0) Выход" << endl;
    cout << "Выбор: ";
}

Graph* get_graph_from_file(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return nullptr;
    }

    Graph* graph = new Graph;
    graph->V = 0;
    graph->E = 0;
    string line;

    getline(file, line);
    stringstream ss(line);
    string vertex;

    while (ss >> vertex && graph->V < 26) {
        graph->vertices[graph->V] = vertex;
        graph->V++;
    }

    for (int i = 0; i < graph->V; i++) {
        getline(file, line);
        stringstream ss2(line);
        int weight;

        for (int j = 0; j < graph->V; j++) {
            ss2 >> weight;
            if (weight != 0 && j > i && graph->E < 100) {
                graph->edges[graph->E].source = graph->vertices[i];
                graph->edges[graph->E].destination = graph->vertices[j];
                graph->edges[graph->E].weight = weight;
                graph->E++;
            }
        }
    }

    file.close();
    return graph;
}

void free_graph(Graph* graph) {
    if (graph) {
        delete graph;
    }
}

void insertion_sort(Graph* graph) {
    if (!graph) {
        cout << "Граф не создан!" << endl;
        return;
    }
    for (int i = 1; i < graph->E; i++) {
        Edge key = graph->edges[i];
        int j = i - 1;
        while (j >= 0 && graph->edges[j].weight > key.weight) {
            graph->edges[j + 1] = graph->edges[j];
            j--;
        }
        graph->edges[j + 1] = key;
    }
}

void print_graph(Graph* g) {
    if (!g) {
        cout << "Граф не загружен!" << endl;
        return;
    }

    cout << "\nГраф" << endl;
    cout << "Вершин: " << g->V << ", Рёбер: " << g->E << endl;

    for (int i = 0; i < g->V; i++) {
        string vertex = g->vertices[i];
        cout << vertex << " -> ";

        bool has_edges = false;
        for (int j = 0; j < g->E; j++) {
            if (g->edges[j].source == vertex) {
                cout << g->edges[j].destination << "("
                    << g->edges[j].weight << ") ";
                has_edges = true;
            }
        }

        if (!has_edges) cout << "нет рёбер";
        cout << endl;
    }
}

int get_neighbors(Graph* g, string vertex, string neighbors[]) {
    int count = 0;

    for (int i = 0; i < g->E; i++) {
        if (g->edges[i].source == vertex) {
            neighbors[count++] = g->edges[i].destination;
        }
        else if (g->edges[i].destination == vertex) {
            neighbors[count++] = g->edges[i].source;
        }
    }

    return count;
}

void bfs(Graph* g, string start) {
    if (!g) {
        cout << "Граф не загружен!" << endl;
        return;
    }

    int start_idx = find_index(g, start);
    if (start_idx == -1) {
        cout << "Вершина " << start << " не найдена!" << endl;
        return;
    }

    bool visited[26] = { false };
    Queue q;
    string neighbors[26];

    visited[start_idx] = true;
    q.enqueue(start);

    cout << "\nОбход в ширину" << endl;
    cout << "Начинаем с вершины: " << start << endl;
    cout << "Порядок: ";

    while (!q.isEmpty()) {
        string current = q.dequeue();
        cout << current << " ";

        int neighbor_count = get_neighbors(g, current, neighbors);

        for (int i = 0; i < neighbor_count; i++) {
            string neighbor = neighbors[i];
            int idx = find_index(g, neighbor);
            if (idx != -1 && !visited[idx]) {
                visited[idx] = true;
                q.enqueue(neighbor);
            }
        }
    }
    cout << endl;
}

void dfs(Graph* g, string start) {
    if (!g) {
        cout << "Граф не загружен!" << endl;
        return;
    }

    int start_idx = find_index(g, start);
    if (start_idx == -1) {
        cout << "Вершина " << start << " не найдена!" << endl;
        return;
    }

    bool visited[26] = { false };
    Stack s;
    string neighbors[26];

    s.push(start);

    cout << "\nОбход в глубину" << endl;
    cout << "Начинаем с вершины: " << start << endl;
    cout << "Порядок: ";

    while (!s.isEmpty()) {
        string current = s.pop();

        int idx = find_index(g, current);
        if (idx != -1 && !visited[idx]) {
            visited[idx] = true;
            cout << current << " ";

            int neighbor_count = get_neighbors(g, current, neighbors);

            for (int i = neighbor_count - 1; i >= 0; i--) {
                string neighbor = neighbors[i];
                int n_idx = find_index(g, neighbor);
                if (n_idx != -1 && !visited[n_idx]) {
                    s.push(neighbor);
                }
            }
        }
    }
    cout << endl;
}

AdjacencyMatrix* graph_to_adjacency_matrix(Graph* g) {
    if (!g) return nullptr;

    AdjacencyMatrix* adj = new AdjacencyMatrix;
    adj->V = g->V;

    for (int i = 0; i < g->V; i++) {
        adj->vertices[i] = g->vertices[i];
    }

    for (int i = 0; i < g->V; i++) {
        for (int j = 0; j < g->V; j++) {
            adj->matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < g->E; i++) {
        int src_idx = find_index(g, g->edges[i].source);
        int dest_idx = find_index(g, g->edges[i].destination);
        adj->matrix[src_idx][dest_idx] = g->edges[i].weight;
        adj->matrix[dest_idx][src_idx] = g->edges[i].weight;
    }

    return adj;
}

void print_adjacency_matrix(AdjacencyMatrix* adj) {
    if (!adj) {
        cout << "Матрица не создана!" << endl;
        return;
    }

    cout << "\nМатрица смежности" << endl;

    cout << "   ";
    for (int i = 0; i < adj->V; i++) {
        cout << adj->vertices[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < adj->V; i++) {
        cout << adj->vertices[i] << "  ";
        for (int j = 0; j < adj->V; j++) {
            cout << adj->matrix[i][j] << " ";
        }
        cout << endl;
    }
}

AdjacencyList* graph_to_adjacency_list(Graph* g) {
    if (!g) return nullptr;

    AdjacencyList* adj_list = new AdjacencyList;
    adj_list->V = g->V;

    for (int i = 0; i < g->V; i++) {
        adj_list->vertices[i] = g->vertices[i];
        adj_list->list[i] = nullptr;
    }

    for (int i = 0; i < g->E; i++) {
        int src_idx = find_index(g, g->edges[i].source);

        AdjacencyList::ListNode* newNode = new AdjacencyList::ListNode;
        newNode->dest = g->edges[i].destination;
        newNode->weight = g->edges[i].weight;
        newNode->next = adj_list->list[src_idx];
        adj_list->list[src_idx] = newNode;

        int dest_idx = find_index(g, g->edges[i].destination);
        AdjacencyList::ListNode* newNode2 = new AdjacencyList::ListNode;
        newNode2->dest = g->edges[i].source;
        newNode2->weight = g->edges[i].weight;
        newNode2->next = adj_list->list[dest_idx];
        adj_list->list[dest_idx] = newNode2;
    }

    return adj_list;
}

void print_adjacency_list(AdjacencyList* adj_list) {
    if (!adj_list) {
        cout << "Список смежности не создан!" << endl;
        return;
    }

    cout << "\nСписок смежности" << endl;
    for (int i = 0; i < adj_list->V; i++) {
        cout << adj_list->vertices[i] << " -> ";

        AdjacencyList::ListNode* current = adj_list->list[i];
        if (!current) {
            cout << "нет рёбер";
        }
        else {
            while (current) {
                cout << current->dest << "(" << current->weight << ") ";
                current = current->next;
            }
        }
        cout << endl;
    }
}

void free_adjacency_list(AdjacencyList* adj_list) {
    if (!adj_list) return;

    for (int i = 0; i < adj_list->V; i++) {
        AdjacencyList::ListNode* current = adj_list->list[i];
        while (current) {
            AdjacencyList::ListNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete adj_list;
}

IncidenceMatrix* graph_to_incidence_matrix(Graph* g) {
    if (!g) return nullptr;

    IncidenceMatrix* inc = new IncidenceMatrix;
    inc->V = g->V;
    inc->E = g->E;

    for (int i = 0; i < g->V; i++) {
        inc->vertices[i] = g->vertices[i];
    }
    for (int i = 0; i < g->E; i++) {
        inc->edges[i] = g->edges[i];
    }

    for (int i = 0; i < g->V; i++) {
        for (int j = 0; j < g->E; j++) {
            inc->matrix[i][j] = 0;
        }
    }

    for (int j = 0; j < g->E; j++) {
        int src_idx = find_index(g, g->edges[j].source);
        int dest_idx = find_index(g, g->edges[j].destination);

        if (src_idx != -1) inc->matrix[src_idx][j] = g->edges[j].weight;
        if (dest_idx != -1) inc->matrix[dest_idx][j] = g->edges[j].weight;
    }

    return inc;
}

void print_incidence_matrix(IncidenceMatrix* inc) {
    if (!inc) {
        cout << "Матрица инцидентности не создана!" << endl;
        return;
    }

    cout << "\nИнциндентность" << endl;

    cout << "   ";
    for (int j = 0; j < inc->E; j++) {
        cout << inc->edges[j].source << inc->edges[j].destination << " ";
    }
    cout << endl;

    for (int i = 0; i < inc->V; i++) {
        cout << inc->vertices[i] << "  ";
        for (int j = 0; j < inc->E; j++) {
            cout << inc->matrix[i][j] << "  ";
        }
        cout << endl;
    }
}

void graph_representations(Graph* g) {
    if (!g) {
        cout << "Сначала загрузите граф!" << endl;
        return;
    }

    int choice;
    cout << "\nКак запишем?" << endl;
    cout << "1) Матрица смежности" << endl;
    cout << "2) Список смежности" << endl;
    cout << "3) Матрица инцидентности" << endl;
    cout << "4) Все три представления" << endl;
    cout << "Выбор: ";
    cin >> choice;

    switch (choice) {
    case 1: {
        AdjacencyMatrix* adj = graph_to_adjacency_matrix(g);
        print_adjacency_matrix(adj);
        delete adj;
        break;
    }
    case 2: {
        AdjacencyList* adj_list = graph_to_adjacency_list(g);
        print_adjacency_list(adj_list);
        free_adjacency_list(adj_list);
        break;
    }
    case 3: {
        IncidenceMatrix* inc = graph_to_incidence_matrix(g);
        print_incidence_matrix(inc);
        delete inc;
        break;
    }
    case 4: {
        AdjacencyMatrix* adj = graph_to_adjacency_matrix(g);
        AdjacencyList* adj_list = graph_to_adjacency_list(g);
        IncidenceMatrix* inc = graph_to_incidence_matrix(g);

        print_adjacency_matrix(adj);
        print_adjacency_list(adj_list);
        print_incidence_matrix(inc);

        delete adj;
        free_adjacency_list(adj_list);
        delete inc;
        break;
    }
    default:
        cout << "Неверный выбор!" << endl;
    }
}

void make_set(DSU* dsu, Graph* g) {
    dsu->size = g->V;
    for (int i = 0; i < g->V; i++) {
        dsu->parent[i] = g->vertices[i];
        dsu->rank[i] = 0;
    }
}

string find_set(DSU* dsu, Graph* g, string x) {
    int idx = find_index(g, x);
    if (idx == -1) return "";

    if (dsu->parent[idx] != x) {
        dsu->parent[idx] = find_set(dsu, g, dsu->parent[idx]);
    }
    return dsu->parent[idx];
}

void union_sets(DSU* dsu, Graph* g, string x, string y) {
    string rootX = find_set(dsu, g, x);
    string rootY = find_set(dsu, g, y);
    int idxX = find_index(g, rootX);
    int idxY = find_index(g, rootY);

    if (idxX == idxY || idxX == -1 || idxY == -1) return;

    if (dsu->rank[idxX] < dsu->rank[idxY]) {
        dsu->parent[idxX] = rootY;
    }
    else if (dsu->rank[idxX] > dsu->rank[idxY]) {
        dsu->parent[idxY] = rootX;
    }
    else {
        dsu->parent[idxY] = rootX;
        dsu->rank[idxX]++;
    }
}

void dsu_demo(Graph* g) {
    if (!g) {
        cout << "Сначала загрузите граф!" << endl;
        return;
    }

    DSU dsu;
    make_set(&dsu, g);

    cout << "\n DSU" << endl;
    cout << "Исходные множества (каждая вершина отдельно):" << endl;
    for (int i = 0; i < g->V; i++) {
        cout << g->vertices[i] << ": представитель = "
            << find_set(&dsu, g, g->vertices[i]) << endl;
    }
    cout << "\nОбъединяем A и B..." << endl;
    union_sets(&dsu, g, "A", "B");
    cout << "После union(A, B):" << endl;
    for (int i = 0; i < g->V; i++) {
        cout << g->vertices[i] << ": представитель = "
            << find_set(&dsu, g, g->vertices[i]) << endl;
    }

    cout << "\nОбъединяем C и D..." << endl;
    union_sets(&dsu, g, "C", "D");
    cout << "После union(C, D):" << endl;
    for (int i = 0; i < g->V; i++) {
        cout << g->vertices[i] << ": представитель = "
            << find_set(&dsu, g, g->vertices[i]) << endl;
    }

    cout << "\nОбъединяем множества (A,B) и (C,D)..." << endl;
    union_sets(&dsu, g, "A", "C");
    cout << "После union(A, C):" << endl;
    for (int i = 0; i < g->V; i++) {
        cout << g->vertices[i] << ": представитель = "
            << find_set(&dsu, g, g->vertices[i]) << endl;
    }
}

bool compare_edges(Edge a, Edge b) {
    return a.weight < b.weight;
}

void sort_edges_for_kruskal(Graph* g) {
    for (int i = 0; i < g->E - 1; i++) {
        for (int j = 0; j < g->E - i - 1; j++) {
            if (g->edges[j].weight > g->edges[j + 1].weight) {
                Edge temp = g->edges[j];
                g->edges[j] = g->edges[j + 1];
                g->edges[j + 1] = temp;
            }
        }
    }
}

//  Алг Краскала
void kruskal_mst(Graph* g) {
    if (!g) {
        cout << "Сначала загрузите граф!" << endl;
        return;
    }

    if (g->E == 0) {
        cout << "Граф не содержит рёбер!" << endl;
        return;
    }

    sort_edges_for_kruskal(g);

    DSU dsu;
    make_set(&dsu, g);

    Edge result[50];
    int result_size = 0;
    int total_weight = 0;

    cout << "\nАлгоритм Крускала" << endl;
    cout << "Отсортированные рёбра:" << endl;
    for (int i = 0; i < g->E; i++) {
        cout << g->edges[i].source << "-" << g->edges[i].destination
            << " (" << g->edges[i].weight << ")" << endl;
    }

    cout << "\nПостроение мин остового дерева:" << endl;
    for (int i = 0; i < g->E; i++) {
        string u = g->edges[i].source;
        string v = g->edges[i].destination;

        if (find_set(&dsu, g, u) != find_set(&dsu, g, v)) {
            result[result_size++] = g->edges[i];
            total_weight += g->edges[i].weight;
            union_sets(&dsu, g, u, v);

            cout << "Добавляем ребро: " << u << " " << v
                << " (вес: " << g->edges[i].weight << ")" << endl;
        }
        else {
            cout << "Пропускаем ребро:" << u << " " << v << endl;
        }

        if (result_size == g->V - 1) {
            break;
        }
    }

    if (result_size < g->V - 1) {
        cout << "\nНельзя построить остовное дерево." << endl;
        return;
    }

    for (int i = 0; i < result_size - 1; i++) {
        for (int j = 0; j < result_size - i - 1; j++) {
            if (result[j].source > result[j + 1].source ||
                (result[j].source == result[j + 1].source &&
                    result[j].destination > result[j + 1].destination)) {
                Edge temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }

    cout << "\nМинимальное остовое дерево" << endl;
    cout << "Рёбра минимального остового:" << endl;
    for (int i = 0; i < result_size; i++) {
        cout << result[i].source << " " << result[i].destination << endl;
    }
    cout << "Суммарный вес: " << total_weight << endl;
}

void save_graph_to_file(Graph* g, string filename) {
    if (!g) {
        cout << "Граф не загружен!" << endl;
        return;
    }

    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка создания файла!" << endl;
        return;
    }

    for (int i = 0; i < g->V; i++) {
        file << g->vertices[i] << " ";
    }
    file << endl;

    int matrix[50][50] = { 0 };

    for (int i = 0; i < g->E; i++) {
        int src_idx = find_index(g, g->edges[i].source);
        int dest_idx = find_index(g, g->edges[i].destination);
        matrix[src_idx][dest_idx] = g->edges[i].weight;
        matrix[dest_idx][src_idx] = g->edges[i].weight;
    }

    for (int i = 0; i < g->V; i++) {
        for (int j = 0; j < g->V; j++) {
            file << matrix[i][j] << " ";
        }
        file << endl;
    }

    file.close();
    cout << "Граф сохранён в файл: " << filename << endl;
}


int main() {
    setlocale(0, "");
    int choice;
    string filename;
    Graph* graph = nullptr;

    while (true) {
        display_menu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите имя файла: ";
            cin >> filename;
            if (graph) free_graph(graph);
            graph = get_graph_from_file(filename);
            if (graph) {
                cout << "Граф загружен успешно!" << endl;
                cout << "Вершин: " << graph->V << ", Рёбер: " << graph->E << endl;
            }
            break;

        case 2:
            print_graph(graph);
            break;

        case 3:
            if (graph) {
                string start;
                cout << "Введите стартовую вершину для BFS: ";
                cin >> start;
                bfs(graph, start);
            }
            else {
                cout << "Сначала загрузите граф!" << endl;
            }
            break;

        case 4:
            if (graph) {
                string start;
                cout << "Введите стартовую вершину для DFS: ";
                cin >> start;
                dfs(graph, start);
            }
            else {
                cout << "Сначала загрузите граф!" << endl;
            }
            break;

        case 5:
            if (graph) {
                cout << "Исходный граф:" << endl;
                print_graph(graph);
                insertion_sort(graph);
                cout << "\nОтсортированный граф (по весу рёбер):" << endl;
                print_graph(graph);
            }
            else {
                cout << "Сначала загрузите граф!" << endl;
            }
            break;

        case 6:
            graph_representations(graph);
            break;

        case 7:
            dsu_demo(graph);
            break;

        case 8:
            kruskal_mst(graph);
            break;

        case 9:
            if (graph) {
                string save_filename;
                cout << "Введите имя файла для сохранения: ";
                cin >> save_filename;
                save_graph_to_file(graph, save_filename);
            }
            else {
                cout << "Сначала загрузите граф!" << endl;
            }
            break;

        case 0:
            cout << "Выход..." << endl;
            if (graph) free_graph(graph);
            return 0;

        default:
            cout << "Неверный выбор!" << endl;
            break;
        }
    }
}