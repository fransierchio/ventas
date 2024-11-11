#include "ventas.hpp"

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
