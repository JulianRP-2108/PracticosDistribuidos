#include <iostream>
#include "calculadora.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 4) {
		cerr << "Coso pone cliente_calculadora despues el <host>  y los operandos capo/a<op1> <op2>" << endl;
		return 1;
	}
	char *server = argv[1];

	CLIENT *clnt = clnt_create(server, CALCULADORA, CALCULADORAVERS, "tcp");//esto devuelve un stub
          //stub:es una estructura que contiene la  información que necesita la biblioteca de rpc
           //parametros  nombre del host servidor,
          //dos constantes: el nombre que esta despes de PROGRAM(EN EL FICHJERO XDR)
           // Y la VERSION  del fichero .x.
           //el ultimo parametro indica el protocola de trasporte.
	if (clnt == NULL) {
		cerr << "Error al crear un cliente de rpc" << endl;
		clnt_pcreateerror(server);
		return 1;
	}
	
	Operandos op;
	op.operando1 = atoi(argv[2]);
	op.operando2 = atoi(argv[3]);
	
	int* resultadoEntero;
	double* resultadoFloat;
	
	
	cout << "Procesando la suma de " << op.operando1 << " y " << op.operando2 << endl;
	resultadoEntero = sumar_1(&op, clnt);

	if (resultadoEntero != NULL) 
           {
		cout << *resultadoEntero << endl;
	    } 
         else
             {
		cerr << "Error al realizar la suma !" << endl;
		clnt_perror(clnt, server);
	      }
	
	cout << "Procesando la resta de " << op.operando1 << " y " << op.operando2 << endl;
	resultadoEntero = restar_1(&op, clnt);

	if (resultadoEntero != NULL) 
          {
		cout << *resultadoEntero << endl;
	  } 
        else 
           {
		cerr << "Error al realizar la resta!" << endl;
		clnt_perror(clnt, server);
	     }
	
	cout << "Procesando la multiplicación de " << op.operando1 << " y " << op.operando2 << endl;
	resultadoEntero = multiplicar_1(&op, clnt);

	if (resultadoEntero != NULL) {
		cout << *resultadoEntero << endl;
	} else {
		cerr << "Error al realizar la multiplicacion !" << endl;
		clnt_perror(clnt, server);
	}
	
	cout << "Procesando la división de " << op.operando1 << " y " << op.operando2 << endl;
	resultadoFloat = dividir_1(&op, clnt);

	if (resultadoFloat != NULL) 
            {
	    cout << *resultadoFloat << endl;
	    }
         else 
             {
		cerr << "Error al realizar la division !" << endl;
		clnt_perror(clnt, server);
	       }
	
	
	
	return 0;
}
