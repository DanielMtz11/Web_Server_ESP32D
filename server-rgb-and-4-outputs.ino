// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "ARRIS-C492";
const char* password = "B043D3924A4B8F99";
// const char* ssid     = "ARRIS-C492";
// const char* password = "B043D3924A4B8F99";



// const char* ssid     = "INFINITUM15F2";
// const char* password = "EV9HLq897W";

// Set web server port number to 80
WiFiServer server(80);

// Decode HTTP GET value 2
String redString2 = "0";
String greenString2 = "0";
String blueString2 = "0";
int pos1_2 = 0;
int pos2_2 = 0;
int pos3_2 = 0;
int pos4_2 = 0;

// Decode HTTP GET value 15
String redString15 = "0";
String greenString15 = "0";
String blueString15 = "0";
int pos1_15 = 0;
int pos2_15 = 0;
int pos3_15 = 0;
int pos4_15 = 0;


// Decode HTTP GET value 19
String redString19 = "0";
String greenString19 = "0";
String blueString19 = "0";
int pos1_19 = 0;
int pos2_19 = 0;
int pos3_19 = 0;
int pos4_19 = 0;


// Decode HTTP GET value 21
String redString21 = "0";
String greenString21 = "0";
String blueString21 = "0";
int pos1_21 = 0;
int pos2_21 = 0;
int pos3_21 = 0;
int pos4_21 = 0;

// Variable to store the HTTP request
String header;



// Red, green, and blue pins for PWM control 
//* OUTPUT-2
const int redPin_2 = 13;     // 13 corresponds to GPIO13
const int greenPin_2 = 12;   // 12 corresponds to GPIO12
const int bluePin_2 = 14;    // 14 corresponds to GPIO14


//* OUTPUT-15
const int redPin_15 = 27;     
const int greenPin_15 = 26;   
const int bluePin_15 = 25;


//* OUTPUT-19
const int redPin_19 = 33;     
const int greenPin_19 = 32;   
const int bluePin_19 = 35;


//* OUTPUT-21
const int redPin_21 = 4;     
const int greenPin_21 = 5;   
const int bluePin_21 = 18;







// Setting PWM frequency, channels and bit resolution
const int freq = 5000;
const int redChannel_2 = 0;
const int greenChannel_2 = 1;
const int blueChannel_2 = 2;

// const int redChannel_15 = 3;
// const int greenChannel_15 = 4;
// const int blueChannel_15 = 5;

const int redChannel_15 = 6;
const int greenChannel_15 = 7;
const int blueChannel_15 = 8;



const int redChannel_19 = 3;
const int greenChannel_19 = 4;
const int blueChannel_19 = 5;

const int redChannel_21 = 9;
const int greenChannel_21 = 10;
const int blueChannel_21 = 11;
// Bit resolution 2^8 = 256
const int resolution = 8;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;




// Auxiliar variables to store the current output state
String output2State = "off";
String output15State = "off";
String output19State = "off";
String output21State = "off";

// Assign output variables to GPIO pins
const int output2 = 2;
const int output15 = 15;
const int output19 = 19;
const int output21 = 21;



