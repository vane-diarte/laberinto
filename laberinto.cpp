#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <algorithm> // Para std::shuffle
#include <random>    // Para std::default_random_engine

int filas;
int columnas;
char simbolo;

struct Punto {
    int x;
    int y;
    Punto(int x_val, int y_val) {
        x = x_val;
        y = y_val;
    } 
};


void imprimirLaberinto(const std::vector<std::vector<char>>& laberinto) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            std::cout << laberinto[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

// Generar el laberinto usando Backtracking
void generarLaberinto (std::vector<std::vector<char>>& laberinto, int x, int y) {
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };
    std::vector<int> direcciones = { 0, 1, 2, 3 }; // Direcciones posibles (0=izquierda, 1=derecha, 2=arriba, 3=abajo)
    
    // Generador de números aleatorios
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(direcciones.begin(), direcciones.end(), rng);

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[direcciones[i]] * 2;
        int ny = y + dy[direcciones[i]] * 2;

       // Verifica si la nueva posición está dentro de los límites y es una pared
        if (nx >= 0 && nx < columnas && ny >= 0 && ny < filas && laberinto[ny][nx] == simbolo) {

            laberinto[ny][nx] = ' '; //elimina la pared en la nueva posicion
            laberinto[y + dy[direcciones[i]]][x + dx[direcciones[i]]] = ' '; // Elimina la pared intermedia
            generarLaberinto(laberinto, nx, ny); // Llama recursivamente a la función para continuar generando el laberinto
        }
    }
}

// Verificar si un punto es válido para moverse
bool movimientoValido(const std::vector<std::vector<char>>& laberinto, int x, int y, std::vector<std::vector<bool>>& visitado) {
    return x >= 0 && x < columnas && y >= 0 && y < filas && laberinto[y][x] != simbolo && !visitado[y][x];
}

// BFS
bool resolver(std::vector<std::vector<char>>& laberinto) {
    std::queue<Punto> q;
    std::vector<std::vector<bool>> visitado(filas, std::vector<bool>(columnas, false));
    std::vector<std::vector<Punto>> raiz(filas, std::vector<Punto>(columnas, Punto(-1, -1)));

    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    q.push(Punto(1, 0)); // Empieza desde la entrada
    visitado[0][1] = true;

    while (!q.empty()) {
        Punto p = q.front();
        q.pop();

        if (p.x == columnas - 2 && p.y == filas - 1) { // Llegamos a la salida
            while (!(p.x == 1 && p.y == 0)) {
                laberinto[p.y][p.x] = '*';
                p = raiz[p.y][p.x];
            }
            return true;
        }

        for (int i = 0; i < 4; i++) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if (movimientoValido(laberinto, nx, ny, visitado)) {
                q.push(Punto(nx, ny));
                visitado[ny][nx] = true;
                raiz[ny][nx] = p;
            }
        }
    }

    return false;
}

// Función principal
int main() {
    std::srand(std::time(nullptr)); // Semilla para la generación de números aleatorios

    std::cout<<"Ingrese el numero de filas: ";
    std::cin>>filas;

    std::cout<<"Ingrese el numero de columnas: ";
    std::cin>>columnas;

    std::cout<<"Ingrese el simbolo para las paredes: ";
    std::cin>>simbolo;

    // Ajustar las dimensiones a un valor impar para asegurar que siempre haya un camino
    if (filas % 2 == 0) filas++;
    if (columnas % 2 == 0) columnas++;

    std::vector<std::vector<char>> laberinto(filas, std::vector<char>(columnas, simbolo));
    laberinto[1][1] = ' ';
    generarLaberinto(laberinto, 1, 1);

    laberinto[0][1] = 'E';
    laberinto[filas - 1][columnas - 2] = 'S';

    imprimirLaberinto(laberinto);
    std::cout << "Resolviendo laberinto...\n";

    if (resolver(laberinto)) {
        std::cout << "Laberinto resuelto:\n";
        imprimirLaberinto(laberinto);
    } else {
        std::cout << "No hay solucion.\n";
    }

    return 0;
}