#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/gint.h>
#include <math.h>
#include "fuentes.h"
#include <stdio.h>
#define PI 3.14159265358979323846

double A = -1, B = -1, C = -1, a = -1, b = -1;
int datos_ingresados = 0;
int ultima_letra = 0;//1=A, 2=B, 3=C, 4=a, 5=b
// Función segura para reiniciar todos los datos
void reiniciar_todo(void) {
    A = -1;
    B = -1;
    C = -1;
    a = -1;
    b = -1;
    datos_ingresados = 0;
}

// Función segura para salir al menú de la calculadora sin bloquear futuras entradas
void salir_programa(void) {
    reiniciar_todo();
    gint_osmenu();
}

// Ventana emergente de error que obliga a oprimir F1 (Borrar) o AC/ON para empezar de nuevo
void mostrar_error(const char* linea1, const char* linea2) {
    while (1) {
        drect_border(30, 65, 366, 176, C_WHITE, 2, C_RED);
        drect(30, 65, 366, 87, C_RED);
        texto_mediano(168, 69, C_WHITE, "ERROR");

        texto_mediano(42, 98, C_BLACK, linea1);
        texto_mediano(42, 122, C_BLACK, linea2);
		texto_mediano(42, 146, C_BLACK, "oprimir [DEL] PARA BORRAR");

        dupdate();
        key_event_t ev = getkey();
        if (ev.key == KEY_EXIT) {
            salir_programa();
        }
        //al presionar [DEL], cierra el error y regresa sin borrar el dato anterior
		 if(ev.key == KEY_EXE || ev.key == KEY_DEL || ev.key == KEY_ACON) {
            return;
        }
    }
}

void mostrar_valor(int x, int y, int color, const char* letra, double val) {
    char buf[32];

    if (val < 0) {
        sprintf(buf, "%s=", letra);
        texto_mediano(x, y, color, buf);
        return;
    }
    int total_cent = (int)(val * 100.0 + 0.5);
    int entero = total_cent / 100;
    int dec = total_cent % 100;

    if (dec == 0) {
        sprintf(buf, "%s= %d", letra, entero);
    } else {
        sprintf(buf, "%s= %d.%02d", letra, entero, dec);
    }
    texto_mediano(x, y, color, buf);
}


double texto_a_numero(const char* str) {
    double resultado = 0.0;
    double decimal = 0.0;
    double divisor = 10.0;
    int es_decimal = 0;

    for (int i = 0; *(str + i) != '\0'; i++) {
        char c = *(str + i);
        if (c == '.') {
            es_decimal = 1;
        } else if (c >= '0' && c <= '9') {
            if (!es_decimal) {
                resultado = resultado * 10.0 + (c - '0');
            } else {
                decimal += (c - '0') / divisor;
                divisor *= 10.0;
            }
        }
    }
    return resultado + decimal;
}

// Ya no usa malloc para evitar fugas de memoria al salir
double pedir_numero(const char* nombre) {
    char texto_buf[20];
    char *texto = texto_buf;
    *texto = '\0';
    int pos = 0;

    while (1) {
        drect(15, 145, 185, 175, C_WHITE);
        drect_border(15, 145, 185, 175, C_NONE, 1, C_BLACK);
        char linea_input[32];
sprintf(linea_input, "%s = %s_", nombre, texto);
texto_mediano(22, 153, C_BLACK, linea_input);
        dupdate();

        key_event_t ev = getkey();
        
        if (ev.key == KEY_EXIT) {
            salir_programa();
            return -1;
        }
//si oprimes DEL cuando no hay números (pos == 0), regresa para elegir otra letra
        if (ev.key == KEY_DEL && pos == 0) {
            return -1;
        }
        
        if (ev.key == KEY_EXE && pos > 0) break;
        if (ev.key == KEY_DEL && pos > 0) {
            pos--;
            *(texto + pos) = '\0';
        }

        if (pos < 12) {
            if (ev.key == KEY_0) *(texto + pos++) = '0';
            else if (ev.key == KEY_1) *(texto + pos++) = '1';
            else if (ev.key == KEY_2) *(texto + pos++) = '2';
            else if (ev.key == KEY_3) *(texto + pos++) = '3';
            else if (ev.key == KEY_4) *(texto + pos++) = '4';
            else if (ev.key == KEY_5) *(texto + pos++) = '5';
            else if (ev.key == KEY_6) *(texto + pos++) = '6';
            else if (ev.key == KEY_7) *(texto + pos++) = '7';
            else if (ev.key == KEY_8) *(texto + pos++) = '8';
            else if (ev.key == KEY_9) *(texto + pos++) = '9';
            else if (ev.key == KEY_DOT) *(texto + pos++) = '.';
            *(texto + pos) = '\0';
        }
    }
    double val = texto_a_numero(texto);
    return (val > 0) ? val : -1;
}

void resolver_triangulo(void) {
    double rad = PI / 180.0;
    double deg = 180.0 / PI;

    if (b > 0 && a < 0) {
        a = 90.0 - b;
    }

    if (A > 0 && B > 0) {
        C = sqrt(A*A + B*B);
        a = atan(A / B) * deg;
        b = 90.0 - a;
    }
    else if (A > 0 && C > 0 && C > A) {
        B = sqrt(C*C - A*A);
        a = asin(A / C) * deg;
        b = 90.0 - a;
    }
    else if (B > 0 && C > 0 && C > B) {
        A = sqrt(C*C - B*B);
        b = asin(B / C) * deg;
        a = 90.0 - b;
    }
    else if (A > 0 && a > 0) {
        b = 90.0 - a;
        B = A / tan(a * rad);
        C = A / sin(a * rad);
    }
    else if (B > 0 && a > 0) {
        b = 90.0 - a;
        A = B * tan(a * rad);
        C = B / cos(a * rad);
    }
    else if (C > 0 && a > 0) {
        b = 90.0 - a;
        A = C * sin(a * rad);
        B = C * cos(a * rad);
    }
}

