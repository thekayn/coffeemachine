///  funcoes da maquina de lavar

//descricao dos pinos I/O
// ENTRADAS
#define botao_menos PC0 // botao menos
#define botao_mais PC1 // botao mais
#define botao_enter PC2 // botao ENTER / STOP processo
#define sensor_presenca //sensor presença, pra ver se tem copo
//SAIDAS
// FICOU DEFINIDO QUE A PORTA DO PWM DO DISPOSITIVO COMO PD3 (OC2B) UTILIZANDO O TIMER2
// ALTO = LIGA O MOTOR E BAIXA = DESLIGA O MOTOR
// VALVULA = 0 LIGA E = 1 DESLIGA
#define valvula_cha_matte PB4 //dispositivo que influencia a ligação do motor
#define valvula_cha_verde PB3 //valvula de entrada agua
#define valvula_cha_bisco PB2 //valvula do cha de bisco
#define valvula_agua_quente_fria PB5 //valvula de agua quente e agua fria

//#define sensor_cheio PC3 // sensor cheio tanque//
//#define sensor_vazio PC4 // sensor vazio tanque//

//SAIDAS
//#define motor_agitacao PB4 // motor agitacao//
//#define valvula_entrada_agua PB3 // valvula de entrada agua//
//#define bomba_saida PB2 // bomba saida tanque//


//definiçao para acessar a memória flash 
PROGMEM const char msg_bemvindo[] = "Bem vindo        \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg_cha[]= "Preparando o cha   \0";        //mensagem para escolher qual o tamanho do copo
PROGMEM const char msg_copo[]= "Qual copo deseja   \0";        //mensagem para escolher qual o tamanho do copo
PROGMEM const char msg_escolha_sabor[] = "Escolha o sabor    \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg__matte[] = "Matte   \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg__verde[] = "Verde    \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg_bisco[] = "Bisco   \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg_inicializando[] = "Em processo...   \0"; //mensagem armazenada na memoria Flash
//..............................//
PROGMEM const char msg_valvula_agua_quente_fria[] = "Aguá quente ou fria   \0"; //mensagem armazenada na memoria Flash
PROGMEM const char msg_fim_de_processo[] = "Volte Sempre!!   \0";    //mensagem armazenada na memoria Flash
PROGMEM const char msg_pequeno[] = "Copo Pequeno \0"; // mensagem para o nivel do copo
PROGMEM const char msg_medio[] = "Copo Medio        \0";            // mensagem para o nivel do copo
PROGMEM const char msg_grande[] = "Copo Grande        \0";           //  mensagem para o nivel do copo
PROGMEM const char msg_agua_quente[] = "Agua quente  \0";            //mensagem sim
PROGMEM const char msg_agua_fria[] = "Agua fria    \0";            //mensagem nao
PROGMEM const char msg_misturador[] = "Misturando...\0";            //mensagem para misturar o cha
PROGMEM const char msg_insira_o_copo[] ="Insira o copo  \0" ;   //mensagem para insirir o copo
PROGMEM const char msg_copo_inserido[] = "Copo detectado \0";     //mensagem do copo detectado
PROGMEM const char msg_jump_line[] = "\n";                        //mensagem para que pule linha
PROGMEM const char caractere_especial [] = {          // CRIAÇÃO DO CARACTERE ESPECIAL
	                                        0b00000,
											0b01110,
											0b01110,
											0b11111,      //Foi criado um caractere especial dentro do LCD
											0b01010,
											0b01110,
											0b01110};


//inicializar a maquina de cha//

void inicializando()
{
	pressionado=0;                       // variavel declarada na aba MAIN.C, ela serve para mudar o estado da variavel que ira pular os cases
	cmd_LCD(0x80,0);                    //limpa display
	escreve_LCD_Flash(msg_bemvindo);   // imprime mensagem
	// RETIRADO DO LIVRO
	cmd_LCD(0x40,0);                 //ENDEREÇO BASE  PARA GRAVAR O SEGMENTO
	for (X = 0; X<7;X++){           //GRAVA 8 BYTES NA DDRAM COMEÇANDO NA END 0X40
		cmd_LCD(pgm_read_byte(&caractere_especial[X]),1); //CRIA-SE ESSE LOOP PARA GRAVAR AS TODOS OS CARACTERES ESPECIAIS
	}
	cmd_LCD(0xC0,0);
	cmd_LCD(0x00,1);
	if(!tempo) {                // verifica tempo=0
		estado = 1;            //muda de estado
		cmd_LCD(0x01,0);      //função para limpar o display
	}
}

