import processing.serial.*;

Serial myPort;
String val;
int radius;
int floor_w = 3;
int floor_h = 3;

int[][] floor_array = new int[floor_w][floor_h];

void setup() {
  size (200, 200);
  background (255);
  println(Serial.list()[4]);
  String portName = Serial.list()[4];
  myPort = new Serial (this, portName, 9600);
  myPort.clear();
  val = null;
  fill(255, 0, 0);
  noStroke();
  textAlign(CENTER, CENTER);
}


void draw () {

  stroke(5);
  fill(180);
  
  if (myPort.available() >0) {
    val = myPort.readStringUntil('\n');
  }

  if (val != null) {
    background(255);
    fill(255,0,0);
    String valTmp = trim(val);
    println(valTmp);
    int[] nums = int(split(valTmp, ','));
    println(nums);
    
    
    for (int i = 0; i < nums.length-1; i ++) {
      floor_array[i % floor_w][i / floor_h] = nums[i];      
    }

    for(int j = 0; j < floor_w; j++)
      for(int k = 0; k < floor_h; k++){
        fill(floor_array[j][k]*100);
        rect(j*20,k*20,20,20);
      }
    //radius = int(valTmp);
    //println("r: "+ radius);
    //ellipse(width/2, height/2, radius, radius);
    //fill(255);
    //textSize(radius/5);
    //text("TEST!!!",width/2,height/2-radius/50);
  }
}