int main(void) {
    int color_text = C_BLACK;
    reiniciar_todo(); // Siempre arranca limpio al entrar

    while (1) {
        dclear(C_RGB(25,21,30));

       // 1. Sombra gris suave desplazada 2 píxeles (x=72, y=7)
texto_mediano(72, 7, C_RGB(16, 16, 16), "TRIANGULO RECTANGULO");

// 2. Texto principal rojo encima (x=70, y=5)
texto_mediano(70, 5, C_RED, "TRIANGULO RECTANGULO");
	    mostrar_valor(20, 30, color_text, "A", A);
        mostrar_valor(20, 50, color_text, "B", B);
        mostrar_valor(20, 70, color_text, "C", C);
        mostrar_valor(20, 90, color_text, "a", a);
        mostrar_valor(20, 110, color_text, "b", b);

        // Tu triángulo original
        texto_escala(310, 100, C_BLACK,2, "A");
        texto_escala(249, 160, C_BLACK,2, "B");
        texto_escala(239, 85, C_BLACK,2, "C");

        dline(200, 150, 300, 150, C_BLACK);
        dline(300, 50, 300, 150, C_BLACK);
        dline(300, 50, 200, 150, C_BLACK);

        if (datos_ingresados < 2) {
            drect_border(4, 198, 64, 222, C_WHITE, 2, C_BLACK);
            drect_border(70, 198, 130, 222, C_WHITE, 2, C_BLACK);
            drect_border(136, 198, 196, 222, C_WHITE, 2, C_BLACK);
            drect_border(202, 198, 262, 222, C_WHITE, 2, C_BLACK);
            drect_border(268, 198, 328, 222, C_WHITE, 2, C_BLACK);

            texto_escala(28, 204, C_BLACK,2, "A");
            texto_escala(92, 204, C_BLACK,2, "B");
            texto_escala(160, 204, C_BLACK,2, "C");
            texto_escala(224, 204, C_BLACK,2, "a");
            texto_escala(288, 204, C_BLACK,2, "b");
        } 
        else {
            drect_border(4, 196, 73, 224, C_RGB(21,30,24), 2, C_BLACK);
            texto_mediano(10, 206, C_BLACK, "DELET");

            drect_border(327, 196, 396, 224, C_RGB(21,30,24), 2, C_BLACK);
            texto_mediano(332, 206, C_BLACK, "SALIR");
        }

        dupdate();

        key_event_t tecla = getkey();

        if (tecla.key == KEY_EXIT) {
            salir_programa();
            continue;
        }

        if (datos_ingresados < 2) {
            if (tecla.key == KEY_F1 && A < 0) {
                double val = pedir_numero("A");
                if (val > 0) {
                    if (C > 0 && val >= C) {
                        mostrar_error("El lado A debe ser", "menor que hipotenusa C");
                    } else {
                        A = val;
                        datos_ingresados++;
                    }
                }
            }
            else if (tecla.key == KEY_F2 && B < 0) {
                double val = pedir_numero("B");
                if (val > 0) {
                    if (C > 0 && val >= C) {
                        mostrar_error("El lado B debe ser", "menor que hipotenusa C");
                    } else {
                        B = val;
                        datos_ingresados++;
                    }
                }
            }
            else if (tecla.key == KEY_F3 && C < 0) {
                double val = pedir_numero("C");
                if (val > 0) {
                    if ((A > 0 && val <= A) || (B > 0 && val <= B)) {
                        mostrar_error("Hipotenusa C debe ser", "mayor que los catetos");
                    } else {
                        C = val;
                        datos_ingresados++;
                    }
                }
            }
            else if (tecla.key == KEY_F4 && a < 0) {
                if (b > 0) {
                    mostrar_error("Ya ingresaste angulo b", "Ingresa un lado (A,B,C)");
                } else {
                    double val = pedir_numero("a");
                    if (val > 0) {
                        if (val >= 90.0) {
                            mostrar_error("El angulo a debe ser", "menor a 90 grados");
                        } else {
                            a = val;
                            datos_ingresados++;
                        }
                    }
                }
            }
            else if (tecla.key == KEY_F5 && b < 0) {
                if (a > 0) {
                    mostrar_error("Ya ingresaste angulo a", "Ingresa un lado (A,B,C)");
                } else {
                    double val = pedir_numero("b");
                    if (val > 0) {
                        if (val >= 90.0) {
                            mostrar_error("El angulo b debe ser", "menor a 90 grados");
                        } else {
                            b = val;
                            datos_ingresados++;
                        }
                    }
                }
            }

            if (datos_ingresados == 2) {
                resolver_triangulo();
            }
        } 
        else {
            if (tecla.key == KEY_F1 || tecla.key == KEY_ACON) {
                reiniciar_todo();
            }
            else if (tecla.key == KEY_F6) {
                salir_programa();
                continue;
            }
        }
    }
    return 0;
}