void Tamanho_do_copo()
{
	cmd_LCD(0x80,0); //limpa display
	//.........................................................SEPARAR LOGICA............................................................//
	escreve_LCD_Flash(msg_copo); // imprime mensagem qual o tamanho do copo
	//.........................................................SEPARAR LOGICA............................................................//
	switch(menu){                                  //MENU PARA ESCOLHER QUAL O NIVEL DE COPO DESEJA
		case 0:                                    //SÃO 3 opções
		cmd_LCD(0xC0,0);
		escreve_LCD_Flash(msg_pequeno); // imprime a mensagem no lcd da mensagem do copo pequeno
	//	preco = 10.00;                  //preço do copo pequeno
	//	peso_copo_do_cha = 1000;         //peso_copo_do_cha = 300;
		break;
		
		case 1:
		cmd_LCD(0xC0,0);
		escreve_LCD_Flash(msg_medio);       //IMPRIME MENSAGEM DO COPO MÉDIO
	//	preco = 12.00;                      //preço do copo medio
	//	peso_copo_do_cha = 1400;             //peso_copo_do_cha = 400;
		break;
		
		case 2:
		cmd_LCD(0xC0,0);
		escreve_LCD_Flash(msg_grande);    //IMPRIME MENSAGEM DO COPO GRANDE
	//	preco = 15.00;                    //preço do copo grande
	//	peso_copo_do_cha = 1800;           //peso_copo_do_cha = 600;
		break;
	
	}
	//.........................................................SEPARAR LOGICA............................................................//
	if(!tst_bit(PINC,1)){     //CONDIÇÃO PARA O BOTÃO OPERA DEFINIDO NO PC1
		menu++;
		if(menu == 2){
			menu = 0;
			_delay_ms(300);
		}
		else(!tst_bit(PINC,0)){
			menu--;      // CASO NAO ENTRAR NO PRIMEIRO IF, INCREMENTA O BOTÃO MAIS ATÉ CHEGAR == 3
			_delay_ms(300);
		}
	}
	//.........................................................SEPARAR LOGICA............................................................//
	if(!tst_bit(PINC,2)) { // ENTER
		estado = 2;        //ENCAMINHA PARA O ESTADO 2
		_delay_ms(300);
		cmd_LCD(0x01,0); // LIMPA O LCD
	}
}

void informe_o_sabor(){                 //o usuario precisa escolher o sabor atraves do botao mais e menos
	cmd_LCD(0x80,0);                    //limpa display
	escreve_LCD_Flash(msg_escolha_sabor); //imprime a mensagem
	switch(cha){                          //menu para a escolha dos sabores do cha
		case 0:
		cmd_LCD(0xC0,0);
		escreve_LCD_Flash(msg__matte); //imprimi a mensagem matte com manga
		break;
		
	
		case 1:
		cmd_LCD(0xC0,0);
		escreve_LCD_Flash(msg__verde);  //imprimi a mensagem verde com manga
		break;
		
	
		case 2:
		cmd_LCD(0xC0,0);
		escreve_LCD_Flash(msg_bisco);   //imprimi a mensgem bisco com manga
		break;

}
	if(!tst_bit(PINC,1)){     //CONDIÇÃO PARA O BOTÃO ALTERA DEFINIDO NO PC1
		cha++;
		if(cha == 2){
			cha = 0;
			_delay_ms(300);
		}
		else(!tst_bit(PINC,0)){
			cha--;      // CASO NAO ENTRAR NO PRIMEIRO IF, INCREMENTA O BOTÃO ALTERA MAIS ATÉ CHEGAR == 1
			_delay_ms(300);
		}
	}
	if(!tst_bit(PINC,2)) { // ENTER
		estado = 3;        //ENCAMINHA PARA O ESTADO 3
		_delay_ms(300);
		cmd_LCD(0x01,0); // LIMPA O LCD
	}
}

