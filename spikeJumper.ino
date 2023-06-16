#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// To play connect 20x4 char LCD with SDA to A4 and SCL to A5 then, connect a push button to pin 2. Optionally, connect a PC speaker to pin 4.
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int tick = 0;
bool jump = false;
int jumpCool = 0;
int spikePos = 0;
bool gameOver = false;
int score = 1;

byte playerRun0[] = {
  0x0E,
  0x0F,
  0x0E,
  0x04,
  0x0E,
  0x15,
  0x0A,
  0x12
};

byte playerRun1[] = {
  0x0E,
  0x0F,
  0x0E,
  0x04,
  0x04,
  0x04,
  0x04,
  0x04
};

byte playerJump[] = {
  0x0E,
  0x0F,
  0x0E,
  0x05,
  0x0E,
  0x14,
  0x0A,
  0x12
};

byte playerDead[] = {
  0x0E,
  0x1F,
  0x1B,
  0x11,
  0x1B,
  0x1B,
  0x1F,
  0x1F
};

byte spike[] = {
  0x00,
  0x00,
  0x04,
  0x04,
  0x0E,
  0x0E,
  0x1F,
  0x1F
};

byte ground[] = {
  0x1F,
  0x16,
  0x0D,
  0x1F,
  0x00,
  0x1F,
  0x1F,
  0x1F
};

void setup() {
  pinMode(2, INPUT);
  lcd.begin(20, 4);
  createSprites();
  drawGround();
}

void loop() {
  handlePlayerInput();
  handleSpike();
  handleGameOver();
  drawFrame();
  tick++;
  delay(1000/score);
}

void handlePlayerInput() {
  if (gameOver == false) {
    if (jumpCool == 0 && digitalRead(2) == HIGH) {
      jump = true;
      tone(4, 700, 100);
      jumpCool = 4;
    } else if (jumpCool != 0) {
      jumpCool--;
    }
    if (jumpCool == 1) {
      jump = false;
    }
  }
}

void handleSpike() {
  if (gameOver == false) {
    if (spikePos == -2) {
      spikePos = 19;
    } else {
      spikePos--;
    }
  }
  
}

void handleGameOver() {
  if (spikePos == 9 && jump == false && gameOver == false) {
    gameOver = true;
    tone(4, 130, 200);
  } else if (spikePos == 9 && jump == true && gameOver == false) {
    score++;
  }
}

void createSprites() {
  lcd.createChar(1, playerRun0);
  lcd.createChar(2, playerRun1);
  lcd.createChar(3, playerJump);
  lcd.createChar(4, playerDead);
  lcd.createChar(5, spike);
  lcd.createChar(6, ground);
}

void drawGround() {
  lcd.setCursor(0, 3);
  int i = 0;
  while (i < 20) {
    lcd.write(6);
    i++;
  }
  lcd.setCursor(0, 0);
}

void drawPlayer() {
  if (gameOver == false) {
    if (jump == false) {
      if (tick%2 == 0) {
        lcd.setCursor(9, 2);
        lcd.write(1);
        tone(4, 440, 10);
      } else {
        lcd.setCursor(9, 2);
        lcd.write(2);
        tone(4, 440, 10);
      }
    } else {
      lcd.setCursor(9, 1);
      lcd.write(3);
    }
  } else {
   lcd.setCursor(9, 2);
   lcd.write(4);
  }
  lcd.setCursor(0, 0);
}

void drawSpike() {
  if (spikePos > -1) {
    lcd.setCursor(spikePos, 2);
    lcd.write(5);
    lcd.setCursor(0, 0);
  }
}

void drawScore() {
  lcd.setCursor(0, 0);
  lcd.print(score-1);
  lcd.setCursor(0, 0);
}

void clearFrame() {
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 0);
}

void drawFrame() {
  clearFrame();
  drawSpike();
  drawPlayer();
  drawScore();
  if (gameOver == true) {
    lcd.setCursor(5, 1);
    lcd.print("Game Over!");
  }
}
