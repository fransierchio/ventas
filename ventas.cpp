#include <iostream>
#include <string.h>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream> // Para manejar archivos CSV

using namespace std;

class Producto
{
private:
    string nombre;
    float precio;
    int existencia;

public:
    Producto(string nombre, float precio, int existencia) : nombre(nombre), precio(precio), existencia(existencia) {}
    void setPrecio(float precio) { this->precio = precio; }
    void setNombre(string nombre) { this->nombre = nombre; }
    void setExistencia(int existencia) { this->existencia = existencia; }
    string getNombre() { return nombre; }
    float getPrecio() { return precio; }
    int getExistencia() { return existencia; }
    void comprado() { existencia--; }
};

class Compras
{
public:
    vector<int> productosComprados;
    string fecha;
    Compras(vector<int>& productos, string& fecha) : productosComprados(productos), fecha(fecha) {}
};

class Sistema
{
private:
    vector<Producto> productos;
    vector<Compras> ventas;

public:
    // Cargar productos desde el archivo
    void cargarProductos() {
        ifstream archivo("productos.txt");
        string nombre, linea;
        float precio;
        int existencia;

        if (archivo.is_open()) {
            productos.clear();
            while (getline(archivo, linea)) {
                stringstream ss(linea);
                getline(ss, nombre, ',');
                ss >> precio;
                ss.ignore(1); // Ignorar la coma
                ss >> existencia;
                productos.emplace_back(nombre, precio, existencia);
            }
            archivo.close();
        } else {
            // Si no existe el archivo, se crea un archivo nuevo
            cout << "No se encontró el archivo de productos. Cargando productos por defecto." << endl;
            crearProductosPorDefecto();
        }
    }

    // Crear productos por defecto si el archivo no existe
    void crearProductosPorDefecto() {
        // Cargar productos por defecto en caso de que no haya archivo
        productos.emplace_back("Jamon", 10, 1);
        productos.emplace_back("Queso", 20, 3);
        productos.emplace_back("Pan", 11, 1);
        productos.emplace_back("Seven Up", 12, 0);
        productos.emplace_back("Coca-cola", 13, 3);
        productos.emplace_back("Cafe", 14, 4);
        productos.emplace_back("Azucar", 15, 6);
        productos.emplace_back("Sal", 16, 3);
        productos.emplace_back("Harina Pan", 17, 1);
        productos.emplace_back("Harina de Trigo", 18, 0);
    }

    // Guardar productos en el archivo
    void guardarProductos() {
        ofstream archivo("productos.txt");

        if (archivo.is_open()) {
            for (auto& producto : productos) {
                archivo << producto.getNombre() << "," 
                        << producto.getPrecio() << "," 
                        << producto.getExistencia() << endl;
            }
            archivo.close();
            cout << "Productos guardados exitosamente." << endl;
        } else {
            cout << "Error al guardar productos." << endl;
        }
    }

    // Cargar ventas desde el archivo
    void cargarVentas() {
    ifstream archivo("ventas.txt");
    string fecha, linea;
    
    if (archivo.is_open()) {
        ventas.clear();
        
        while (getline(archivo, fecha)) {  // Leemos la fecha de cada venta
            getline(archivo, linea);  // Leemos la línea de productos
            
            vector<int> productosComprados;
            stringstream ss(linea);
            string nombreProducto;

            while (getline(ss, nombreProducto, ',')) {  // Leemos los productos separados por coma
                if (!nombreProducto.empty()) {
                    // Buscar el índice del producto en la lista de productos
                    for (int i = 0; i < productos.size(); i++) {
                        if (productos[i].getNombre() == nombreProducto) {
                            productosComprados.push_back(i);
                            break;  // Salir del bucle una vez que encontramos el producto
                        }
                    }
                }
            }

            // Almacenamos la venta con los productos comprados y la fecha
            ventas.emplace_back(productosComprados, fecha);
        }
        archivo.close();
    } else {
        cout << "No se encontró el archivo de ventas. Cargando ventas por defecto." << endl;
    }
}


    // Guardar ventas en el archivo
    void guardarVentas() {
    ofstream archivo("ventas.txt");

    if (archivo.is_open()) {
        for (auto& compra : ventas) {
            archivo << compra.fecha << "\n";  // Guardamos la fecha de la venta
            for (int i = 0; i < compra.productosComprados.size(); i++) {
                archivo << productos[compra.productosComprados[i]].getNombre();
                if (i < compra.productosComprados.size() - 1) {
                    archivo << ",";  // Separar productos con coma
                }
            }
            archivo << "\n";  // Nueva línea para la siguiente venta
        }
        archivo.close();
        cout << "Ventas guardadas exitosamente." << endl;
    } else {
        cout << "Error al guardar ventas." << endl;
    }
}


    // Métodos para gestionar las compras y ventas
    void ordenar() {
        for (int i = 0; i < productos.size() - 1; i++) {
            for (int j = 0; j < productos.size() - i - 1; j++) {
                if (productos[j].getPrecio() < productos[j + 1].getPrecio()) {
                    swap(productos[j], productos[j + 1]);
                }
            }
        }
    }

    float factura(vector<int>& productosComprados) {
        float total = 0;
        for (int i = 0; i < productosComprados.size(); i++) {
            cout << productos[productosComprados[i]].getNombre() << ": " << productos[productosComprados[i]].getPrecio() << endl;
            total = total + productos[productosComprados[i]].getPrecio();
        }
        return total;
    }

