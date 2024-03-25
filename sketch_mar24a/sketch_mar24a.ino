#include <ESP8266WiFi.h>

 

const char* ssid = "NOM WIFI";  //entrez le nom du wifi  

const char* password = "MOT DE PASSE WIFI";   //entrez le mot de passe du wifi

 

int LAMPE = 5;   // LAMPE connectée au GPIO5 (D1)


WiFiServer server(80);

 

void setup()

{

  Serial.begin(115200); 

  pinMode(LAMPE, OUTPUT);

  digitalWrite(LAMPE, LOW);

 

  Serial.print("Connexion au réseau");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)

  {

    delay(500);

    Serial.print(".");

  }

  Serial.println("WiFi connecté"); 

  server.begin();  // Démarre le serveur

  Serial.println("Serveur démarré");

 

  Serial.print("Adresse IP: "); 

  Serial.println(WiFi.localIP());

  Serial.print("Copiez et collez l'URL suivante: https://"); // affiche l'adresse IP au format URL

  Serial.print(WiFi.localIP());

  Serial.println("/");

}

 

void loop()

{

  WiFiClient client = server.available();

  if (!client)

  {

    return;

  }

  Serial.println("En attente d'un nouveau client");

  while(!client.available())

  {

    delay(1);

  }

 

  String request = client.readStringUntil('\r');

  Serial.println("requête: "+request);

  client.flush();

 

  int value = LOW;

  if(request.indexOf("/LAMPE=ON") != -1)

  {

    digitalWrite(LAMPE, HIGH); // Allume la lampe

    value = HIGH;

  }

  if(request.indexOf("/LAMPE=OFF") != -1)

  {

    digitalWrite(LAMPE, LOW); // Eteint la lampe

    value = LOW;

  }

 

//*------------------Code  HTML ---------------------*//

 

  client.println("HTTP/1.1 200 OK"); //

  client.println("Content-Type: text/html");

  client.println("");

  client.println("<!DOCTYPE HTML>");

  client.println("<html>");

 

  client.print("<h1> LAMPE: </h1>");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println("h1{ margin-top: 20%; font-size: 80px;}");
  client.println("h2 {font-size: 50px;}");
client.println(".button { background-color: #FF0000; border: none; color: white; padding: 16px 40px;");
client.println("text-decoration: none; font-size: 100px; margin: 2px; cursor: pointer;}");
client.println(".button2 {background-color: #008000; border: none; color: white; padding: 16px 40px;");
client.println("text-decoration: none; font-size: 100px; margin: 2px; cursor: pointer;}");
 client.println("</style>");

  if(value == HIGH)

  {

    client.print("<h2>ALLUMEE</h2> ");

  }

  else

  {

    client.print("ETEINTE");

  }

  client.println("<br><br>");

  client.println("<a href=\"/LAMPE=ON\"\"><button class=\"button2\">ON</button></a>");

  client.println("<a href=\"/LAMPE=OFF\"\"><button class=\"button\">OFF</button></a><br />");

  client.println("</html>");

 

  delay(1);

  Serial.println("Client déconnecté");

  Serial.println("");
}