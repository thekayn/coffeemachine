//------------------------------------------------------------------------------------- //
//		AVR e Arduino: Técnicas de Projeto, 2a ed. - 2012.								//
//------------------------------------------------------------------------------------- //
//=====================================================================================	//
//		MAQUINA DE CHA FUNCOES DE MENU    							//
//																						//
//		LCD Interface de dados de 4 bits
//=====================================================================================	//

//vamos ter que usar 3 sabores diferentes//
//agua quente e fria//
//sensor de presença//
//copo pequeno, medio e grande//
//botoes mais, menos e enter//

#include "def_principais.h"			//inclusao do arquivo com as principais definicoes
#include "LCD.h"   // biblioteca .c e .h LCD display 16x2

// variaveis
unsigned int tempo=0, pressionado=0, contador_pequeno = 0, contador_medio = 0, contador_grande = 0, presenca=0, peso_copo_do_cha = 0;

unsigned char  estado=0, menu = 0, cha=0, agua_quente_fria = 0;	//declara variavel global

#include "funcoes_maquina_de_cha.h"


//--------------------------------------------------------------------------------------
int main()
{	unsigned char digitos[tam_vetor];	//declaracao da variável para armazenagem dos digitos

	DDRB = 0b00111111;			//PORT B saida
	PORTB= 0;		        	//inicia desligado
	DDRC = 0b00000000;			//PORT C entrada
    PORTC= 0b11111111;          //PULL UP portC
	DDRD = 0xFF;				//PORTD como saída (display)
	PORTD= 0xFF;				//desliga o display
	UCSR0B = 0x00;				//PD0 e PD1 como I/O genérico, para uso no Arduino
	DDRC= 0b00000000;           // saidas motor e valvulas
	
	inic_LCD_4bits();				//inicializa o LCD
	cmd_LCD(0x01,0); 				//desloca cursor para a segunda linha
	
	tempo=1000;   // varial de contagem de tempo
	while(1) 					//laco infinito
	{
		
		switch(estado)
		{  //estado inicial
		    case 0:                      pressionado=0;   // variavel
		     cmd_LCD(0x80,0);  //limpa display
		     escreve_LCD_Flash(msg_bemvindo);  // imprime mensagem
		 	  if(!tempo) {    // vefica tempo=0
				 estado=1;  //muda de estado
				 cmd_LCD(0x01,0);  // limpa display
	    	  	 }
			 _delay_ms(1);  // atraso
			 tempo--;    // decrementa tempo
			break;
			case 1:                     cmd_LCD(0x80,0);
			escreve_LCD_Flash(msg_agitacao1); // imprime mensagem
			ident_num(tempoagitacao1,digitos);  // separa digitos
			cmd_LCD(0xC5,0);	//posiciona lcd
			cmd_LCD(digitos[2],1); //imprime centena
			cmd_LCD(digitos[1],1);  //imprime dezena
			cmd_LCD(digitos[0],1); //imprime unidade
			cmd_LCD(0xC9,0);  //posiciona lcd
			cmd_LCD('<',1);  //imprime
			cmd_LCD('E',1);   //imprime
			cmd_LCD('N',1);   //imprime
			cmd_LCD('T',1);   //imprime
			cmd_LCD('E',1);   //imprime
			cmd_LCD('R',1);   //imprime
			cmd_LCD('>',1);	  //imprime
			if(!tst_bit(PINC,0)) { tempoagitacao1++; _delay_ms(100);}  //if botao+
			if(!tst_bit(PINC,1)) { tempoagitacao1--; _delay_ms(100); }  //if botao-
			if(!tst_bit(PINC,2)) { estado=2;  _delay_ms(300); cmd_LCD(0x01,0); } break; // ENTER
			
			case 2:   cmd_LCD(0x80,0); 	escreve_LCD_Flash(msg_molho1); // imprime mensagem
			ident_num(tempomolho1,digitos); // separa digitos
			cmd_LCD(0xC5,0);	//posiciona lcd
			cmd_LCD(digitos[2],1);
			cmd_LCD(digitos[1],1);
			cmd_LCD(digitos[0],1);
			cmd_LCD(0xC9,0);
			cmd_LCD('<',1);
			cmd_LCD('E',1);
			cmd_LCD('N',1);
			cmd_LCD('T',1);
			cmd_LCD('E',1);
			cmd_LCD('R',1);
			cmd_LCD('>',1);
			if(!tst_bit(PINC,0)) { tempomolho1++; _delay_ms(100);} //if botao+
			if(!tst_bit(PINC,1)) { tempomolho1--; _delay_ms(100); } //if botao-
			if(!tst_bit(PINC,2)) { estado=3;  _delay_ms(300); cmd_LCD(0x01,0); } break; //Enter
			
			case 3:   cmd_LCD(0x80,0); 	escreve_LCD_Flash(msg_agitacao2);
			ident_num(tempoagitacao2,digitos);
			cmd_LCD(0xC5,0);			//posiciona lcd
			cmd_LCD(digitos[2],1);
			cmd_LCD(digitos[1],1);
			cmd_LCD(digitos[0],1);
			cmd_LCD(0xC9,0);
			cmd_LCD('<',1);
			cmd_LCD('E',1);
			cmd_LCD('N',1);
			cmd_LCD('T',1);
			cmd_LCD('E',1);
			cmd_LCD('R',1);
			cmd_LCD('>',1);
			if(!tst_bit(PINC,0)) { tempoagitacao2++; _delay_ms(100);}
			if(!tst_bit(PINC,1)) { tempoagitacao2--; _delay_ms(100); }
			if(!tst_bit(PINC,2)) { estado=4;  _delay_ms(300); cmd_LCD(0x01,0); } break; // enter
			
			case 4:    break; // enter

			case 5:    break; //enter
			//// iniciacao dos processos
			
			case 6:  cmd_LCD(0x80,0); 	escreve_LCD_Flash(msg_inicializando);
			ident_num(tempo/10,digitos);  //divide por 10 antes de separar digitos
			cmd_LCD(0xC5,0);
			cmd_LCD(digitos[2],1); //
			cmd_LCD(digitos[1],1);
			cmd_LCD(digitos[0],1);
			tempo--;
			_delay_ms(1);
			if(!tempo) {  estado=7; cmd_LCD(0xC0,0); escreve_LCD_Flash(msg_enchendo); }  break;

			
			case 7:  enchendo();  break;  // enchendo
			case 8:  agitacao1(); break; //agitacao 1
			case 9:  molho1();  break; // molho 1
			case 10: agitacao2(); break; //agitacao 2
			case 11: molho2();  break; // molho2
			case 12: esvaziando(); break;  // esvaziando
			case 13: centrifuga();  break; // centrifuga
			case 14: reinicializando(); break; // reinicializando

			//  ........
		}   // fim switch
		
		if(estado>5)  // testes de estados emergencia tilizando botao ENTER a partir do estado 5
		{
			if(!tst_bit(PINC,2))  // verifica se botao enter pressionado
			{ pressionado++;  // incrementa variavel pressionado
				if(pressionado>3000)  // compara se ultrapassou o valor limit, botao pressionado por longo periodo
				{ estado=14; }   // direciona para estado 14
			} else
			{
				pressionado=0;  // botao nao pressionado  zera valor
			}  // fim if botao pressionado
		} // fim if teste de estados emergencia
		
	}// fim do while
	
} // fim do main
//======================================================================================


