#define R 9//Define a saída vermelha como pino 9 (PWM)

#define G 10//Define a saída verde como pino 10 (PWM)

#define B 11 //Define a saída azul como pino 11 (PWM)

int LDR; //Variável para a leitura do LDR

int cont; //Variável utilizada para armazenar os valores lidos pelo sensor

int i; //Variável para contagem

void setup()

{

  pinMode(R,OUTPUT);//Define o pino 9 como saída

  pinMode(G,OUTPUT);//Define o pino 10 como saída

  pinMode(B,OUTPUT); //Define o pino 11 como saída

  Serial.begin(9600); //Inicia a serial

}

void loop()

{

  LDR=0; //Zera a variável LDR

  for(i=0;i<=10;i++) //Faz 10 vezes a leitura do sensor

  {

  cont=analogRead(A0); //Lê o valor do sensor (LDR ligado ao pino A0) e guarda na variável LDR

  LDR = LDR+cont; //Armazenamento na varíavel LDR o valor lido + o valor anterior

  delay(10); //Delay se 10 milissegundos

  }

  LDR=LDR/10; //Divide o valor armazenado por 10

  Serial.println(LDR); //Imprime o valor do LDR

  (LDR <= 1100) ? (analogWrite(B,100)) : (analogWrite(B,0));

  (LDR <= 1100) ? (analogWrite(R,80)) : (analogWrite(R,0));

 	//Se o valor lido (luminosidade) for maior ou igual a 1100, liga a fita na cor estabelecida através dos valores analógicos, senão desliga.

}