#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "control.h"
#include "display_ui.h"

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initDisplay()
{
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    while (true)
    {
      digitalWrite(PIN_ALARM_LED, !digitalRead(PIN_ALARM_LED));
      delay(200);
    }
  }

  display.clearDisplay();
  display.display();
}

void showStartupScreen()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(8, 20);
  display.println(F("Cold Plasma"));
  display.display();
  delay(1600);
}

void updateDisplay(const SystemData &sys)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print(F("State: "));
  display.println(stateToText(sys.state));

  display.setCursor(0, 12);
  display.print(F("F: "));
  display.print(sys.measuredFrequencyHz, 1);
  display.println(F(" Hz"));

  display.setCursor(0, 22);
  display.print(F("D: "));
  display.print(sys.measuredDutyCycle, 1);
  display.println(F(" %"));

  display.setCursor(0, 32);
  display.print(F("T: "));
  display.print(sys.measuredTempC, 1);
  display.println(F(" C"));

  display.setCursor(0, 42);
  display.print(F("Fan PWM: "));
  display.print(sys.fanPwm);

  display.setCursor(0, 52);
  display.print(F("Relay: "));
  display.print(sys.relayEnabled ? F("ON") : F("OFF"));

  if (sys.state == STATE_RUNNING)
  {
    display.setCursor(82, 12);
    display.print(F("t:"));
    display.print(getStopwatchSeconds(sys));
    display.print(F("s"));
  }

  display.display();
}