    string obtenerFechaActual() {
        time_t now = time(0);
        tm* ltm = localtime(&now);

        string fecha = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
        return fecha;
    }

    void compraCliente() {
        int num;
        float total;
        vector<int> productosComprados;

        do {
            cout << "Cual producto deseas comprar: " << endl;
            for (int i = 0; i < productos.size(); i++) {
                if (productos[i].getExistencia() <= 0) {
                    cout << i + 1 << ") " << productos[i].getNombre() << ": SIN EXISTENCIAS" << endl;
                } else {
                    cout << i + 1 << ") " << productos[i].getNombre() << ": " << productos[i].getPrecio() << "$" << endl;
                }
            }
            cout << "0) Salir" << endl;
            cin >> num;
            system("cls");

            // Validación errores
            if (cin.fail() || num < 0 || num > productos.size()) {
                cin.clear();
                cin.ignore();
                cout << "Opcion invalida, por favor ingresa un numero valido." << endl;
                system("cls");
                continue;
            }

            if (num == 0) break;

            num--;

            if (productos[num].getExistencia() > 0) {
                productos[num].comprado();
                productosComprados.push_back(num);
                cout << "Producto '" << productos[num].getNombre() << "' comprado con exito." << endl;
            } else {
                cout << "Producto sin existencias" << endl;
            }

            cout << endl;

        } while (num != -1);

        if (!productosComprados.empty()) {
            total = factura(productosComprados);
            string fecha = obtenerFechaActual();
            ventas.emplace_back(productosComprados, fecha);

            cout << "El monto total es de: " << total << "$" << endl;
        } else {
            cout << "No se ha realizado ninguna compra." << endl;
        }

        cin.ignore();
        cout << "Presiona Enter para continuar...";
        cin.get();
        system("cls");
    }

    void masVendido() {
        if (ventas.empty()) {
            cout << "No se han realizado compras." << endl;
            return;
        }

        vector<int> contador(productos.size(), 0);

        for (int i = 0; i < ventas.size(); i++) {
            for (int j = 0; j < ventas[i].productosComprados.size(); j++) {
                contador[ventas[i].productosComprados[j]]++;
            }
        }

        int max = 0;
        int indice = -1;
        for (int i = 0; i < contador.size(); i++) {
            if (contador[i] > max) {
                max = contador[i];
                indice = i;
            }
        }

        if (indice != -1) {
            cout << "El producto mas vendido fue " << productos[indice].getNombre()
                 << " con " << max << " ventas." << endl;
        }

        cin.ignore();
        cout << "Presiona Enter para continuar...";
        cin.get();
        system("cls");
    }

    void ventasPorDia() {
        string fechaBusqueda;
        cout << "Introduce la fecha (formato dd/mm/yyyy): ";
        cin >> fechaBusqueda;
        system("cls");
        cout << "Ventas del dia " << fechaBusqueda << ":" << endl;

        bool ventasEncontradas = false;

        for (int i = 0; i < ventas.size(); i++) {
            if (ventas[i].fecha == fechaBusqueda) {
                ventasEncontradas = true;

                float totalVenta = 0;

                for (int j = 0; j < ventas[i].productosComprados.size(); j++) {
                    int indiceProducto = ventas[i].productosComprados[j];
                    cout << productos[indiceProducto].getNombre() << ": " << productos[indiceProducto].getPrecio() << "$" << endl;
                    totalVenta += productos[indiceProducto].getPrecio();
                }

                cout << "Monto total de la venta: " << totalVenta << "$" << endl;
                cout << "----------------------------" << endl;
            }
        }

        if (!ventasEncontradas) {
            cout << "No se encontraron ventas para la fecha " << fechaBusqueda << "." << endl;
        }

        cin.ignore();
        cout << "Presiona Enter para continuar...";
        cin.get();
        system("cls");
    }

    void masCaro() {
        Producto productoCaro = productos.front();

        for (auto& producto : productos) {
            if (producto.getPrecio() > productoCaro.getPrecio()) {
                productoCaro = producto;
            }
        }

        cout << "El producto mas caro es: " << productoCaro.getNombre() << ": "
             << productoCaro.getPrecio() << " $" << endl;
        cin.ignore();
        cout << "Presiona Enter para continuar...";
        cin.get();
        system("cls");
    }
};

int main()
{
    Sistema negocio;

    // Cargar productos y ventas desde archivo
    negocio.cargarProductos();
    negocio.cargarVentas();

    char opc;
    do
    {
        cout << "1) Registrar Venta" << endl
             << "2) Ventas por dia" << endl
             << "3) Producto mas vendido" << endl
             << "4) Producto mas caro" << endl
             << "0) salir" << endl;
        cin >> opc;

        switch (opc)
        {
        case '0':
            // Guardar productos y ventas antes de salir
            negocio.guardarProductos();
            negocio.guardarVentas();
            cout << "Gracias por usar nuestro sistema" << endl;
            break;
        case '1':
            system("cls");
            negocio.compraCliente();
            break;
        case '2':
            negocio.ventasPorDia();
            break;
        case '3':
            negocio.masVendido();
            break;
        case '4':
            negocio.masCaro();
            break;

        default:
            cout << "Opcion invalida" << endl;
            break;
        }

    } while (opc != '0');

    return 0;
}
