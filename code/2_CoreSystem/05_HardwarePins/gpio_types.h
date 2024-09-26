// gpio_u8_t.h
#ifndef GPIO_U8_T_H
#define GPIO_U8_T_H

#include <stdint.h>


    //   gpio.push_back(gpio_u8_t()); // add new GPIO
    // gpio[0].setPin(2);
    // gpio[0].setInverted(true);


struct gpio_u8_t {
  // The data member now stores the pin number in the lower 5 bits,
  // the pull-enabled flag in bit 6, and the inversion flag in bit 7.
  uint8_t data = 0;

  // Getter for the pin number (5 lower bits)
  uint8_t getPin() const {
      return data & 0x1F;  // Mask to get only the lower 5 bits
  }

  // Setter for the pin number (5 lower bits)
  void setPin(uint8_t pin) {
      data = (data & 0xE0) | (pin & 0x1F);  // Preserve MSBs and set pin number
  }

  // Getter for the inversion flag (MSB, bit 7)
  bool isInverted() const {
      return data & 0x80;  // Check if MSB is set
  }

  // Setter for the inversion flag (MSB, bit 7)
  void setInverted(bool inverted) {
      if (inverted) {
          data |= 0x80;  // Set the MSB to 1
      } else {
          data &= 0x7F;  // Clear the MSB to 0
      }
  }

  // Getter for the pull-enabled flag (6th bit)
  bool isPullEnabled() const {
      return data & 0x40;  // Check if the 6th bit is set
  }

  // Setter for the pull-enabled flag (6th bit)
  void setPullEnabled(bool pullEnabled) {
      if (pullEnabled) {
          data |= 0x40;  // Set the 6th bit to 1
      } else {
          data &= 0xBF;  // Clear the 6th bit to 0
      }
  }

  // Determines the pull direction based on the inversion flag
  const char* getPullDirection() const {
      if (!isPullEnabled()) {
          return "None";
      }
      return isInverted() ? "Pull-Up" : "Pull-Down";
  }
};

#endif // GPIO_U8_T_H
