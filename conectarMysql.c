/********************************************************
*	Universidad de la Sierra Sur						*
*	Programa en C para manipulación de datos con MySQL 	*
*	@utor: Leonardo Marcos Santiago						*
*	Fecha: 15/dic/2015									*
*********************************************************************************
*	compilar este programa con el comando										*
*	$ gcc -L/usr/lib64/mysql conectarMysql.c -o conectarMysql -lm -lmysqlclient	*
*	ejecutar con: ./conectarMysql												*
* 	En caso de no compilar: instalar $ sudo apt-get install libmysqlclient-dev	*											*
*********************************************************************************
*Base de Datos 																	*
create database Prueba;															*
use Prueba;																		*
create table persona(															*
	nombre varchar(20),															*
    telefono	char(10),														*
    primary key (telefono)														*
);																				*
*********************************************************************************/
//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>	//biblioteca de mysql
//variables constantes
#define HOST "127.0.0.1" 	// Se puede reemplazar con "localhost" 
#define USER "root"			// Usuario del servidor BD
#define PASSWORD "root"			// Contraseña de usuario servidor BD
#define BD "Prueba"			// Nombre BD
//declaración de procedimientos y funciones
void mostrar (MYSQL* conexion, MYSQL_ROW fila, MYSQL_RES *resultado);
void imprimirLista (MYSQL* conexion, MYSQL_ROW fila, MYSQL_RES *resultado);
void insertar (MYSQL* conexion,MYSQL_ROW fila, MYSQL_RES *resultado);
void eliminar(MYSQL* conexion,MYSQL_ROW fila, MYSQL_RES *resultado);
void modificar(MYSQL* conexion,MYSQL_ROW fila, MYSQL_RES *resultado);
int main()
{
	printf("\n*** Bienvenido a conectarMysql. ***\n");
	MYSQL *conexion; 
	MYSQL_ROW fila;			//guarda una tupla de la consulta
	MYSQL_RES *resultado; 	//resultado de la consulta
	//char persona[20], eliminar[20], tel_per[20];
	//char telefono[20], new_tel[20];
	int opcion=1;

	conexion = mysql_init(NULL);
	if (!mysql_real_connect(conexion,HOST,USER,PASSWORD,BD,3306,NULL,0))
	{
		printf("\n%s\n",mysql_error(conexion));
		return 1;
	}
	printf("Conectado a Base de Datos: %s\n",BD);
	//consulta = "SELECT * FROM persona"
	do
	{
		system("clear");//limpiar pantalla
		printf("|---------------------------------------|\n");
		printf("|-----------------Menú------------------|\n");
		printf("| 0- Salir     \t\t\t\t|\n");
		printf("| 1- Mostrar   \t\t\t\t|\n");
		printf("| 2- Insertar  \t\t\t\t|\n");
		printf("| 3- Modificar \t\t\t\t|\n");
		printf("| 4- Eliminar  \t\t\t\t|\n");
		printf("|---------------------------------------|\n");
		printf("Seleccione una opción (0-4):");
		scanf("%d",&opcion);
		switch(opcion)
		{
			case 0: break;
			case 1://mostrar
				mostrar(conexion,fila,resultado);
			break;
			case 2://insertar
				insertar(conexion,fila,resultado);
			break;
			case 3://modificar
				modificar(conexion,fila,resultado);
			break;
			case 4://eliminar
				eliminar(conexion,fila,resultado);
			break;
		}
	}while(opcion != 0);
	system("clear");//limpiar pantalla
	return 0;
}
void mostrar (MYSQL* conexion, MYSQL_ROW fila, MYSQL_RES *resultado)
{
	int salir;
	do
	{
		system("clear");
		imprimirLista(conexion,fila,resultado);
		printf("\n\nEscriba (1) para regresar:");
		scanf("%d",&salir);
	}while(salir != 1);
}
void imprimirLista (MYSQL* conexion, MYSQL_ROW fila, MYSQL_RES *resultado)
{
	system("clear");
	printf("Lista de datos de la Base de Datos %s\n",BD);
		if (mysql_query(conexion,"SELECT * FROM persona")== 0)
		{
			resultado = mysql_use_result(conexion);
			printf("\n---------------------------------");
			printf("\n| Nombre \t| Teléfono\t|\n");
			printf("---------------------------------\n");
			while(fila = mysql_fetch_row(resultado))
			{
				printf("| %s \t",fila[0]);
				printf("| %s \t|\n",fila[1]);
			}
			printf("---------------------------------\n");
			if (!mysql_eof(resultado))
			{
				printf("Error de lectura: [%s]\n",mysql_error(conexion));
			}
		}
}
void insertar (MYSQL* conexion, MYSQL_ROW fila, MYSQL_RES *resultado)
{
	int salir;
	char nombre_per[20],tel_per[10];
	char persona_insertar[100];
	imprimirLista(conexion,fila,resultado);
	printf("Escriba los sigs. datos para insertar en la Base de Datos:\n");
	printf("Nombre:");
	scanf("%s",nombre_per);
	printf("Teléfono:");
	scanf("%s",tel_per);
	sprintf(persona_insertar,"INSERT INTO persona VALUES ('%s','%s')",nombre_per,tel_per);//crea la cadena de inserción
	if (mysql_query(conexion,persona_insertar) == 0)
	{
		do
		{
			system("clear");
			printf("%s con teléfono %s se ha agregado correctamente!\n",nombre_per,tel_per);
			printf("Escriba (1) para regresar: ");
			scanf("%d",&salir);
		}while(salir != 1);
		
	}
}
void modificar(MYSQL* conexion, MYSQL_ROW fila, MYSQL_RES *resultado)
{
	int salir;
	char tel_per[10],nom_per[10],modificacion[250];
	do{
		system("clear");
		imprimirLista(conexion,fila,resultado);
		printf("\nEscriba el teléfono de la persona \nque desea modificar: ");
		scanf("%s",tel_per);
		printf("\nEscriba un nuevo nombre para la persona \ncon teléfono %s: ",tel_per);
		scanf("%s",nom_per);
		sprintf(modificacion,"UPDATE persona SET nombre='%s' WHERE telefono='%s'",nom_per,tel_per);//crea la cadena de modificacion
		if (mysql_query(conexion,modificacion) == 0)
		{
			printf("\nLa persona con teléfono %s se ha modificado con éxito!\n",tel_per);
		}
		printf("Escriba (1) para regresar: ");
		scanf("%d",&salir);
	}while(salir != 1);
}
void eliminar(MYSQL* conexion, MYSQL_ROW fila, MYSQL_RES *resultado)
{
	int salir;
	char tel_per[10],eliminacion[250];
	do{
		system("clear");
		imprimirLista(conexion,fila,resultado);
		printf("\nEscriba el teléfono de la persona \nque desea eliminar: ");
		scanf("%s",tel_per);
		sprintf(eliminacion,"DELETE FROM persona WHERE telefono='%s'",tel_per);//crea la cadena de eliminación
		if (mysql_query(conexion,eliminacion) == 0)
		{
			printf("\nLa persona con teléfono %s se ha eliminado con éxito!\n",tel_per);
		}
		printf("Escriba (1) para regresar: ");
		scanf("%d",&salir);
	}while(salir != 1);
}