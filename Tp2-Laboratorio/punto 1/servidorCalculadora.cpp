
#include "calculadora.h"

int* sumar_1_svc(Operandos* operandos, struct svc_req* req) {
	static int result;	
	result = operandos->operando1 + operandos->operando2;	
	return &result;
}

int* restar_1_svc(Operandos* operandos, struct svc_req* req) {
	static int result;	
	result = operandos->operando1 - operandos->operando2;	
	return &result;
}

int* multiplicar_1_svc(Operandos* operandos, struct svc_req* req) {
	static int result;	
	result = operandos->operando1 * operandos->operando2;	
	return &result;
}
///aca iba double
double* dividir_1_svc(Operandos* operandos, struct svc_req* req) {
	static double result;	
	result = operandos->operando1 / operandos->operando2;	
	return &result;
}