void informa_agua_quente_fria(){
	cmd_LCD(0x80,0);
	escreve_LCD_Flash(msg_valvula_agua_quente_fria);            // imprime mensagem deseja gelo
	switch(agua_quente_fria){                                  //MENU PARA ESCOLHER SE DESEJA OU NAO GELO EM SEU CHÁ
		case 0:                                   //SÃO 2 opções
		cmd_LCD(0xC0,0);
		escreve_LCD_Flash(msg_agua_quente);             //imprime a mensagem no lcd da mensagem que deseja, que é de agua quente
		break;
		
		case 1:
		cmd_LCD(0xC0,0);
		escreve_LCD_Flash(msg_agua_fria);       //IMPRIME MENSAGEM QUE NAO DESEJA GELO NO CHÁ
		break;
	}
	if(!tst_bit(PINC,1)){     //CONDIÇÃO PARA O BOTÃO MAIS DEFINIDO NO PC1 PARA MUDAR O TIPO DE AGUA
		agua_quente_fria++;
		if(agua_quente_fria == 1){
			agua_quente_fria = 0;
			_delay_ms(300);
		}
		else(!tst_bit(PINC,0)){
			agua_quente_fria--;      // APERTAR O BOTAO MENOS PARA MUDAR O TIPO DE AGUA
			_delay_ms(300);
		}
	}
	if(!tst_bit(PINC,2)) { // ENTER
		estado = 4;        //ENCAMINHA PARA O ESTADO 4
		_delay_ms(300);    //delay do click do botao
		cmd_LCD(0x01,0); // LIMPA O LCD
		tempo = 150;
	}
}

// INICIO DO PROCESSO DA MÁQUINA DE CHÁ, ESTADOS PARA DETERMINAR CADA PARTE

void inicio_do_processo()
{
	cmd_LCD(0x80,0); //limpar display
	
	//.........................................................SEPARAR LOGICA............................................................//
	
	escreve_LCD_Flash(msg_inicializando);  //escreve a msg de inicialização
	
	//.........................................................SEPARAR LOGICA............................................................//
	
	ident_num(tempo/10,digitos);  //divide por 10 antes de separar digitos
	cmd_LCD(0xC5,0);
	cmd_LCD(digitos[2],1);
	cmd_LCD(digitos[1],1);
	cmd_LCD(digitos[0],1);
	
	//.........................................................SEPARAR LOGICA............................................................//
	
	if(!tempo) {
		if(cha == 0){              // se for a variavel cha = 0 ligara a valvula do cha matte
			set_bit(PORTB,valvula_cha_matte); //abre a valvula de cha matte
		}
		if(cha == 1){           // se for a variavel cha = 1 ligara a valvula do cha verde
			set_bit (PORTB,valvula_cha_verde); //abre a valvula de cha verde
		}
		if(cha == 2){       // se for a variavel cha = 2 ligara a valvula do cha de bisco
			set_bit(PORTB,valvula_cha_bisco);  //abre a valvula de cha bisco
		}
		
		//.........................................................SEPARAR LOGICA............................................................//
		
		
		if(menu == 0){                //Menu para realizar a contagem dos copos pequenos
			contador_pequeno = contador_pequeno + 1;
			eeprom_write_word(&copo_pequeno,contador_pequeno); //salva o contador no endereço do copo
		}
		if(menu == 1){                  //Menu para realizar a contagem dos copos  medios
			contador_medio = contador_medio + 1;            //incrementar a contagem dos copos
			eeprom_write_word(&copo_medio,contador_medio);   //salva o contador no endereço do copo
		}
		if(menu == 2){                //Menu para realizar a contagem dos copos  grandes
			contador_grande = contador_grande + 1;        //incrementar a contagem dos copos
			eeprom_write_word(&copo_grande,contador_grande);   //salva o contador no endereço do copo
		}
		
		//.........................................................SEPARAR LOGICA............................................................//
		
		cmd_LCD(0x01,0);
		cmd_LCD(0x80,0); // apaga display
		
		//.........................................................SEPARAR LOGICA............................................................//
		
		estado = 6;                  //avança para o estado 6
		escreve_LCD_Flash(msg_cha);  //imprime a mensagem Preparando cha
	//	escreve_USART_Flash(msg_DO_USART); //imprimi o preço para a parte serial
	//	imprime_usart(preco); //manda o valor da compra do cha para o caixa
		tempo = 400;
	}
	
}

void Parte_cha(){
	if(!tempo){
		clr_bit(PORTB,valvula_cha_matte);
		clr_bit(PORTB,valvula_cha_bisco);       //desliga as valvulas dos 3 tipos de cha
		clr_bit(PORTB,valvula_cha_verde);
		cmd_LCD(0x80,0);
		estado = 7;
		tempo = 400;
	}
}

