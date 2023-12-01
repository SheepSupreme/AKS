using namespace std;

struct Box {
  char name[10];
  int position;
  char characteristics[20];
  
};

const int num = 12;
Box boxArray[num];

void setup() {

//Inventar:

  strcpy (boxArray[0].name, "Box1");
  boxArray[0].position = 1;
  strcpy (boxArray[0].characteristics, "Holz");

  strcpy (boxArray[1].name, "Box2");
  boxArray[1].position = 1;
  strcpy (boxArray[1].characteristics, "Holz");

  strcpy (boxArray[2].name, "Box3");
  boxArray[2].position = 1;
  strcpy (boxArray[2].characteristics, "Holz");

  strcpy (boxArray[3].name, "Box4");
  boxArray[3].position = 2;
  strcpy (boxArray[3].characteristics, "Metall");

  strcpy (boxArray[4].name, "Box5");
  boxArray[4].position = 2;
  strcpy (boxArray[4].characteristics, "Metall");

  strcpy (boxArray[5].name, "Box6");
  boxArray[5].position = 2;
  strcpy (boxArray[5].characteristics, "Metall");

  strcpy (boxArray[6].name, "Box7");
  boxArray[6].position = 3;
  strcpy (boxArray[6].characteristics, "Kunststoff");

  strcpy (boxArray[7].name, "Box8");
  boxArray[7].position = 3;
  strcpy (boxArray[7].characteristics, "Kunststoff");

  strcpy (boxArray[8].name, "Box9");
  boxArray[8].position = 3;
  strcpy (boxArray[8].characteristics, "Kunststoff");

  strcpy (boxArray[9].name, "Box10");
  boxArray[9].position = 4;
  strcpy (boxArray[9].characteristics, "Wolle");

  strcpy (boxArray[10].name, "Box11");
  boxArray[10].position = 4;
  strcpy (boxArray[10].characteristics, "Wolle");

  strcpy (boxArray[11].name, "Box12");
  boxArray[11].position = 4;
  strcpy (boxArray[11].characteristics, "Wolle");

  //Inventar ausgabe:
  Serial.begin(9600);

    for (int i = 0; i < num; i++) {
    Serial.print("Name: ");
    Serial.println(boxArray[i].name);
    Serial.print("Boxbehälternummer: ");
    Serial.println(boxArray[i].position);
    Serial.print("Eigenschaften: ");
    Serial.println(boxArray[i].characteristics);
    Serial.println();
  }

  //QR-Code einlesen
  

}

void loop() {
  
  // Warte eine Sekunde, bevor du die Schleife erneut durchläufst
  delay(1000);
}