void setup() {
  // configure LED PWM functionalitites
  Serial.begin(115200);
    pinMode(output2, OUTPUT);
    pinMode(output15, OUTPUT);
    pinMode(output19, OUTPUT);
    pinMode(output21, OUTPUT);

    digitalWrite(output2, LOW);
    digitalWrite(output15, LOW);
    digitalWrite(output19, LOW);
    digitalWrite(output21, LOW);


  ledcSetup(redChannel_2, freq, resolution);
  ledcSetup(greenChannel_2, freq, resolution);
  ledcSetup(blueChannel_2, freq, resolution);

  ledcSetup(redChannel_15, freq, resolution);
  ledcSetup(greenChannel_15, freq, resolution);
  ledcSetup(blueChannel_15, freq, resolution); 

  ledcSetup(redChannel_19, freq, resolution);
  ledcSetup(greenChannel_19, freq, resolution);
  ledcSetup(blueChannel_19, freq, resolution);

  ledcSetup(redChannel_21, freq, resolution);
  ledcSetup(greenChannel_21, freq, resolution);
  ledcSetup(blueChannel_21, freq, resolution);  
   
  
  // attach the channel to the GPIO to be controlled
//OUTPUT 2  
  ledcAttachPin(redPin_2, redChannel_2);
  ledcAttachPin(greenPin_2, greenChannel_2);
  ledcAttachPin(bluePin_2, blueChannel_2);


//OuTPUT 15
  ledcAttachPin(redPin_15, redChannel_15);
  ledcAttachPin(greenPin_15, greenChannel_15);
  ledcAttachPin(bluePin_15, blueChannel_15);  

//OUTPUT 19 
  ledcAttachPin(redPin_19, redChannel_19);
  ledcAttachPin(greenPin_19, greenChannel_19);
  ledcAttachPin(bluePin_19, blueChannel_2);

//OuTPUT 21
  ledcAttachPin(redPin_21, redChannel_21);
  ledcAttachPin(greenPin_21, greenChannel_21);
  ledcAttachPin(bluePin_21, blueChannel_21);

  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();



               // turns the GPIOs on and off
            if (header.indexOf("GET /2/?r255g255b255&") >= 0) {
              Serial.println("GPIO 2 on");
              output2State = "on";
              digitalWrite(output2, HIGH);
            } else if (header.indexOf("GET /2/?r0g0b0&") >= 0) {
              Serial.println("GPIO 2 off");
              output2State = "off";
              digitalWrite(output2, LOW);
            } else if (header.indexOf("GET /15/?r255g255b255&") >= 0) {
              Serial.println("GPIO 15 on");
              output15State = "on";
              digitalWrite(output15, HIGH);
            } else if (header.indexOf("GET /15/?r0g0b0&") >= 0) {
              Serial.println("GPIO 15 off");
              output15State = "off";
              digitalWrite(output15, LOW);
            } else if(header.indexOf("GET /19/?r255g255b255&")>=0){
              Serial.println("GPIO 19 on");
              output19State = "on";
              digitalWrite(output19,HIGH);                           
            } else if (header.indexOf("GET /19/?r0g0b0&") >= 0) {
              Serial.println("GPIO 19 off");
              output19State = "off";
              digitalWrite(output19, LOW);
            } else if (header.indexOf("GET /21/?r255g255b255&") >= 0) {
              Serial.println("GPIO 21 on");
              output21State = "on";
              digitalWrite(output21, HIGH);
            } else if (header.indexOf("GET /21/?r0g0b0&") >= 0) {
              Serial.println("GPIO 21 off");
              output21State = "off";
              digitalWrite(output21, LOW);
            }              
             


            // Serial.println("estado 2:" + output2State);

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                client.println("<link rel=\"icon\" href=\"data:,\">");
                client.println("<title> The Room </title>");


                client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
                
                client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");

                            //*************       CSS     *************
                
                client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                client.println(".button { position: relative; border:transparent; width: 60px; height: 30px; border-radius: 20px; transition: 0.5s; box-shadow: inset 0 0 5px rgba(0,0,0,.9);}");
                              
                client.println(".onn {background-color: #007399}");                     
                client.println(".off {background-color: gray}");
                            
                client.println("body {background-color: #121f3d}");

                client.println(".ctr { margin: 30px 15px; font-size:50px; text-transform: uppercase; font-weight: normal; display: block; height:auto; text-align:center; color: white;  animation: neon 3s infinite;}");           
                
                client.println(".container-btns { position: relative; transition: 0.5s;}");
                client.println(".circle-off {position:absolute; top: 1px; left:0; height: 28px; width: 28px; background-color: #bbb; border-radius: 50%; transition: 0.5s;}");                                     
                client.println(".circle-on {position:absolute; top: 1px; right:0; height: 28px; width: 28px; background-color: #bbb; border-radius: 50%; transition: 0.5Ss;}");
                                                                    
                              
                client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                client.println("</style>");
                
                //*************       /CSS     *************
                

                
            client.println("</head>");
            
            client.println("<body>");

                client.println("<h1 class= \"ctr\">Control</h1>");



                    client.println("<div>");
                                client.println("<p>GPIO 2 - State " + output2State + "</p>");
                                

                                  client.println("<img src = \"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIAAAACACAYAAADDPmHLAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAADsQAAA7EB9YPtSQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAABZXSURBVHic7Z15eFTV3YDfWTOTTBKykRAI+2rYQRYFF0SllLafO59VWttK+1mlFrUuRStudalCW7e6oMXWpZXWFUQoiCCggGAQCoQQwp49mUwyk9nu98fJTO4smWwzdwa47/PMQ+6559x7hvubc8/5bQdUVFRUVFRUVFRUVFRUVFRUVFRUVFRUVFowATcCK4CDQA1wAlgL3A3kxq9rKrHmf4AjgBThYwceAPRx6qNKDEgH/krkBx/8+QQwx6OzKtFlBlBG0APWG4xSr35DpUGF50oFg0dJyZb0cELwbrw63Vk08e5AAmIGHgMWAFpfoUajYeA5Exk2djoGY5K/siRJHD24m91frsHtcsqvcwPwd4X63GVUAQhkArAcOEdemGxJZ9z0OWTn9W2zYXX5MTavfhOvx+MrKgaGIUaEhEUX7w4kCHrgt4iHnyc/0XfIaCbPvBpLelbECyRb0nC7nNRUHPMVZQGrgOPR72700LZf5YxnKLAJeAgw+AqTTMlMvuRqxk37LnpDUpuN5fQfNja46LxodTJWnM1LFg1wC/AkkCw/kdd3CGPPn02SKTlsw7ZISc1AbzDK5wJ9otHRWHK2CkBvYBlwmbxQb0hi1OSZ9B0yOlr3cbZfJb4kugD0AQoRDywXyGkp9/3HVgMnEZq5bxDKmPb4X+A5IENemJ3Xl3HT55BsSe9yZxutNcErgcNdvphCJJIAaICxwMyWzySgRyfau4DdwKfAv4DtBM7AM4HngevkjbQ6PSPGX8CgwkloNN1bFJXu2xlctLlbF1SARFgGDkOsma8HBkbxuiXAs4ihfmrLv/nyCulZuYyf/j3SMnLCNO8cVaeOsHn1W0her69oFzCu2xeOMfEUgAuBu4DZHe2HVqcHTcvCxevF63V3pFkjYpLnv4dGo2HIqKkMGzcNrbb7K+HjpXvZuWklHrdLXvx94MNuXzzGxOMVMA5YghCAsOiTUjCn5ZBkycJoTsNgTkVnMKHRBK5avR4XbqcdZ2MdjoYq7NYKnE11wZdLCThIy2D89O+R2bN3t7+Ix+Nm99Y1lB3YFXzqr5wGDx+UFYAewFPATwijfzAm98CS3RdLZgEGc2qHLqjVGTCaDRjNaViyhZbOabdiqyrDWn4Qj6s5pE2vvkOj8vAb6qrY/tl7WGsrg0/9A7i52zdQCKVeATOA14GC4NtbsvqQ3msoptTuv4flSF4P1vISao/vxeNyBJzrPeAcxk//rnildIEjxUUUbf00eMj3InQKiwBP2IYJiBICMB94MfheKZm9ySwYhTG5MxP9zuN1O6kq20VDxaGA8qy8AqZeNhddJ4TA7XLyzZZPOFayJ/hUBcJZ5NPu9ldplLAFvIpMv65PSiF3yFQy+oxEZzDF/OYarY6UzN4YTBaa6k6CJFaGdpsVW30N+f2Hd2j5V19dzpbVb1N16kjwqf8iVL7fRLvvSqCEAIwAJvsOTKlZZBaMROkFSFJKD8zpuTRWH0WSxFKtoa4Kr8dNTv6AiG1L/7uDbZ+9R7OjMdxpC0LIa6LdZyVQQgA2AlfQosVzOWxotXpMadF953cEfVIy5vRcbFVl/pGgpuIYmT17k5KWEVLf5XSwY8MHlOz5yi80AGZzMu7W978BYT5+I+ZfIAYoYQ1sBOYiU9PWHC3C0VClwK1DMVmy6Dl4ckDZri9W4Q5aMdRWHuez95dxsmx/QPnYCZNZ+uJyho0YKS+eiZgDnHYo5Q9QjtDbz/EV2OtOkdqzP1qt8qoIY3I6HpeD5kYxartdzWg0WnJ69QPg0N5t7NjwAc7mVtOCTqfjyutu5OZbfo3JZGboiELWr1mJt1XzdwHwGtCk6JfpJko6hGxHDJWFIJQ4LofNv35XGnNaDrbKMrweMZTX15TTq+8wdm78iEP/3YEktZoRcnrmcfcDjzHtwpn+CWNqWjout4t9e3b7qvm0jWsU/SLdRGmPoDXAtbRY4lx2Kzq9EVNqZG+bWKDR6tDpDTTWCocdr9fD4QM7sdWHzuV+9+gzDBg0NKR86LBCtn6xAVuD1VdUDLwXu15HH6UFoBnYAvzId2+7tZzkjHz0RuU9qY3J6diqyvC6W0y4Unj3varKCqZdeElIuU6nY3jhKA4fKqFHRlajVsM8u90en8lNF4mXMejXwDO+A4PJQp/Rl6PVGSI0iQ21x/dSc6QouPgAYtI6xlfwy9vvYdpFMyNeS5K48/orZj4d9U7GkHj5BC5FZixxOWxUHtoel46kZvcLLvoC4R08D+FjAMDyZS9gtdZHvJZGw0N/X7E2mibtmBMvAegFBMz+bFVlWIPUtUqgT0rBaE6TFzkBG1AEPOErbLDW89eXn2vvcslaHe1WSiTiIQAjgK3IhlcfVaU7cDZF/pXFAlNaT/nhWFpfjY8gVL0AbN64jq+3b23vcrPefn/t3Kh2MIYoLQA9gI+RWQUt5tb3vuT1UF68GcmrrDEtKSXAIJVBa/+aEaZd/2J/2Yt/xN4UealfU1PzJ6PJtAzxqkuLWDnOKL0KeAM433dw7vBcNj53HUUlVZSeFL98j6sZj8tBSmb3bfYdxeNuFurhVt6hNaDjKEKNPQnA3tREU6ONcROntHm9l579Q0pZack4YAowGPhnLPodDZQcAa4ArvYdDO+bySdPX0l+dgrLF11ObkarD7614lDwA4kpYaySwTbq+xDh4QCsXf0R+/bupi1crgA/gWuA73Wvh7FDSQG4z/eH0aDj3Ufm0MMiIm7yMlNYvmgWWm3rqrTy0DZcjgZFOhbsakaoADQAv/AdSJLEy889g8sZ3u3/imt+iFYbcM1ngY65OSmMUq+AGcC9voM75k5g7iXDAioMzE/H6fKyqUiMvJLkxdFQRWrOwG67a7eH1+2k/lSxvOgdYG9QtYOI4Xw0QEODFa/kZeTo8SHXy8zKpsFaT0nxPl9ROiLqeHWUu95tlBoBvu/7Q6/TcttV4b2lH/zJFKaNavXcbm6spfpIiMNl1PG4Q37JIZ6lLdyOMGwB8NG//0FpSXHYitfd8BOysgNM3gsQc4KEQikB8HsAz5hQQH52SthKep2Wvz3wHbLSWt/J9ScP0FhzLGz9aBHsMwhYw9VDWDR/7W/n8fDy80vweEJXLWZzMj/7v9vlRVrgL8gCUBMBJQQgmZZhE+DCMZHjJQt6prLs3suQj/oVJV/hbg7rjRMVXA5bcFGI35eMt4CPfAelJQdY+UH4hCBjJ0xm0tRp8qLRwK+61svYoIQA9JDfZ2hBqOdNMHPOG8iCq1tfE163k1MHNgd45UST5sZa+WEFcKqdJj9H9pr455uvc+LY0bAVb5q/gOQUi7xoMdGNgOoWSghAwOzXN/Nvj8d/Po2Jw1ozrzXbqqk52vbSqzs02wJMwB1x7jyBbFXjcrl49cWlAT4EPnpkZDL3xp/Ki5KB33Stp9FHCQEImMI3NHUsYtpo0PHmg7NJT2kVmLrj+4RnbxRxOWzBy80tHWz6F+Bz38Heb79h3ZqVYSvOvHwOQ4cXyosSJm5AiWVgA2IJqAU4b2Q+k87Ji9yihcxUEwPz03n3s9aZtr2+HEt2v6iZjhsqS7HXBYz4dyJ+4e0hIaJ/b6Ylwmrfnt1Mv+hSzMmBiSU0Gg1jx0+ivq6WgYOH2QYOGH5daekBZZQc7aCEAHgQptVMELr/ay4O9a5pi8IBWZyqaWTH/goAJK8bZ1Mtqdn9IQr6gcrS7fJVwAlEwGpHqUbYCS4B8SqoKD/BedMvDqloNiczaep0Jk4+zzhxyhTp3beXJ0QQiVLLwE2+P9ZsO4LN7opUN4Rnbr2QUQOz/cf2+gpqj4dE53SaZlsNzsaAJf+bdD6r11OAPzHA9i83s/WLDREbSBK3//3f6yZ08j4xQSkB8OfLq29sZvknwUq2yJiT9Lz14GxSTK3Dfs2xPditFd3qVN3JAJdvCXi5C5dxAz9r+ReA119+Vu4nGA6dVut9df369XFP0KGUAKxDli5t8WtbqbaGKF8iMqJfJn++XTa0ShIVxVvCRgB3BJejAVt1wHJ/PcIVrCt8DfhdwerrannjtRcjt5AYU271LOji/aKGUgLgQeThA6Cq3s5Nv1+Nx9u50fZH3zmHGy4b4T92O+1UHGzXQSMsNUeKgp1AH+3ShVpZjPAKBuDzdZ9StCuym9uBffseNRiNuxGrif7dvH+XUNIauBzhbwfAx5tLmf/kWtyezil3nls4g2F9W5VJTXUnqTuxL0KLUOz1FdiqAxQ3GxCjVHewI1YEfql65fmlOBxt561652+vmFxO50hgOvA34uChpbRDyCZElq5kgF0HK9n87QkuHlcQsN6PhFarYf+ROrbv89tkcFgrMPfIQ29sP6+f1+vm1P7PW13BxSz+h0A0DA5lCH/HiQBNjTaczc2MGX9u2Mrf7NzG0SOHfYd9ERnPdkShHx1GaQGoRvzargeMAKUnrbz84bc0OtwMKejRpiA4XR7+sf4AP35sNR9vKQ06K2GvLyctZwCadnL+VJV+HbzufwV4oYvfJxwbEXGCaQAlxfsYPe7cYMsgAP0HDmbdpyvlxqRpiJEyxDgRK+IVF3ABwuYeoBHSaTWMHpTDlMJe5GUmk2TUUV3vYM/hajYVHcfaGFmLmJJVQN7Q89s8b6sqo7w4QNFXjghXi3Zo9/eB930HfQr68dgzL2IwBCqvXE4nr7ywhM/XB0ST/RORyk6RJNPxzBKWi/ARvLSL7auAB4GFyIwr2QMmkJ43JKSyw1bNiT3r5A6nXuA7xC6rx9vIchJeNXceV8+dB8Cxo2V8sOJttm7e0JZXUSki0HQJCo4G8UADfBcxOezojhy1wB9p0SwinDWbfec1Wp3UZ/Tl0qCpc/2fgjGzJJ0+Kfg6j8T4u+UihFRsNKHXS08sfUm64tofSjqdrqPf9QQtWsazgQmIzZdWIuzxNYjlYxXCQPMacBVi86ZgFiL7jzOYUqUBk65uffgGU/B/7HsoM/+ZJ7+v0RgihB35uBCJNGNCImQKjQYa4ANk+Qcs2f1I6zmQU/s3+UPAW9iC+FV1JK9wNPgEuDzcCXN6T9J6DiIpJROtTo/TYcNhraD+VHGwl5ILkdj6s2h37kwRAIBshE6+1eVIowlW9mwHZiFWI0rRH5HD2O8VotHq6DloEpbQuERABMhUHPwyWFN5CBhFlBNQnEk7hjQhHvCNhFeofI54+G05fMaKOsSqoEUwNeQNPR9LVkGbDTQaLZasPrjsDTjt/lC5DMScYFs0O3em7RiyEaGSDeYtxIw/ooUmRgxH5g2cmtOvg1FPGrIHjEerN8oLfxbtzp1pAgBix69VLX+7ERPE64lf7p6ApALpeR33hdAZTKTm9JcXjUHsRRQ1zkQB8CJ2+pyF+PUtiW938EfAaHV6kiyZkeqGYA6MXNYi9jiKGnG3R8cIJ4kThePfgkSr75i9Q06YNp2ToPauH82LqYTF77DgcdrbzEPUFpInxHsqqmprVQBiy0+BH/sOJMnb6QQYYbyegi1h3UIVgNigQaxGXiHoNRvkhhYRyesJDpMvov2glU6hCkD0MSCSRz8Q7qSt6jBNdR17hlWlX+N2Bigso74XsSoA0SUVkf3sJnnhuVNa4wMlSaL8wKaIAS6S5KW6bBfWihJ5cTlEPwHVmaQJjDe9EJlQ/U9bp9Nx8y8Xcv28myk/eZwjZeL1LUlebFVluOz1aLV6dEZh3/K4RKqaykPbgiOivYiE251zp+4AZ5ItIJ6MQCif/Mp9k8nMr35zP2PHTwLA6WzmyYd/y57dnc53ICGUWUuj1NcA1BGg+1yA+OX7vZt6ZGRy3+InGFHYugWtTqdnyrQLqa+r4/Ch8EklwlCH2GTrlSj2NwBVALrHtcAKZBHQ+b0LWPTw0/QpCLX06XR6Jkyayjkjx3CoeH+kzKO1iInktcCX0e92K2eqJlAJFiLCwvwT6WEjCrnzvoexpEZODeiwN1FZWR5cfBj4A7AHkUizc5EzXUSdA3QeDfA4QTH+506Zxq0L78VojKzuXb92Fa++sDQ4rUwRwjUutrlwwqAKQOdIQuwKGrAB9aw5VzDvp7dEzGYmSRIr3nmDFW8vDz61BpE/MR6malUAOkEGwtV7uq9Ao9Fw5XU3+r1928LtdvOXPz/Fpg3/CT71OmJfxc6FS0cRVQA6Rn/EMm+4r8BgMPCLBXdx3vQZERva7U0sffIhinYGxAlKwEMIt/a4ogpA+4xGeCr73XhSLKncce/igGVeOGprqnni4fsoKw3Q6LmBW+haKHrUUQUgMpcC7yLL+J2Zlc3d9z9G3/6RE30dPXKYJx66l+qqgM2lbYil3arwrZRHFYC2+THwErLEjgX9BnDPA4+RmRV508tvi3ay5PEHaWoKyG14EjHT3xm+VXxQBSAUDfC7lo+fkaPHsfCexSEJoIL5fP0aXnr2aTwet7x4DzCbyAko44IqAIHoERa3+fLCCy6+lPm33tHuTuOrPvwXbyx7IThf4HrgSpR3R+8QqgC0YkFELM+WF3Zkje/1enntpT+z9pMPg0+9i4hTUESr1xVUARDkIbay8ed+12q13DT/NmbOirzXQ7PDwR//8DA7t4eo7P+ESCwdm/y2UUIVAJEfYCUyU26SycSCOxcxPsK2MAB1tTU8+cgiSksCckt5EKnhn49BX6PO2S4AUxFBpf4khOk9MvjNokcZODiy+/3xY0d4fPG9VAUadRoRKXBC3gWJytksAFciEjP596zNzcvnngd+T15+5NCt4v17eerR+2kINOdWI2IAN8egrzHjbBWAXyG2rvWbcgcPHcFdix4hLS297VbAV1s28tySx3E6A/ITliBiDzvs6ZEonG0CENaUO3Hy+dx2x33tmnLbWOZ9idgVrDJ8q8TmdBeA9JZPRxxbtIiHf5W8cMKk87jhpp8H7/IVwsoPVrD645Cd4b9EBH/IfbebEXOBhFz3B3M6CYABEWn7A2AyYvZujNgivjQiNICrEGbkhFIBn05ogdsQSRi7kmMnUT7fIOYJKp3kLuL/8KL5+Q8y03K8OR2cQgO3GdNoMKVmY0rJRJeUjFabaF9BwuNqxu2047BW4LSHeHrNAL5CbKX7leLdCyLR/vcCKLzoGou1tnF4RclWvG4nqT0HkpY7CF0X4uzjhctho+7EPhoqS+VJKvMRKXMvQ6S1iRsJOwkcPP2aHMmtWw8Utlv5NMBlt1JevCV4i7pqRKLLQ/HpVYIGhkyYMN/gwP0+GrFl+5mAzpBEas5Amptq5buUJSNiCV9FzA+U71c8btoe5gFDn9IgXR/vfkQbjUaDJbMP9oZK3M3+nFX5iKQPsd8kOQyJFB6eBNytM5if9bqab413Z2KFRqsjd/CU4G3vHkJmk1CSRBoB7gcekbzuSe7mRl2kRIqnO77cf/Z6vyUxHTECRD38u92+KH3DCFzp+8PtUiqNb/xIzx2MNtDF7Afx6EeiCEAvYKTvwJSaHaHqmYFWb8ScHrBfxmzi8DwSRQ8wFtmS1GhKC7el+xmH0ZyGzHE8E+GVFNUsYO2RKAIQkA6zoiSmIfGJTAEKC0CivAKimv3yNCZF6RsmigBsQrb16llKHWIHUkVJJFXwKERa9UTqk1K4EVbCsvYqqqioqKioqKioqKioqKioqKioqKioqKiodIb/B1OaV3LyBczqAAAAAElFTkSuQmCC\">");   
                                        

                            
                                  //*Buttons RGB                        
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                              
                              
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                
                                client.println("<div class=\"container\">");

                                    client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color1\" role=\"button\">Change Color</a> ");

                                    client.println( "<input class=\"jscolor\" id=\"rgb1\" type=\"button\">");

                                    // client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> ");

                                    // client.println( "<input class=\"jscolor\" id=\"rgb\" type=\"button\">");

                                client.println("</div>");
                                  //Button State                              
                                    if (output2State=="off") {
                                      client.println("<div class=\"container-btns\"> <p> <a href=\"/2/?r255g255b255&\"><button class=\"button off \"> <span class=\"circle-off\"></span>  </button></a></p></div>");
                                    } else {
                                      client.println("<p><a href=\"/2/?r0g0b0&\"><button class=\"button onn \">  <span class=\"circle-on\"></span></button></a></p>");
                                    } 
                    client.println("</div>");

                    client.println("<div>");
                          // Display current state, and ON/OFF buttons for GPIO 15  
                        client.println("<p>GPIO 15 - State " + output15State + "</p>");


                            client.println("<img src = \"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIAAAACACAYAAADDPmHLAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAADsQAAA7EB9YPtSQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAACAASURBVHic7Z13fF3Fmfe/c87t9+qqVxfJRcZFcqGZYsDGhoQkhGyykLakd7KEhCSbuvGmbt4Uskk2hZBsKpsQNrtA6AYMGNOMe5GLbNmS1dvtp8/7x1G1rootGRnw7/ORLd1zZs7c8zzzzDNPGziLsziLsziL1yjEZBpv37XrTaDcDM4vltXU/K8QQk7VwKYKO3bsvQjV+YJweHLp0prbpns8Zxo8k2ksUK4AeRWIq3bu3rNl++7dX1leU/PweO06OjpyTNv+OZLrgMhEnqXrupNKpzP+gP/91XPn/nW8+3ft2rXUkeIbCOfNSJAKEeAsA5wAZTKNTT3zJQQ3IWkGzheSh3bs2vXk9j17Vo3ZzrK+huTdTJD4ABIU27bDWkb701j3bdu7t3rHzt13OohtCN4MJEF8y9S0t030Wa8lTGoJ6MeePXt8ts37pJBfB0rdT+UGIeUXli5d+tKJ9ze3tD4PXHgyz9B0nUQiAUCoIP/c6hkztg29vmPHjplSVb8qJB/AlWyGFPzWK8TXlixZ0npKX+w1gClhgH48d/BgNKgZnxbIz0iIAhLB3dj2V5ctW7a//77m1tZtSJZn68OyLEzTJBgMDvt8KAME/b4rFixY8BTA1q1bi1Wv/8sgPwb4AQvJfylCfqO2trZxKr/fqxGTWgJOxEXV1fFltUv+zetR54L4PggNyfUonuellBN6lm3bmJY14WeqXu/dID8FeBHc6ajK4mVLaz5ylvgTw6SUwNGwaNGiLuBzW+vqblNN+/NCyIwQwhmrjWYJHAd0Q2AYAo/hCqegT44ppiT8TiCOCml/b2nt0l1T+DVeE5jSJWCiOHEJONCu8t0NEZwsm8hrFuvcsEIbdQk4i8lhSpeAU0VbQh0gvmEYZDIZ6Pu7JX5GDPFVi9OyBJwqUqkULc2tSCnJzculpKR4uof0qsekGGDLli1eTzBYo9j2mNNUCNFUW1vbNl5/6XQGKd2pn06lgPEZoK6uLscwjAXj3efz+Q4sXLgwMW6HrzFMigG8/uC3ceRnpRhbTEvoklKWjKcI5kQixGNxHMchGo1OaAy6Yf0fQrly3PtM6wlg3Ptea5gUAzg4GxUpLkOI8fp5YTziAwSCAebMrcKxbTxe78QGoXAfkiiIMRRaKRHcO7EOX1uYFAOsqK29H7h/soOYnW8T8knShkBRFBTFlSiqAguK7THbLqup+RHwo8mO4bWKM0IJrCyw+c/r4wDouo5uGERzcqZ5VK8NnN1jvcZx2hmgrq4uZ8uWLbkTvd/j8eDzTXD9BzZv3hzct29f4SkN7ixOHwPU1dXlbN+5+6u6aTd6/YG6ifoCVFUl4A9M+DnhnOj/GpbdsnPXrl/s2LFj5ikP+DWKKWeAzZs3B7fv3PMZ3bTrheDrIHMRPD2RXcCpQT4NCIn4KIp6cOfu3T/YunXrWQvSBDFlDLBlyxbvzp17PhLOyT0ohPwByGKQz6GItctqam6YaD+2ZaFp2oSfu6y29ls49gIpuB2EV0o+o3p9R7bv3v3v27ZtyzulL/MawqQZQEqp7Ny553qvP7BHCvlLkDNA7hZS3LCstvbiZUuWPH4y/Vm2jWGaJzWGZcuWHVleU/NRqbAMwV+BkJD8i+Lx1u/YtWt9XV3d2S3FKJgUA2zbvfuCnbv27JRC3gVUA/tQxPVLa2qWLl26ZNy4vaFwpI1pa5i2htX3v2nrJzWe5UuW7FlWU3ODglwlYCNQAOJrumkd2Llzz/Un1dlrBJOyAyiS9yFYgqABWL9/794/3nDDDWNbbrKgK9XExgO/w5Ej1YTF5ZezpPyKk+qvtrZ2M7Bm165dVzmIbwIXSuF8BjgppnwtYHKmYMv8sqr671cVuWHJkiXGqfaT0DoHiG/oBpZlEQqHAOhJn3o4X21t7aNSyg3bd+++QqrqkROv37l+dZFtWldlbN9KzVHm25bIs1BDlk3wiBWco0vpFR5pexTpKB5pBHx2S9TvPBqI+L/7lVvveVVEHE3OFLxiRS/wwBSNhXhvgrq99UjpUDajhMqqye/q+nIVNgL8af2lb4hl/Dd2pIOr2tLBsvsO+D1y/EyG/mXSH4ecdliA4KZ3vOft+MKGE8k12goLje9/43OP/nDSg50GnBGm4H709MSQfZKgp7N3Shjg91++4saWdODWwz3Rmvv2+9RJdwhusIrpwej1KN29ofLuo/zgne+54QehfE0vK9X/41tfevhfpuQ5LwPOKAbIL8ijvbUDx5EUFBeccj//880rK5tjys/qe6JrHzgc9E/hEEeFNL2k2r3++vacz7/rA2/7fF6R1lFRIi/4ymcfOPpyPP9UcUYxQDQ3wvLzajBNi1A4OH6DE/Dn9asWtCSDt99/oODytOmZlnhHACcToLsxUNzdbDd86DNv1haU6Vd//vMPPz1d4xkLZwQD5ASKUYQHR1p4fV68Q3wB+aGycdv/bf0lJQ3xyD0PHiq4SLenRspPCWyVZEtOYGt7+KmPfe5NPVWF2pwvfGFDbLqHNRRnRFQwuHYA2zHRdR3DMMjJyUEIBY/iA0ZPDPnVF9d8a1tb4ee7M/6TYmYhoCSkURlNUBrJcLA7Sl3X6IbDwlle/BGFdNwh3WNjpE/esi0CBjOqkg9//2sPvv6kG58mnDEM0I/R4gFOZIAtd763va6zcOOBrtzSiT63KKSxorSb2uJulhR3E/IOmiwcKfjKk+dxpHek0bBojo/qS0PDPjMykliLSazVoqfJxDImmhgt8Rem7YUznIVf/OK9hyY69tOFM2IJGAqPqiLHcQfvfvh7b33q6IxP9mr+ceV90GNx8cx2Vs1qY2FB76iBY4qQVBfEszJAtHjka/IFBcVzfRTP9SEdSW+zRccRg+5Gkyz2rCEQ6F1hdWfSOPiv33r9T77+5YduHu87nE6ccRJgNGi6TjweY+ejP0s+vDUesZ2xh54XMHj93CaumnOckHdIqpkHKFAgKSE9fNb+blc1D9WP3HrOvTBI6YKJbSaMlENLnU7bQR17nAw3IRyKKmMNP/nOA3Mm1PlpwCsmIkjaltx417+nHtiSGJP4ftXmzdXHuG3dc1y34CghnwVBAWpfm6CAUN/PaYAvrFB5XpBz/yHKjMUBlDFklJQKHQ35VR/+9HXp0zKYCeCMWwIs28ayzGFBIZaR1u786Wcze1pD+WO1XTWzjRtrDxH191mlIwJyFfACKQkdEhISTAcmvGafGjx+hdnnBiie76P+uTSJ9tHFQaI1EnzvTW8255hbctff3vyyMsMZJwFsy8IwBt3BppbQf/n/PmuPRfy8gM6tF+3ipvP3usQPCpipQFEf8U1ckd8PTcJpCk85EcGoQs3VEarODyKU0ZlO787x7LcuSFy/Ht/LMzIXZ5wEGAppm/Z/3fZ5vb47NGqWyDmFMW65YDd5AQNUoEBAuI+vLQm9QPJlovYYKF/oJ6/Cw65HerC17K/dSYcV5cib0vD3l40uZwQDeJP7KNzxEYSdGvhMSviPl85N1XXkjkr8q+ce5z21B1GFdNf0IgGKAEdCj3Rn/RiS3vLkkwwvIxOYh+6bSe+xNmD/iPsqo+cxr6ictNVLXG+nV2/Bdk7e+RmMqqy4toDtD3VgJUYqlUIIHC1HveE9r9fu+v1DEw+MnATOEAY4MEB8aYF04E9HliWebxqd+DcsOsw/nNNnZs9XILdPqUtL6HJglKgEw1tGT+46YrmXofmHK9+W+mjWNn5PhOLQ/CGfSGJ6O+3pg7SnDmI6Ew9h8/oVll1TwPaH27Fj4RHXFUVBT0T87/3oNS2/++WD5RPu+BRxRjBAP6ykJFVvsz84w35wX37WMC4h4L21B3nd3CZXgylW3DVfAt0SEtnFfTK8lPaid5EMr6B/96s4GuH0LkLpvQSMRvJjaWDkY4/FtxLo6iLsLSDXX07UX0pu38/8/EvoSNVzNL6NlNk1oe/p83lZcFmE/U/HcWIjn+cP+Ok8rpX982ev+v1Pvv/oeybU6SnijGIAs9choQb4zeH5YrS6IG9fdNglvgBKFAgIsPs0fG2kvM8Eqmku+wSp0BLAJXpuYhP5vRsIp3cg5KCoUO1qsjGAZiVoSx0cvE94KAxWUho+h4LgbErC1ZSE59ORPsKh3s3o1vhJyHl5uZQtjdG6PYGTGPnMSCTKgd2dN97yuWt/9KPv3bd13A5PEWcUA3iiCr8+styIab6smvAb5jdx3YKj7jpfqvSXhIJWx/1/CBwlREvp++nOvxaJgseKUdT9Nwq770F10ifcG0Tzz0b3zSCb0pDjKybi85I2e3Ckgy0t2tP1tKfr8athZkdXUB5ZRHFoLoXB2TTEtnAsvj1rX0NRMaOCWOwA6YMqMjPc1CyEIOAP09CQeRFXvT0tOKMY4IlETbKuKydr7cAlxT28e0mf6bxIuMS3JbSNJL4WmMvRmV9B981ESJvirrso7fwTipMZuEf3zaA390qS4XNJhxYiUUkcehTYMuLZ5ZHFrChbgSMtYnoLXZljtKcPYtgZdDvFwZ5NHI1vY17+xZSGqpmbdxEFwdns7dqAYaVG9NcPr8dDWVkxTXobRr0XaQ03gQeDIbq70sonbn7d4z/78cOnJbX9jGAAKzSbhCyQd+/IH6kVAYVBnZvP34MipLvNC/Vp+i1yBPF7c9fQWH4rUvER1OqZdfy7BPSGvquCeM5KOgpvIBWqGWgjsAlq9fiN41nHlzQ6SVu9hDy55AdmkR+Yxbz8S+jKNHAsto240YZhp9jXuYHWQB0LC68kz1/BBWU3sKv9AeLG6LUxiooK6ezpxqpox2kqRzpDTDMCwjk5HKyLrfniF9cWfuc7j01MyTgJnBEMYERX8MddizoShrfkxGtCwMfO3UfUb7qWvajiStZ26e7zh6Cz4C00l34MhEJhz9+paPsFom+7lgotobn0JjJBV5tX7RTRxCby4k8TTu9CcTJsTlYDI30Bzck92M2H8CoB8gIzKA1VUxCspCg4h6LgHLq1Yxzs2UTGjNGjNbGl5S4WFa6jIDiL5aVvZk/HI3Rp2QODFEWhuKAQI9OClt8DXcPTHP1+P6mEwpEW9SgnUVl1ojgjLIHdjdtaXmwYSXyAdVXN1BT3gEdAYd9wu5wRCl9n4VtpLvsECEF52+3MaPkxwjFwhJ+m8luor/ohmeB8vGYHFa0/Y9GBdzCr+QfkJF8YtjSMBdPR6EjXs7vzIZ5t/j1HYy9hOQYFgdlcUPZ2KnPPRQiB6Wjs7LifluQ+FOGhpuT1FAYqR+23oCAfr8+DpzCJCIzcUobCITqOWeHPfenacyc00JPAGSEBHvnbHdjOSLtH1G/wziX17h+FwtX8U3K4WRfoyV1Lc+lHQTrMbLmNgl63XrXum8XRmV9FC1QhHIOSrr9Q0vWXAakAoPnnkIhcSDJcS3djK7BnxDiq8i5kblEZvfpxutINaHYS085wJPYCTcmdzM29iLLwQubkriTPP4O9XRsw7Qz7u5/EcnRmRZezpPhqdrTfS0wfuRyoqkp+fh6W2YVd2IlsnjlMfwwEQyTiSY426puA0IgOJoFplwDx9oPdWxv9WQ0e1y88QtBjuaI/2Lfd6x6+z0+HFtJUcSsgqGi/fYD46eAiDs35EVqgioDWQPWRmyjt+APCMZCo9OSu4+Dc/+TAvF/SUvpBEpELcZTscYg+JUhxaC7V+ZexsuKfWFZyLQXBWQCYtsb+7o3sbP87hp0hPzCTFaVvIaC6lXLre58blATF1+DzZFVzyM/LQ1EFSsiEnPiI68FQgK5mJ/jVr79uyUTf7UQw7RLg8f/5mW45I/mwIpJmTWWLO+vz+653y2EWPluNcGzGl5DCQ1HX3yjq+hvgrvdHZn8HRwmQm3iGWU3/jiLdNLNE5Hyay27q2/KBaifIjT9NJLWN/N4YMNLn1BB7kWBXL4XBWRQG55AfmEl+YCYxvYWD3U+TNLvo0ZvY0vpXaouvIcdXzIqyt7Ct9f/Q7Dj7u58k4ImQH5jF4sJ1bG+7lxO3iIFgEJ/Xi+1zcPJ6cJI5MMTt7Q8ESSVTHD7qfRo49ZDpEzCtEsC2dGP7MTtrSNe11cdQFQk5wt0FG9IV/0PQXHYThreMULqO8vZfAa5Ib5j1DRwlQH7vw8xu/AaK1HGEj6byWzgy+9vovhn4jSZmNX+PxQfewcyWH5EXfxLVyb5lM+w0rak69nQ+yjNNv6W+dzOGnSHXX8555dczO+paFw07xY72e+nVmvGrYZaWvhGvGgQkezsfw7BS5PkrmB0dGQsjgGhuFNUrEF4HcYIU8Hq9eDweOpvtMV3iJ4tpZYB9T/72UFz3jhhDrt/g0llt7lvJ67vcM5z4yfBSenKvRJE6s49/ByFtHCXI0VlfxVYjRBPPMLP5NgQOlppHfdUP6c5/A4o0KG+7nQX1Hya/91GEPLlMZFsaNMZ38HzLnRxP7EIAc/MuYknR1ShCxXIMdnc+SMLoIOTJY0nhVYDAdDLUdbuJ0pW55xPwjFToI5EIIPB4VERujBOlhD8QwMhYfOqzV//nSQ16DEwrA7y4dXvWmO91c5rxKg6EhTtCTUJmyMsQCs2lnwQEJR134jNbAGgqvwXdN5OgdpDKpm+7xPfkUz/nh2SCC/Abx5l/+JMUd909zAQMENd9tKey6wCZuDMivMt2DA72bGJnxwOYjk5xaC61xW8YYIJdHQ9i2BnyAjP6JAR0a020pw6hCg/z8i4e8ZxQMIQAVJ8CqoUIDbdY+vyugfRYo/jIaO/0ZDFtDGBm4lZdWyDrWrZqZl9CaE7f8E7Q+mM5q9ACVfjMFoq73ITfRORCenPXoDhpKpu+iZAmjhLkyKxvDTDFvCO3DDEKuS7nHW2FfPOZ5XzswUvY2pq91FBLnc6Wv/Zy4Kk0qe7hjNOdOcb2tv9Dt1PkB2ayqHAd/ctBXdcGQFKVex4hj1sm6VBsM460KQrOI+QdLs1VVcEfCKAoAiEERJLDrvv6LOSpHjllutu0MUD9S/ceM7Mof9X5ccoiGfDhmnsdTlj7Be1F7wSgpOMuhLSQQuV42U0AlHX8Hp/RLxFuJhOcj99oZM6xL+GxB3MyOtIBvvPsMv792aXs6chn7KL04NjQdcxg14MJ6p9NDwsDT5nd7Gz/+4AkmB1dBrgzvjW1H0V4mJ9/KQCGlaI5tRchBJXR80Y8JxgIgADFI1wJoA7d9Qi8Pi+mYfGFL1z96TEHPEFMGwPs3/VSVgfHeeWd7i/9VxPOsKUwHVxIJjAPr9VNfuwRAHpy12H4ygloDRR23QNAb/RyenPXojhpqo59DY81SPwD3bl8aeP57Go/eWVaSmivN9h1f4JMfFAapMxu9nVuQErJnLyVhL1u3/W9z2E7BgXBSnJ8rq2rKb4diaQ4NA+vOtz+0S/mVY8CQkJw+DLg9brXGzvEl0968FkwbQzQ0JHJavmrLelxf8kAx+0Ryl9P3lUA5PVu6FPgBB1FbwegpOu/Edg4SoCWso8DUNF2O36jaaD97o58vvnMcpLGxEvRZYOWctj9cJJMfHCGdmvHaE7uRqBQXXAZAKad4XhyNwCVUdeQp1lJerRGFKFQMizQBPx9Yl5RXdKI4HArpepxZ0asc2q2gtPCALaepikeGqFxRXwWc/KG+NJPUNAlCr1Rt1pIfmwDAKlQDbpvJl6zndzYkwB0570B01NIKLOfgt6HBto3J0Lc9kIt5tjFzScMS5fUPZHCNgeZ9HDvC5hOhjx/BQUB16/QGN+JxKEwVIm3z9jUmnJDz05kgP4ayf31t09kAI/HXf71tJySuPZpYYDWA0+nssX2z8mLI8bwoWeC1dhqDj6jeUCZ68ldC0B+7DEEDgiFjkL3hLjSjj/Qn6YjEdy+bSFpc2Kudb9/YokgWsKmYcug/d7dJm4HYGaOu983nQxdmaMIFErCLsG7M8eQOET9ZQP5jwBqXyKBEMLNYlIt92fgussAlmnxvfX/kFWKngymxRLYcmRbVirPjo7uOwdIhV3lKpIePDEuEbkAgLzYEwAkQ8sxvcX4jePkJF8cuG/TsVL2d49esFQIwfnnnceFF62kqrISVVXRdZ1D9fVs3vQM++rqRm3bcVin7Bwv4QL3dTYn91KVewEFwZn4PGEMK0VHqp6i4BwKg7M5ntiF5RgkjHaivjKivjK6tWMAA4WyAYQqkJYEnwkZt+/+JQDgWCL9LeDDY760cTAtDNDW0hxxA/aHoyIydk5EOuCeCxFOuQ4b3TcD01uMx+4loLvu1liOq23nxR5nqPZ478HZo/YbCoV43/vex/z584Z97vf7WbJ4MYsXLaT+8BHu+NUdGMbIaGApoXmPQfVlfbPTMejKHHMjhAJVtCT30KMfByS5/nIEChKHXq2VqK+MHH/JAAMIZVAyCiEAifAZyMxIG0UyyevGfGETwLQsAT1JPasC0zqKIaYfut8lYj+xMwFXnIbTe+gndiq8FICc5GBkz9FYhKZEdieMqqp86IMfHEH8oRBCoaSkmA9/ZPTJ1t1kDNMFejS3hlRBoAJwzckZM44qvIS8bhp62nQV3v6/AaQzuLMQfVtTaQyPkOuXEum0mHSN5GmRALrhZI35u//QLDKWh6rc7EGVzXonjuihvdVEOBXEIwXEO7cRTe7hULwCqag0G10gO2lvjSOk+/L3dI5uPr9s1Sqq5lSNO2ZV8RCNRrl01aU8s+mZEdcdGxp3agRzXeL0qgdJ5UYx7QxtMdcR9VLrFsK+AtqTJklDJ+45Qiaah24laYu79xiGidnlenxtw0EmAzBi9ruMYVlZxOhJYloYwHSUrAzgSMGGIxWjN9zeH7ffH8/fC/Rr+ef0/d9/dvW4xwgBcOmqSyd0X79WvnLlhVkZAKBl39DClhn28dCw64d54YQWGfaccI+Lsf09/SnutjV5CT4tS4DePbVBDaeKvLw8CgsnJkVt2xXNgcDJ1y6aajiOu7OxbO+kt4LTwgCpLvVlSXsaD673bWLoT1iVUk54i3i60H+ymmZEX5kSwGOf3tTsiSKTmVgsYEbLDMw6IQS6fnI1jKcWQ72inkm/yGligGyHxL786O7uJhYbu2iXaVkkk4P2iWzbwJcT/YwIIDy+Sb/HaVEC/dbo9aQXZGxmGlny+4Qgc9UqEBB89BlwHMz5c7DmzcJbfxTPoaOgqmTWXQKOJLjhGXeDDjT6FQ4GRloApZS8+OKLrFu3LutYNE0nmRrukt29e/Tzqc3887F97lkVQa/DRTPjZCyF5xrdHNea0hTFIZM9HWHak16ifpvzKhIkDJUtx930MN3IgJRIQEtnyLGOEXYG6yXb5pB3p/gmnfc+LQzgc6QkS32ilQmLt3eNLl67l9RilxVRsGkPalMrmhIgceVq/Dm7iL7o5u51L12OXZRP/rP78DS6buEGv8rB8uwm4KeefIqly5YTCYdQVfce27bRdR3LHs6opmHwyMPZM4gRKunKG3FU196wsDzB6qXt7G0P84R0414uv7SR0ojB9mdnkYn7WFSeZPXSNna2hXlalGFaBqmUK5Es06bHTiL8kkXpPxGxXYeWOSQyRfFMngGmZQnIsbLL0Spj7Erzams7AGZZEQCeJndmmPNmDdzj3e8WBTcWDxp2Kg2bfCu7tEymUvzlz38mlU4TTySIJxKk0ukRxLcsi1/96tfDRPBQmNHFA8QHWFDk6hf1Pe6uIeh1KImYWI6gPenOu+KI+xo6Uu6u2LIHvV9Wn5SUCJLqYNC0bQ3xC/hCY9vOJ4BpYYBCaWXNler0jD0czzF3Rlvzq9y/m9tRkmmcgjzsvtrCvp2uzV6/YOlAOyFhdWz0tfvIkSP89Mc/obm5OWtYyMEDB/nRbT8iFusdtQ+tdNAq61EcakpSSGBfu7vjnZevIZAc6w3QHwVdmes6kY7H3V3F0HlhDRH1fmfwudYQBvB5ODbqgCaIaVkCcrGeA0akyjT7xmYA3/4G0oB5Tp8hSEq8ew+iX7gM49wagg8/hX/XfkQyjTW7AmvuLDyHXZPsxUmLZ6Je2kfGoAKQSCb53W9/hxCCivIy8guL6exoo7V13DOvMXOXYeUsHPj7/Iokfo/D0d4AnWnXWFdb5uoS+ztdhvCqkso8DSnhcE8Q27awnUHi6sbg7yG7ffBZQ5jE7Dw8ukIyQUyLBIiG5PeyzbTj4zCA98gxhG5gVZRgF7rWssBzOwHQLu4LtbZsQhufByB17WBCrUfCOzoN1HH0Ziklx5tb2L1r54SILz0RUrNvHHyOIlk9152xTze4Nn6/x2FJSQopYVura3uYX5DGq0qOJ/ykDQXTHHQpW6aNYzt9407jl65eYBrGgA1AUb2k4+2T3pJMCwNc+cSml/JjI3Pg4qqgdSwmsGx82/eBEOgXua5h796DKL1xrIoSjJpqAIKPP4tIZzCWVGMsXzzQvEq3uaHLmLLqmFJ4Sc77Z6Rv0JlzRVUvBUGTloSfXe2uTnDRrDg+1aG+O0hvn1v3vArX37GzNYJEohuD78MYMvuj1mBSqTHE/iCCU5MbMm0hYbN6U1kL5x3Isl0bisCzbixA5uIVoAiE47jbQiD9hjUgBCKZJnyPGzGUuPE6nLzBUkMXJM0JSYLxINUQyepbsCKDyuasqM7aeT1I4J66QqQEn+pweaUrETb2SYSwz2FRcdqVCC0RDEMbmNkAmjY4sXPthoHf9SHi35Mza0oMEtPGALNtLWuh5L3BsRnAu68etbMHp6QQ7Vw3TS741IsoiRTm/NnoF7rKX/DJF/DuO4STEyb+kXcgvYPqzgVJk0+2Zig1T20XZeWcQ3zRV7Byzhn4LBqwuXF5Kx5Fsqkhl8Pdrva/dl4POX6bo70BDvSt/6sqe/Gqkv2dYXo1FV0ftEiahoVt9VkdkeRahwFX+x+6/nsVfUoKSk4bAxT7nU9lq5t4KKgSU0cflpCS0INPAZB+w2pXCugG4btdr1ryH6/BiYTAkUR/fTdqVy/m/NnExp33ZQAAEwRJREFUP/7OYUxQqTvc2pzhrd0GhRNkBDs0m9Scj5Ko/iyOfzAaK8dv86HzWsgLWjT0BHjgoOtgqogaXF4ZQyK4d5+7dQ16bC6Z7a7pjx3OwzA0nCExAJnMIJFz7KP4ZKLv80EmUT1eksd3TcmhmNPGAFc/9dQjM9oTI1jAAbaGx18GlO4Y9swytEvd2PrAc9vx1R3GyY2Q+OD1oAiUeJLcH/0WJZnGqDmH2Kc/gJM76ADySFgVN/lSc4abWzLM1bPbIYyCC4kt+TrxRf+KUXABQ0uOl0V0PrHyOGURndakj99uK8d2BH6Pwz8tbUVVJJuP5tLYt9W7urqHoMfhYFeQhh4fmja4lbctG0MbXBmLzd0Dv2tD/RaR2VNmSp/W1LDFyXhTts8353jGruRqWUT++iAAyX+4GicnDFKSc8ddKLEkxpJqkm9/EwBqWyd53719QBL0fPWT6CsWD+tOSKjSHWaMUj/YiizACQyPUxBILq3s5eaLmykMmhyL+fnlizNImwqqkLx7WRtFYVcZvP+Au2OZETW4ZHYcRwruqysio6WHrf2plI7sc/Z4ZJp8q8+moesD7miAcH7FjrFez8lgWhmgPGxe48sifru8CrtDY5so/C/txrd7PzISIvH+t4JwZ3zOHX8ByyKzZiWpN64B+pjg2z/Hu7ceJxoh/vF30XvL+zHnjx4nOBoEsKQ0xS2XHOe6hV14FIfnmqL84oUKUoaCIiTX17azsChNwlD5/bZSLEfBpzq8s7YNgWTT0ShNvQLDHJzVlmmja4OWwFJzK4p0pUEqOeiPUL1B4g1P/eSkBz4KpvWAnT82NHa8NX/2v3bmBkfszLo9CiuT1phbNt/Bo2gXrcCeWY7QDbz1x1C7evG0dmKctwRz4VxkMIBv3yGEbhB4YQdKPIk5bzZ2RQnapee50iAQQFgmB4wMx/wj54STv5RZM0q5aFac62s6uGhWnBy/TXfGy593lfB0Qx6OFHhVh/csb2dpaQrdUrhjSwVtfWbef6zpoLowQ2vSx592lBBPxofN/ngsjdPnJFWkwXztPhRMDMMYxgDe4iWanWp9AMR1ILY1HD1y76m+f5hmBgD4/Kzioh25eStPPMoj5hGUmZKyMRQ0kdHxHG9Hv3Ap5qL5eFo68LS0uz9NbejLF2FVV2JVV+HdewhF0/E2HCf49BaEaWHPKMUpysdYPA/t8gvYb+s0t7SMeM4bL6rg2vMjzMnXCHgdujJeHj5YwN17SmjvI3Bx2OBD57UyJz9D2lT4zdZyGmNu3MvauT1cVhXDsBVuf6mc1u409hCnTiZjoA1R/maYmwe0/3isd0D8C6FQfM6Fb0i27M991TDAnxobH3xzYeXXenICIyZ7k0/hksTYgW9qexfCcTAXzUNfthDf4UbUzh7U1g58hxowahZgzShFv2g5SjyJ53gbwjDxHjhCcMMzeI81IzQdGQhwqK2V5ubmEc+YN28+5FSxrSXCAweKuH9/IY3xAI4UqIpkdVWMdy1tJzdg0Zb08auXKmjpKwa9cmacNy3sAgR/2llKXYuDMcTqZ1sO8djgjs7rJJmXuRcFB13TSA2JRfAULzTvueMb758zZ86KVw0DAHy2vLR6T37u0hM/z6gCBcF8bWwvoffgUWTAj1VdhXZBLWpbJ57mdtSuXvzP78CaXY49owxjxWLMmmpELIGnoxvhSNTWTvw79xN84jkOdbRxLIslco95EZu6l3KgK0RvX6l3VUjOq0jy7qVtLCtPoiqSF49H+f32UhK6e8+qyl7esrgTAdy7r4jNDZ5hWr8EYr2pAbMvwFztfsJOO1JKent7kH3LghCC4jkXvnv3C4/sedUxwJ+bjv1tNClwxKewJGMTHSeMzFdXjwyFsObNxjx3CZgm3sONKJpO4LkdqO1dWPMqscuK0VcuQ79gKTLoR4mnUFLuDKwLqll1ACN3KXa4CiFgZlTnssoY19e0c+6MJCGfQ3vSxx93lLLpaC62FHhVyVsWd7J2bi9I+L+6IjbWB8how8PdU4kMhj64FBRYe5lhbHavJZPo2qDp11u0yLznt998B8BUMsC0F4nqxxVWz62NVs4PzRNcwo4i+EOeh890mPjGOunZkUT+/HeUeILUdetIve31WAvmEvmvu1GSaQLP78C/bS/a5ReQvmoVdlkRqbdcReotV6H0xvHWH8PcugU6RuoANSUplq5opTIvQ8g7OFtbkz6eOJzP9pbwQH2B4rDBu5e2URE1MG3BX3aX8uJRBU0bXvMnndLJpAfXfZ8Tp0pzg00swxym+CkeH7MqFkx5jUA4QyQAwB8bG5/7UKTsc40FkRE5AymvSothcO4Eyvl4Dx7FV9+IsWQ+1uxytMsvQJgmnobjCNvGe7iR4GOb8R5pREhwciI4eTnYFSUc6u7IqgOsXFpF7fxCvKqkK+1lW0sO99YV89DBAlqTPkDgURzWzuvhHbXt5AVtOlNefv1SBbtbJJo+PG5Dz5gkE4NbQCEtFmbuIiB7cByHnu7uYTuESNWlR/78iy8M1AN4VUoAgEv8anVdTGvuzh0ZNb6nJMwDB7q4xuvvy5kbHd59h8j/9s9J/NNbMGqqSb79jWirzif0wEZ8W3YjpMS3+yC+3W4YmVVRij2jBK/Mbl7f0x5m1/YymuL+AW9ePzyKw8pZcVZXxcgNWEjghaYo99YV0BtPYljDfTZaxiAZH+oJlczRHibsuJInHosNM/qo4RLu/c36uWN+4UngjJEAALcfq09+La8ob19O5CIniz/gSGEIGjqYIxQUz9hDFxmdwPM78Bxvw5ozC7u0CP28GvSLV+CEgqjdvShplxBKIoWnuZ0DejqrDtDqPY8WuQDNGrw2I2qwZk4vN9R2UFuaIuBxOB738YftZWxqCBNPxLFOqCyVyegnEB9m6k9Rar4EQCqZIpMelBaK6qX0nMves+v5R3YObfOqlQAAb3rx6U//Wlx5/aPzymeceE0Cjy6rwLu5gYu9XoKleYx39qV/6x78O+vQLlpO+porsIsLSF97Jelrr0Rt6cBXdxhv/VHU1k7QerL24VMlpbk6pRGdOfka8wsz5AUGidsY8/PY4Xz2toXQ9DSakRhe4U1CMpEZ5ugBKDeep8J4FgAto5FMDOoJQggic1c/d+ePP/uHcV7ZpHDGMQDAB194fOa/+d5g75uVP2I6OorggUuqSL9wjIt3HCWnqgRf7jiZZpZNYNNLBDZvxVwwF+3i5RjLF2GXF5MpLyazZiUAmUcfhRdHnhfwpnO6WHHucLdFQlfZ0ZrD1uYITTE/hqWjaT3DPHsAju0Qj2UwzeHSYKb+FBV9Gr+u68RPiDf0Fi/V7vnVF0fWkptinJEMAHCVrc2OdySbjhePTN9yFMHjK2eT3NHCZXsbyckPE6woxBsdJ2/PkXjr6vHW1SMVBbtqBsaiuVizKrDLitwzCLLAcgRNcT+dKS+N8QAHOwO0Jf04gGXq6HrPCHEPoGsmiURmYC8PIHCo0h6h2HSriOi6Tm93D0NFhhqdLR++67aXJQnxjGWAS559/Pgjl1226p4edVNXfpZ3IQQvLK+goyDM2heOUbjnKL5IgGBZAf6CCGKMmAIA4Th4DjcOBI0CBAv9kDPyldxbV4jeNXiOgOM4mGYG3ciMmPHgWveSicyw0C5wPXzVmXvIsd0wLy2j9c38QeJ7QkXMWXRxPvf+dszxTxWmvVr4WLj66aefuVpv/2AwNnoO35HZufzPVdXsLcyhN2UQO9RM15ZDxPcfR++M45hZI89OGo5jo+sZUqkY8WQXGS05gvi2LUnEM3R3JUYQP88+RE3qNwPET6dSxHp7hm33VH8u88+9ZOHt3/3omPlqQgofgBRy0mFhZ6wE6Md1T2z6zZ2XXao8bBX+Si/MepIcPbkB7r9qATX72lm+t52oZRLuTqJ3u5Y3NeDDEw3iCfnxBHyoAS+K1wuqGHdLCaBpKeKJ7lGvW6ZNJmOga+YwggJ4ZIZK/TEK+4I7pJQkYrERialqqIjqxZcs+/k3bxl5cuUJkEJEkRIh5ci68ieJM54BAN719DN3/Gb1qobHWs1HjeLcgRSuoXAE7FxcQn1VPkv3trHgcDd+RxLEIagZ2JpB1qQzRUEobjGxzIoZkDOy8NaJRAVwHImhm2gZA9McuQwIHIrMnczSn8LTZ18wTYt4b8+w5A4ANWemLD+ntuzn37+lfURHWSClLBeAEKJjIvePhVcEAwB8YOOmDfe87uLq/221D/QWREQgmF1HSoW8PHv+TA7MLaSmroPKpl5iUsUnJX4cfFLihcFydI7TX0luTDiOg2U6mJaNoVtYoywtirQotnZSZrwwkNEjpSSTSpFMJkdKiKLF5qN3/9THfRN+FdBXDkUKeeCkWmXBK4YBAK57+NlDgPKlS9d0b4tk8sPR6EDhxBPRVRDkyUtmk5Mso/pwN3OP9hBOmwNmAxXwSIkKKDgIwBzFppBManRpYx8GGXQ6KDL3UGzuHJjx4CZzxGNxLGu4HVsoKuE5l++779dfXXxiX+NACDgfQFXV3ePdPG5nk+1guvDzS1bd+fdA8J0EAoRzIsPq62WFlJR1pJjZnKCiLUFerzbiyz+0Zh5tJSOribX5zuOo/6oT+nOIOC3kWkfIsw4SdoZnEVmWRSqRQNNGJsAowXxKFl5603/f9pmfTeS7DsWaNWuWIcV2JMeeePLx0U+imiBeURJgKD6+edO7Nqxd++kHYumGHZlMwBfwE45ERpUICEFrSYTWkghQTkCzKOjVKOjJkJMyaC8KZSU+QImxFYkHXcklYHcTdNoJO22ocqRWYVkWqWRyeBRvHxTVg6/8/N72UGXJY7d99OROqhjs5W0gEYJHTq39cLxiJcBQ/HzV6vVPqN6vdQgbvz9AMBTE5x/faTRVcBwHTdPQMplhyRtD4cmf65RW1b7xj7d9KltZsAlhPeuVJ1c/eQjEHImyZuPGDRtPta9+vCoYoB8/v+Tyux/3+t/WKWyEEPgDAQKBAF6fb/wl4iRh2zamrqPpOnoWMQ+uPV/JmSELZy7/8V9+9plbJvvM1avXvl0g/wwcumLj5eesZ/2kC0S8qhigH/956aV/eFYNvbtFdYTTp3V7PF58Pi8enxeP6kH1eCbMFLZtY1uWm55lmRi6McxleyIU1YMarbLzZi74l7t+8tkfTMV3Wr16tUegbAeWSPjwxo2P3zEV/b5idYCxcNMzz9wI3Pi7dZevOJRWH9nl8xbFbYt02oQhLn+hKChCIISC6D+mBemWFpIOjpQ4tpPVDnAihBCo4VL8JQsOleZGV/76ts+Mbjk6BQih3oyUS4BDxcWFv5+yfqeqozMd/33ZZWsPW+odh1TP7BavUAwmRtjRIIRA8YVRwhW2v3DG7pmlOW+fiBXvVLBmzZolOOIFBCGEfOMTTzzxwFT1/ZphgBPx93WXL2rKyO+0OOrKdsVTmFDw6IoiLCGxhMBQVWyPHxQvUvEgFL9U/LmmN5TXHo7kPBrweNf//j8+NekSLeNh3bp1uY7lPCthEVL85oknH/vgVPZ/Whhg/fr1SmFx2Q9BjnbMqanr1gFpa1/+3Oc+N+lCR69WrF69OiAQD4JYLWBXOCd80X333TclaeH9OC06QHFx8QwHPjUWfwnkNYYtaoG1p2MMr3SsXr06IqTyPwhWA8dRuHaqiQ+TZwCxevXaD4AsGvrh448/kb/6yuzFFwdbguNYtZN8/qsSa9euXexY8q8IFgPHFVVc/dhjjx0dt+EpYFJLwOrVqxcKlH0nfh4KhXj9G980ZlvDMMhkMt15eXlf7P/MtO12VVoP3nzzzdNZjHfasHr1ag+oNwnkt4AwsFv1KG/csGHDadM1JiUBNm7cuH/16rUfU5Bzhn4eCPijwMfHay8EqpTylwODURTica0ROPm87Vcw1rNeeXLNk28RUqyXyFoAAb8N54RvOh1ifyhOixL405/+tFIKT8NY9xiGgaZlYtFo7rCTnLq7u4mE/LNvvfXWxtHavlpw+eXrFqnC/kchxI0S3BJnksMo8p+ncqs3Fs5AQ5Bg5/Y9N1x5xZWnfYv1MiMiFZkjpKhwYIGQrEQ4FUMiExqE5P/5Q/7fPPjggy/bEngGMgD09HZ/f2qORTzDIF1iCwb+aQHnEUUqfyooKXj8r3/969hp0KcBZyADSLw+7yMIxi7k/8pDEkckpJDtiuSAJZXdTz21YYQC/XLjjGIAy7KQUnLhhZfe/C//cnrMqmcxHJNmgCuvvLJSyuF2gJdeeqn83PNXjt9YovSXP3WkQyaTQVUV4yzxXz5MigGuvPLKSulwCMSwftrbJhasKgFN1wdyrjyqtzfgVz8xmTGdxclhUgzg8Xg6Td16AMGwRE5VVX3AmFY+27ZBon/5S5+fNdZ9Z3F6MSkGeOSRR1LAdSd+/tOf/rRSQsNo7QzTRNM0PKpnQnHwZ3H6cFqUwEQiEbccDNvOfkQsgEf1xlVFfe/peP5ZnMVZnMVZnMVZnMVZjI3/D0XvLtx1DJVBAAAAAElFTkSuQmCC\">"); 
                                      
                        
                        
                        

                        client.println("<div class=\"container\">");

                                    client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color1\" role=\"button\">Change Color</a> ");
                                    client.println( "<input class=\"jscolor\" id=\"rgb1\" type=\"button\">");

                                    //   client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> ");

                                    // client.println( "<input class=\"jscolor\" id=\"rgb\" type=\"button\">");
                                    
                                client.println("</div>");

                                //Button State                                  
                                  if (output15State=="off") {              
                                    client.println("<div class=\"container-btns\"> <p><a href=\"/15/?r255g255b255&\"><button class=\"button off \"> <span class=\"circle-off\"></span>  </button></a></p> </div>");
                                  } else {
                                    client.println("<p><a href=\"/15/?r0g0b0&\"><button class=\"button onn \">  <span class=\"circle-on\"></span></button></a></p>");
                                  }
      

      
                    client.println("</div>");


                    client.println("<div>");
                          client.println("<p>GPIO 19 - State " + output19State + "</p>");


                          
                                client.println("<img src = \"data:image/png;base64,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\">"); 
                          
                          
                          
                          
                          client.println("<div class=\"container\">");

                                    client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color1\" role=\"button\">Change Color</a> ");

                                    client.println( "<input class=\"jscolor\" id=\"rgb1\" type=\"button\">");

                                    //   client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> ");

                                    // client.println( "<input class=\"jscolor\" id=\"rgb\" type=\"button\">");

                                    

                                client.println("</div>");
                                
                                // If the output19State is off, it displays the ON button 
 

                               if (output19State=="off") {
                                      client.println("<div class=\"container-btns\"> <p> <a href=\"/19/?r255g255b255&\"><button class=\"button off \"> <span class=\"circle-off\"></span>  </button></a></p></div>");
                                    } else {
                                      client.println("<p><a href=\"/19/?r0g0b0&\"><button class=\"button onn \">  <span class=\"circle-on\"></span></button></a></p>");
                                    }                
                    client.println("</div>");
      

                    client.println("<div>");
                        client.println("<p>GPIO 21 - State " + output21State + "</p>");

                            client.println("<img src = \"data:image/png;base64,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\">");             

                      client.println("<div class=\"container\">");

                                    // client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color1\" role=\"button\">Change Color</a> ");

                                    // client.println( "<input class=\"jscolor\" id=\"rgb1\" type=\"button\">");

                                      client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> ");

                                    client.println( "<input class=\"jscolor\" id=\"rgb\" type=\"button\">");

                                client.println("</div>");
                                
                          if (output21State=="off") {
                                      client.println("<div class=\"container-btns\"> <p> <a href=\"/21/?r255g255b255&\"><button class=\"button off \"> <span class=\"circle-off\"></span>  </button></a></p></div>");
                                    } else {
                                      client.println("<p><a href=\"/21/?r0g0b0&\"><button class=\"button onn \">  <span class=\"circle-on\"></span></button></a></p>");
                                    } 
                    client.println("</div>");


                
                    // client.println("function update(picker){document.getElementById('rgb-15').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
                                   
                    // client.println("document.getElementById(\"change_color-15\").href=\"/15/?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");

                    
                    // client.println("function update(picker){document.getElementById('rgb-2').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
                                   
                    // client.println("document.getElementById(\"change_color-2\").href=\"/2/?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");

                    
                // ---------------SCRIPTS------------------------
                client.println("<script>");
                    client.println("document.getElementById('rgb').addEventListener(\"change\", function(e){updateColor(this.jscolor)})");

                                                
                    // client.println("function updateColor(picker) {document.getElementById(\"change_color\").href=\"/2/?r\" + Math.round(picker.rgb[0]) + \"g\" + Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");
                    
                      client.println("function updateColor(picker) {document.getElementById(\"change_color\").href=\"/21/?r\" + Math.round(picker.rgb[0]) + \"g\" + Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");
                      
                      //   client.println("function updateColor(picker) {document.getElementById(\"change_color\").href=\"/19/?r\" + Math.round(picker.rgb[0]) + \"g\" + Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");
                        
                      //     client.println("function updateColor(picker) {document.getElementById(\"change_color\").href=\"/21/?r\" + Math.round(picker.rgb[0]) + \"g\" + Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");




                    //   client.println("document.getElementById('rgb-15').addEventListener(\"change\", function(e){updateColor(this.jscolor)})");

                                                
                    // client.println("function updateColor(picker) {document.getElementById(\"change_color-15\").href=\"/15/?r\" + Math.round(picker.rgb[0]) + \"g\" + Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");

                // client.println("</script>");
                                                
                    // client.println("document.getElementById('rgb-15').addEventListener(\"change\", function(e){updateColor-15(this.jscolor)})");
                    // client.println("function updateColor-15(picker) {document.getElementById(\"change_color\").href=\"/15/?r\" + Math.round(picker.rgb[0]) + \"g\" + Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");


                    // /2



                    // client.println("document.getElementById('rgb-15').addEventListener(\"change\", function(e){updateColor-15(this.jscolor)})");
                            
                    // client.println("function updateColor-15(picker) {document.getElementById(\"change_color-15\").href=\"/15/?r\" + Math.round(picker.rgb[0]) + \"g\" + Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");


                    




                client.println("</script>");
                
                
             client.println("</body></html>");
            
            
            // The HTTP response ends with another blank line
            client.println();

  
                  // Request sample: /?r201g32b255&
            // Red = 201 | Green = 32 | Blue = 255
            if(header.indexOf("GET /2/?r") >= 0) {
              pos1_2 = header.indexOf('r');
              pos2_2 = header.indexOf('g');
              pos3_2 = header.indexOf('b');
              pos4_2 = header.indexOf('&');
              redString2 = header.substring(pos1_2+1, pos2_2);
              greenString2 = header.substring(pos2_2+1, pos3_2);
              blueString2 = header.substring(pos3_2+1, pos4_2);
              /*Serial.println(redString2.toInt());
              Serial.println(greenString2.toInt());
              Serial.println(blueString2.toInt());*/
              ledcWrite(redChannel_2, redString2.toInt());
              ledcWrite(greenChannel_2, greenString2.toInt());
              ledcWrite(blueChannel_2, blueString2.toInt());
            }  


            else if(header.indexOf("GET /15/?r") >= 0) {
              pos1_15 = header.indexOf('r');
              pos2_15 = header.indexOf('g');
              pos3_15 = header.indexOf('b');
              pos4_15 = header.indexOf('&');
              redString15 = header.substring(pos1_15+1, pos2_15);
              greenString15 = header.substring(pos2_15+1, pos3_15);
              blueString15 = header.substring(pos3_15+1, pos4_15);
     
              ledcWrite(redChannel_15, redString15.toInt());
              ledcWrite(greenChannel_15, greenString15.toInt());
              ledcWrite(blueChannel_15, blueString15.toInt());
            }  

            
    else if(header.indexOf("GET /19/?r") >= 0) {
              pos1_19 = header.indexOf('r');
              pos2_19 = header.indexOf('g');
              pos3_19 = header.indexOf('b');
              pos4_19 = header.indexOf('&');
              redString19 = header.substring(pos1_19+1, pos2_19);
              greenString19 = header.substring(pos2_19+1, pos3_19);
              blueString19 = header.substring(pos3_19+1, pos4_19);
     
              ledcWrite(redChannel_19, redString19.toInt());
              ledcWrite(greenChannel_19, greenString19.toInt());
              ledcWrite(blueChannel_19, blueString19.toInt());
            }   
                         
    else if(header.indexOf("GET /21/?r") >= 0) {
              pos1_21 = header.indexOf('r');
              pos2_21 = header.indexOf('g');
              pos3_21 = header.indexOf('b');
              pos4_21 = header.indexOf('&');
              redString21 = header.substring(pos1_21+1, pos2_21);
              greenString21 = header.substring(pos2_21+1, pos3_21);
              blueString21 = header.substring(pos3_21+1, pos4_21);
     
              ledcWrite(redChannel_21, redString21.toInt());
              ledcWrite(greenChannel_21, greenString21.toInt());
              ledcWrite(blueChannel_21, blueString21.toInt());
            }   
                        
                        
                               

            
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

}