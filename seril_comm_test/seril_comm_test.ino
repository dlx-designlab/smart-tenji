
// the setup function runs once when you press reset or power the board
const int floor_w = 3;        // floor width
const int floor_h = 3;        // floor height

int tenji_matrix[floor_w][floor_h] = { //user position matrix
  {1,0,1},
  {0,1,0},
  {1,0,1}
};

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);          // set up Serial library at 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);

}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

  String tenji_array = "";

  for ( int j = 0; j < floor_h; j++ ) {
    // loop through columns of current row
    for ( int k = 0; k < floor_w; k++ ){
      tenji_array += String(tenji_matrix[j][k]) + ",";
    }
  }
  
  tenji_array += "#";
  
  Serial.println(tenji_array);


}