void adiciona_agua_quente_fria(){
	if(!tempo){
		clr_bit(PORTB,valvula_agua_quente_fria);     //desliga a valvula de agua quente 
		estado = 9; //muda de estado
		cmd_LCD(0x01,0);
		cmd_LCD(0x80,0); //desliga display
		escreve_LCD_Flash(msg_misturador);
		tempo = 1000;                   //define quanto tempo ira girar
		if(cha == 0 || cha == 2 || cha == 4 ){
			_PWM(51);                   //motor gira com 20% de sua velocidade
		}
		if(cha == 1 || cha == 3 || cha == 5){
			_PWM(204);                //motor gira com 80% de sua velocidade
		}
		tempo = 200;
	}
}

void reinicializando()
{
	//valor_ADS = ads1115_readADC_SingleEnded(ADS1115_ADDR_GND, 0, ADS1115_DR_8SPS, ADS1115_PGA_6_144); //faz a leitura do ads
	//valor_pot_peso = valor_ADS;            //le o valor do pot no ads
	if(valor_pot_peso == 0){               //Nesse caso é necessario zerar o potenciometro para passar a mensagem reiniciando
		//zera o valor pot peso para que nao pegue lixo
		estado = 0; // retorna para o estado 0 para dar inicio novamente ao processo
		tempo = 150; // determina um "delay" para a mensagem bem vindo permanecer no LCD quando retorna p/ o estado 0
		cmd_LCD(0x01,0);
		cmd_LCD(0x80,0);
		escreve_LCD_Flash(msg_reiniciando);
		_delay_ms(1000);
		valor_pot_peso = 0;                  //zera os valores de valor de peso, para que nao pegue os valores anteriores
		valor_ADS = 0;                       //Zera o valor de ADS para nao pegar lixo
		menu = 0;                            //zera o menu em caso de repetição da maquina
		agua_quente_fria = 0;                            // mesmo caso do gelo
		cha = 0;                            // zera para iniciar a primeira opção
	}
}

void Temporizacao(){
	TCCR0B = (1<<CS02) | (1<<CS00); // TC0 com prescaler de 1024 a 16MHz
	// CS02 = 1, CS00 = 1 e CS01 = 0 --> PRESCALER DE 1024
	TIMSK0 = 1<<TOIE0;              // Habilita a interrupção do TC0
	TCNT0 = 100;
}

void _PWM (unsigned char valor_PWM){
	TCCR2A = (1<<COM2B1) | (1<<WGM21) | (1<<WGM20);    // ATIVAR O MODO PWM RÁPIDO COM WGM20 = 1 E WGM21 = 1
	// ATIVA TAMBÉM A PORTA OC2B
	TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20);        //ATIVA UM PRESCALER DE 1024
	OCR2B = valor_PWM;
	// Fazendo 20% da tensão equivale a 26 --> valor_PWM = 20% X 255 = 51
	// 2^8 = 255
	// 0% refere-se ao desligamento valor_PWM = 0
	// P/ 80% --> valor_PWM = 80% X 255 = 204
	sei();
}



/*void enchendo();
void agitacao1();
void molho1();
void agitacao2();
void molho2();
void esvaziando();
void centrifuga();
void reinicializando();

void enchendo()
      { 
    	set_bit(PORTB,valvula_entrada_agua);                       //liga valvula de entrada agua limpa NF
        if(!tst_bit(PINC,sensor_cheio))                     // testa sensor cheio
	        {
                clr_bit(PORTB,valvula_entrada_agua);                 // desliga valvula de entrada
                estado=8;                         // muda estdo
                tempo=tempoagitacao1*1000;        // estabelece multiplica atraso de segundos milisegundos em tempo
                cmd_LCD(0xC0,0);                  // posiciona segunda linha LCD
               escreve_LCD_Flash(msg_agitacao1);  // imprime mensagem
	    	}
      }


void agitacao1()
        {
        set_bit(PORTB,motor_agitacao);                    //liga motor agitacao
        if(!tempo)                           //testa tempo=0
             {
             clr_bit(PORTB,motor_agitacao);          // desliga motor agitacao        
             estado=9; tempo=tempomolho1*1000;  //desliga valvula de entrada
             cmd_LCD(0xC0,0);	
	         escreve_LCD_Flash(msg_molho1); 
	         }
        _delay_ms(1); tempo--;
  }	  
  
  void molho1()
      {
    
      }
  
void agitacao2()
      {
         
       }
	  
void molho2()
     {

     }

void esvaziando() {
    
     }
	
void centrifuga()
           { 
          }

void reinicializando()
            {
           
            }

*/
