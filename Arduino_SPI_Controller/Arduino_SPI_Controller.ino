 
 #include<SPI.h>                             //Library for SPI

#define LINE_BUF_SIZE 128   // Maximum input string length
#define ARG_BUF_SIZE 64     // Maximum argument string length
#define MAX_NUM_ARGS 8      // Maximum number of arguments

bool error_flag = false;

char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

// Function declarations
int cmd_wave();
int cmd_start();
int cmd_stop();
int cmd_clear();

// List of functions pointers corresponding to each command
int (*commands_func[])(){
    &cmd_wave,
    &cmd_start,
    &cmd_stop,
    &cmd_clear
};

// List of command names
const char *commands_str[] = {
    "wave",
    "start",
    "stop",
    "clear"
};

int num_commands = sizeof(commands_str) / sizeof(char *);

void setup() {
    Serial.begin(115200);
    pinMode(13, OUTPUT);
    
    SPI.begin();                            //Begins the SPI commnuication
    SPI.setClockDivider(SPI_CLOCK_DIV8);    //Sets clock for SPI communication at 8 (16/8=2Mhz)
 
    cli_init();
}

void loop() {
     my_cli();
}

void cli_init(){
    Serial.println("FES-Controller Arduino CLI Control Interface");
}

void my_cli() {
    Serial.print("> ");
 
    read_line();
    
    if(!error_flag){
        parse_line();
    }
    if(!error_flag){
        execute();
    }
 
    memset(line, 0, LINE_BUF_SIZE);
    memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);
 
    error_flag = false;
}

void read_line() {
    char line_string[LINE_BUF_SIZE];
    char line_char = 0;
    int index = 0;

    // Clear Buffer
    for (int i = 0; i < LINE_BUF_SIZE; i++) {
      line[i] = 0;
    }

    // Get string from serial
    while (line_char != '\r') {
      if (Serial.available() > 0) {
        line_char = Serial.read();
        // Handle Special chars
        if (line_char == 0x7F) { // Backspace
          // Do Nothing
        } else if (line_char == 0x1B5B41) { // Up Arrow
          // Do Nothing
        } else if (line_char == 0x1B5B42) { // Down Arrow
          // Do Nothing
        } else {
          Serial.print(line_char);
          line[index] = line_char;
          index++;
        }
      }
    }
    Serial.print('\n');
}

void parse_line() {
    char *argument;
    int counter = 0;
 
    argument = strtok(line, " ");
 
    while((argument != NULL)){
        if(counter < MAX_NUM_ARGS){
            if(strlen(argument) < ARG_BUF_SIZE){
                strcpy(args[counter],argument);
                args[counter][strlen(args[counter])] = '\0';
                argument = strtok(NULL, " ");
                if (argument == NULL) {
                  args[counter][strlen(args[counter]) - 1] = '\0';
                }
                counter++;
            }
            else{
                Serial.println("Input string too long.");
                error_flag = true;
                break;
            }
        }
        else{
            break;
        }
    }
}

int execute() {  
    int i = 0;
    for(i=0; i < num_commands; i++){
        if(strcmp(args[0], commands_str[i]) == 0){
            return (*commands_func[i])();
        }
    }
    return 0;
}

int cmd_exit() {
    Serial.println("Exiting CLI.");
 
    while(1);
}

int cmd_stop() {
    uint8_t data[1];
    data[0] = 'D';
    Serial.println("sending stop byte");
    send_spi(data, 1);
}

int cmd_start() {
    uint8_t data[1];
    data[0] = 'S';
    Serial.println("sending start byte");
    send_spi(data, 1);
}

int cmd_clear() {
    uint8_t data[2];
    data[0] = 'C';
    Serial.println("Which channel to delete? (1 -> 16)");
    read_line();
    data[1] = atoi(line) - 1;
    if (atoi(line) == 0) {
      Serial.println("Bad Channel. Exit.");
      return 0;
    }
    Serial.println("sending channel to clear");
    send_spi(data, 2);
}

int cmd_wave() {
    uint8_t waveData[10];
    waveData[0] = 'W';
    Serial.println("Waveform (sine/square/sawtooth/triangle)");
    read_line();
    if (!memcmp(line, "sine", 4)) {
      waveData[1] = 's';
    } else if (!memcmp(line, "square", 4)) {
      waveData[1] = 'q';
    } else if (!memcmp(line, "sawtooth", 4)) {
      waveData[1] = 'a';
    } else if (!memcmp(line, "triangle", 4)) {
      waveData[1] = 't';
    } else {
      Serial.println("Bad Waveform. Defaulting: sawtooth");
      waveData[1] = 'a';
    }
    Serial.println("Channel: (1 -> 16)");
    read_line();
    waveData[2] = atoi(line);
    if (atoi(line) == 0) {
      Serial.println("Bad Channel. Defaulting to 1");
      waveData[2] = 1;
    }
    Serial.println("Frequency: (Hz)");
    read_line();
    waveData[3] = atoi(line);
    if (atoi(line) == 0) {
      Serial.println("Bad Frequency. Defaulting to 50 Hz");
      waveData[3] = 50;
    }
    Serial.println("Amplitude: (0 -> 255)");
    read_line();
    waveData[4] = atoi(line);
    if (atoi(line) == 0) {
      Serial.println("Bad Amplitude. Defaulting: 100");
      waveData[4] = 150;
    }
    Serial.println("Pulse Width: (uS)");
    read_line();
    waveData[5] = atoi(line) / 4;
    if (atoi(line) == 0) {
      Serial.println("Bad Width. Defaulting: 250uS");
      waveData[5] = 50;
    }
    Serial.println("Envelope? (y/n)");
    read_line();
    if (!memcmp(line, "y", 1) && !memcmp(line, "n", 1)) {
      Serial.println("Error. Default no envelope");
    } else if(!memcmp(line, "y", 1)) {
      waveData[6] = 't';
      Serial.println("Ramp In Width: (mS)");
      read_line();
      waveData[7] = atoi(line) / 10;
      if (atoi(line) == 0) {
        Serial.println("Bad Width. Defaulting: 250mS");
        waveData[7] = 100 / 10;
      }
      Serial.println("Ramp Out Width: (mS)");
      read_line();
      waveData[8] = atoi(line) / 10;
      if (atoi(line) == 0) {
        Serial.println("Bad Width. Defaulting: 250mS");
        waveData[8] = 100 / 10;
      }
      Serial.println("Envelope Width: (mS)");
      read_line();
      waveData[9] = atoi(line) / 50;
      if (atoi(line) == 0) {
        Serial.println("Bad Width. Defaulting: 1000mS");
        waveData[9] = 1000 / 50;
      }
      Serial.println("sending data");
      send_spi(waveData, 10);
      return 0;
    }
    waveData[6] = 'f';
    Serial.println("sending data");
    send_spi(waveData, 7);
}

void send_spi(uint8_t* spiData, uint8_t size) {
  for (int i = 0; i < size; i++) {
    SPI.transfer(spiData[i]);
    delay(5);
  }
}
