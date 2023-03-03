
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 }; //physical mac address
byte ip[] = { 
  192, 168, 0, 2 }; // ip in lan
byte gateway[] = { 
  192, 168, 0, 1 }; // internet access via router
byte subnet[] = { 
  255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString;

int pin[8];
char* nome_pin[8];
int estado_pin[8];
int tipo_pin[8];

//////////////////////

void setup(){

  //CONFIGURACAO DAS PORTAS EM QUE OS RELES ESTAO CONECTADOS:
  //Lembre-se tambem que Ã© necessario utilizar um Arduino Ethernet
  //OU um Arduino UNO + Shield Ethernet

  pin[0] = 2; //Luz do Quarto 
  pin[1] = 3; //Luz da Varanda
  pin[2] = 4; //Estande LED
  pin[3] = 5; //Ventilador 
  pin[4] = 6; // AUX 1
  pin[5] = 7; // AUX 2
  pin[6] = 8; // AUX 3
  pin[7] = 9; // AUX 4

  //NOME DOS BOTOES
  nome_pin[0] = "Quarto";
  nome_pin[1] = "Varanda";
  nome_pin[2] = "Ventilador";
  nome_pin[3] = "rele 4";
  nome_pin[4] = "rele 5";
  nome_pin[5] = "rele 6";
  nome_pin[6] = "rele 7";
  nome_pin[7] = "rele 8";

//TIPO DOS BOTOES 0-> toggle, 1-> pulse
  tipo_pin[0] = 0;
  tipo_pin[1] = 0;
  tipo_pin[2] = 0;
  tipo_pin[3] = 0;
  tipo_pin[4] = 0;
  tipo_pin[5] = 0;
  tipo_pin[6] = 0;
  tipo_pin[7] = 0;

  //ESTADO INICIAL DOS BOTOES 0 -> desligado, 1 -> ligado:
  estado_pin[0] = 0;
  estado_pin[1] = 0;
  estado_pin[2] = 0;
  estado_pin[3] = 0;
  estado_pin[4] = 0;
  estado_pin[5] = 0;
  estado_pin[6] = 0;
  estado_pin[7] = 0;

  pinMode(pin[0], OUTPUT);
  pinMode(pin[1], OUTPUT);
  pinMode(pin[2], OUTPUT);
  pinMode(pin[3], OUTPUT);
  pinMode(pin[4], OUTPUT);
  pinMode(pin[5], OUTPUT);
  pinMode(pin[6], OUTPUT);
  pinMode(pin[7], OUTPUT);



  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //the pin for the servo co
  //enable serial data print
  Serial.begin(9600);
  Serial.println("RoboCore Remote Automation V2.1"); // so I can keep track of what is loaded

}

void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string
          readString += c;
          //Serial.print(c);
        }

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////////// control arduino pin
          Serial.println(readString); //print to serial monitor for debuging

            char pesquisa[] = "?xx";
          for(int i=2 ; i <= 9 ; i++){
            pesquisa[2] = i + 48;

            pesquisa[1] = 'l';
            if(readString.indexOf(pesquisa) > 0){
              if(tipo_pin[i-2] == 1){
                digitalWrite(pin[i-2], HIGH);
                Serial.print("Rele");
                Serial.print(i);
                Serial.println("On");
                delay(200);
                digitalWrite(pin[i-2], LOW);
                Serial.print("Rele");
                Serial.print(i);
                Serial.println("Off");
              } else {
              digitalWrite(pin[i-2], HIGH);
                Serial.print("Rele");
                Serial.print(i);
                Serial.println("On");
              estado_pin[i-2] = 1;
              }
            }

            pesquisa[1] = 'd';
            if(readString.indexOf(pesquisa) > 0){
              digitalWrite(pin[i-2], LOW);
                Serial.print("Rele");
                Serial.print(i);
                Serial.println("Off");
              estado_pin[i-2] = 0;
            }

          }



          //clearing string for next read
          readString="";


          ///////////////

          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<html>");
          client.println("<head>");
          client.println("<title>RoboCore - Remote Automation</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          client.println("<link rel='stylesheet' type='text/css' href='https://api.gabrielsanzone.app/automacaoArduino/main.css' />");
          client.println("<script type='text/javascript' src='https://api.gabrielsanzone.app/automacaoArduino/main.js'/></script>");

          client.println("</head>");
          client.println("<body>");
          client.println("<div id='wrapper'>RoboCore Remote Automation V2.1<br>");

          client.println("<div style='display: inline-block; width: 400px;'>");

          for(int i=0;i<=7;i++){
            client.print("<div id='porta");
            client.print(i+2);
            client.print("_estado'>");
            client.print(estado_pin[i]);
            client.println("</div>");
            client.print("<div id='porta");
            client.print(i+2);
            client.print("_titulo'>");
            client.print(nome_pin[i]);
            client.println("</div>");
            client.print("<div id='porta");
            client.print(i+2);
            client.println("_botao' style='position: relative;'></div>");

            if(i==3){
              client.println("</div><div style='display: inline-block; width: 400px;'>");
            }
          }
          client.println("</div>");

          client.println("</div>");

          client.println("</body>");

          client.println("<script>VerificaEstado();</script>");

          client.println("</html>");


          delay(1);
          //stopping client
          client.stop();

        }
      }
    }
  }
}